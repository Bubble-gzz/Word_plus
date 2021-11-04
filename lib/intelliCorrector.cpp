#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <ctime>
#include <windows.h>
#define rep(i,j,k) for (i=j;i<=k;i++)
using namespace std;
const int N=100;
const int K=50;
const int Size=500000;
string s;
string tmp[N];
char line[1000];
int len,i,j,sn,flag,found,Lim;
int cs,k,maxK,End[K],Changed[K],bp[K];
clock_t start_time,end_time;
struct cp{
    string s;
    map<string,int> list;
    cp(string S):s(S){}
    cp(){}
};
struct seg{
    vector<string> a;
    vector<int> b;
    seg(){}
};
map<string,cp> maps;
map<string,int> words;
map<string,int> ans;
map<string,seg> Ans;
void showtime(string spc)
{
    end_time=clock();
    double cost=(end_time-start_time)*1.0/CLOCKS_PER_SEC;
    cout<<"     Done "<<spc<<" ";
    printf("%.3lfs",cost);
}
struct dictionary_tree
{
    int pos,tot;
    int ch[Size][26];
    int walk(char wh,int modify)
    {
        if (!ch[pos][wh-'a']) {
            if (modify) ch[pos][wh-'a']=++tot;
            else return 0;
        }
        pos=ch[pos][wh-'a'];
        return 1;
    }
    int walk(string s,int modify)
    {
        int tmp=pos;
        if (modify) pos=0;
        for (int i=0;i<s.length();i++)
            if (modify) walk(s[i],1);
            else if (!walk(s[i],0)) {pos=tmp; return 0;}
        return 1;
    }
}Tree;
bool isVowel(char ch) {
    return (ch=='a' || ch=='o' || ch=='e' || ch=='u' || ch=='i') ;
}
void print_division()
{
    int i,j;
    End[0]=-1;
    cs++;
    printf("case #%d: ",cs);
    for (i=1;i<=k;i++)
    {
        for (j=End[i-1]+1;j<=End[i];j++) printf("%c",s[j]);
       printf("/");//,vowel[i]?"VOWEL":"CONSONANT");
    }
    printf(" k=%d\n",k);
}
bool special_open(string s)
{
    if (s[1]=='r' || s[1]=='l') return 1;
  //  if (s=="ure" || s=="ere" || s==) return 1;
    return 0;
}
void replace(int now,int changed,string t)
{
    string part=s.substr(End[now-1]+1,End[now]-End[now-1]),_part;
    _part=part;
    if (part.length()==2 && !isVowel(part[0]) && part[1]=='e') //开音节简化，去掉e
        part.erase(part.length()-1);
    if (now==1) Tree.pos=0;
    int pos_tmp=Tree.pos;
    if (now>k)
    {
        if (words.find(t)!=words.end()) {
            if (ans.find(t)==ans.end())
            {
            //    cout<<endl<<"         found "<<t;
            //    showtime("at");
                ans[t]=1;
                for (int i=1;i<=k;i++) {
                    Ans[t].a.push_back(t.substr(bp[i-1]+1,bp[i]-bp[i-1]));
                    Ans[t].b.push_back(Changed[i]);
                }
            }
            found=1;
        }
        return ;
    }
    if (maps.find(part)==maps.end()) return ;
    cp tmp=maps[part];
    if (Tree.walk(_part,0)) {
        Changed[now]=0;
        bp[now]=bp[now-1]+_part.length();
        replace(now+1,changed,t+_part);
    }
    Tree.pos=pos_tmp;
    if (changed>=Lim) return ;
    for (auto i=tmp.list.begin();i!=tmp.list.end();i++)
    {
        string ss=(*i).first;
        Changed[now]=1;
        if (Tree.walk(ss,0)) {
            bp[now]=bp[now-1]+ss.length();
            replace(now+1,changed+1,t+ss);
        }
        Changed[now]=0;
        Tree.pos=pos_tmp;
    }
}
void divide(int i)
{
    if (i+1>len) {
        if (Lim<0) print_division();
        if (k>maxK) maxK=k;
        replace(1,0,"");
        return ;
    }
    int j,flag;
    string tmp;
    int o;
    if (isVowel(s[i]) && !isVowel(s[i+1]) && s[i+2]=='e') { //开音节判定
        k++; End[k]=i;
        k++; End[k]=i+2;
        divide(i+3);
        k-=2;
        if (i+3>len-1 && !special_open(s.substr(i,3))) return ;
    }
    for (j=len-1;j>=i;j--)
        if ( !(maps.find(s.substr(i,j-i+1))==maps.end()) ) 
        {
            tmp=s.substr(i,j-i+1);
            if (tmp.length()==2 && isVowel(tmp[0]) && (tmp[1]=='l' || tmp[1]=='r'))
                if ((i+2<len) && isVowel(s[i+2])) continue; //e re
            k++; End[k]=j;
            divide(j+1);
            k--;
            flag=0;
            
            for (o=0;o<tmp.length();o++)
                if (!isVowel(tmp[o])) {flag=1; break; } 
            if (tmp[0]==tmp[1]) break; //双写
            if (tmp=="tion" || tmp=="sion") break;
            if (tmp=="ex" || tmp=="tr") break;
            if (flag) continue; //有辅音，都可能继续分解
            if (tmp=="ia" || tmp=="io" || tmp=="ie") continue;
            break;
        }
}
void correct()
{
    found=0;
    ans.clear();
    Ans.clear();
    End[0]=bp[0]=-1;
    Lim=0; maxK=k=0;
    divide(0);
    if (s.length()<=3) Lim=1;
    else if (s.length()<=4) Lim=2;
    else Lim=maxK/2;
    divide(0);
    if (!found) printf("Cannot recognize!\n");
    else {
        printf("\nGuess you want:\n");
        for (auto i=ans.begin();i!=ans.end();i++)
        {
            seg ss=Ans[(*i).first];
            for (int i=0;i<ss.a.size();i++)
            {
                if (ss.b[i]) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                cout<<ss.a[i];
            }
            cout<<endl;
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
int main()
{
    FILE *wd=fopen("vocalbulary.json","r");
    fgets(line,500,wd);
    fgets(line,500,wd);
    while (fgets(line,500,wd)!=NULL)
    {
        int l,r;
        for (l=0;l<strlen(line);l++)
            if (line[l]==':') break;
        for (r=l+1;r<strlen(line);r++)
            if (line[r]==',') break;
        l+=2; r-=2;
        if (l>=r) continue;
        string s;
        s.append(line+l,r-l+1);
        Tree.walk(s,1);
        words[s]=1;
    }
    fclose(wd);
    ifstream _in("combination.in");
//    Tree.pos=0;
//    printf("%d\n",Tree.walk("facilitate",0));
    while (1)
    {
     
        _in>>s;
        if (s[0]=='$') break;
        sn=0; flag=0;
        while (1)
        {
            if (s[0]=='.') break;
            if (s[s.length()-1]=='.') {s.erase(s.length()-1); flag=1;}
            sn++;
            tmp[sn]=s;
            if (maps.find(s)==maps.end())
                maps[s]=cp(s);
            if (flag) break;
            _in>>s;
        }
        rep(i,1,sn)
            rep(j,1,sn)
                if (i!=j)
                    if (maps[tmp[i]].list.find(tmp[j])==maps[tmp[i]].list.end())
                        maps[tmp[i]].list[tmp[j]]=1;
    }
    _in.close();
//    fclose(stdin);
    while (1)
    {
        cs=k=0;
        cin>>s;
  //      if (words.find(s)!=words.end()) printf("word found!\n");
    //    else printf("word not found!\n");
        
//    if (maps.find(s)==maps.end()) printf("Not found!\n");
//    else cout<<s<<endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        len=s.length();
        start_time=clock();
        correct();
        cout<<endl; showtime("in");
        cout<<endl;
    }
    return 0;
}
