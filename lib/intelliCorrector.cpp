#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <queue>
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
const int DIST[7]={1,1,3,5,8,10,20};
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
struct pr{
    string s;
    int dist;
    pr(string S,int DIST):s(S),dist(DIST){}
    pr(){}
};
struct cmp{
    bool operator () (const pr &A,const pr &B) {
        return A.dist>B.dist;
    }
};
priority_queue<pr,vector<pr>,cmp> heap;
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
bool isVowel(char ch) {
    return (ch=='a' || ch=='o' || ch=='e' || ch=='u' || ch=='i') ;
}
void setcolor(int x) {
    if (0<=x && x<16) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
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
struct Trie
{
    int pos,tot;
    int ch[Size][26];
    void clear()
    {
        pos=0; tot=0;
        memset(ch[0],0,sizeof(ch[0]));
    }
    int walk(char wh,int modify)
    {
        if (!ch[pos][wh-'a']) {
            if (modify) {
                ch[pos][wh-'a']=++tot;
                memset(ch[tot],0,sizeof(ch[tot]));
            }
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
void judge(string t,int dist)
{
    if (words.find(t)!=words.end()) {
        if (ans.find(t)==ans.end())
        {
            ans[t]=dist;
            heap.push(pr(t,dist));
            for (int i=1;i<=k;i++) {
                Ans[t].a.push_back(t.substr(bp[i-1]+1,bp[i]-bp[i-1]));
                Ans[t].b.push_back(Changed[i]);
            }
        }
        else ans[t]=min(ans[t],dist);
        found=1;
    }
}
void replace(int now,int changed,int distance,string t)
{
    string part=s.substr(End[now-1]+1,End[now]-End[now-1]),_part;
    _part=part;
   // if (part.length()==2 && !isVowel(part[0]) && part[1]=='e') //????????????????????????e
   //     part.erase(part.length()-1);
    if (now==1) Tree.pos=0;
    int pos_tmp=Tree.pos;
    if (now>k)
    {
        judge(t,distance);
        return ;
    }
    if (maps.find(part)==maps.end()) return ;
    cp tmp=maps[part];
    if (Tree.walk(_part,0)) {
        Changed[now]=0;
        bp[now]=bp[now-1]+_part.length();
        replace(now+1,changed,distance,t+_part);
    }
    Tree.pos=pos_tmp;
    if (changed>=Lim) return ;
    for (auto i=tmp.list.begin();i!=tmp.list.end();i++)
    {
        string ss=(*i).first;
        Changed[now]=1;
        if (Tree.walk(ss,0)) {
            bp[now]=bp[now-1]+ss.length();
            replace(now+1,changed+1,distance+(*i).second,t+ss);
        }
        Changed[now]=0;
        Tree.pos=pos_tmp;
    }
}
void divide(int i)
{
    if (i+1>len) {
       // if (Lim==0) print_division();
        if (k>maxK) maxK=k;
        replace(1,0,0,"");
        return ;
    }
    int j,flag;
    string tmp;
    int o;
    if (isVowel(s[i]) && !isVowel(s[i+1]) && s[i+2]=='e') { //???????????????
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
            if (tmp[0]==tmp[1]) break; //??????
            if (tmp=="tion" || tmp=="sion") break;
            if (tmp=="ex" || tmp=="tr") break;
            if (flag) continue; //?????????????????????????????????
            if (tmp=="ia" || tmp=="io" || tmp=="ie") continue;
            break;
        }
}
void Output()
{
    if (!found) {
        setcolor(4);
        printf("Cannot recognize!\n");
    }
    else {
        printf("\nGuess you want:\n");
        for (int o=1;o<=5;o++)
        {
            if (heap.empty()) break;
            if (heap.top().dist>Lim*3 && o>1) break;
            auto i=ans.find(heap.top().s);
            seg ss=Ans[(*i).first];
            for (int i=0;i<ss.a.size();i++)
            {
                if (ss.b[i]) setcolor(2);
                else setcolor(7);
                cout<<ss.a[i];
            }
            setcolor(7);
            for (int j=0;j<=25-(*i).first.length();j++) putchar(' ');
            cout<<"distance = "<<(*i).second<<endl;
            heap.pop();
        }
    }
    setcolor(7);
    cout<<endl;
}
void correct()
{
    found=0;
    ans.clear();
    Ans.clear();
    while (!heap.empty()) heap.pop();
    End[0]=bp[0]=-1;
    Lim=0; maxK=k=0;
    divide(0);
    Lim=maxK;
/*
    if (s.length()<=3) Lim=1;
    else if (s.length()<=4) Lim=2;
    else Lim=maxK/2;
*/
    divide(0);
    Output();
}
void reload()
{
    Tree.clear();
    words.clear();
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

    maps.clear();
    ifstream _in("combination.in");
    while (1)
    {
     
        _in>>s;
        if (s[0]=='$') break;
        sn=0; flag=0;
        int type=1;
        while (1)
        {
            if (s[0]=='.') break;
            if (s[s.length()-1]=='.') {s.erase(s.length()-1); flag=1;}
            if (s[0]>='0' && s[0]<='9') {
                type=atoi(s.c_str());
            }
            else {
                sn++;
                tmp[sn]=s;
                if (maps.find(s)==maps.end())
                    maps[s]=cp(s);
            }
            if (flag) break;
            _in>>s;
        }
        rep(i,1,sn)
            rep(j,1,sn)
                if (i!=j)
                    if (maps[tmp[i]].list.find(tmp[j])==maps[tmp[i]].list.end())
                        maps[tmp[i]].list[tmp[j]]=DIST[type];
                    else maps[tmp[i]].list[tmp[j]]=min(maps[tmp[i]].list[tmp[j]],DIST[type]);
    }
    _in.close();
}
int main()
{
    reload();
    while (1)
    {
        cs=k=0;
        setcolor(7);
        cin>>s;
        if (s[0]=='$') {
            reload();
            setcolor(6);
            printf("\n Hot reloaded.\n");
            continue;
        }
    //    if (words.find(s)!=words.end()) printf("word found!\n");
    //    else printf("word not found!\n");
        
//    if (maps.find(s)==maps.end()) printf("Not found!\n");
//    else cout<<s<<endl;

        len=s.length();
        start_time=clock();
        correct();
        cout<<endl; showtime("in");
        cout<<endl;
    }
    return 0;
}
