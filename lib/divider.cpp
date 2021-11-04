#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#define rep(i,j,k) for (i=j;i<=k;i++)
using namespace std;
const int N=100;
const int K=50;
string s;
string tmp[N];
int len,i,j,sn,flag;
int cs,k,End[K];
struct cp{
    string s;
    map<string,int> list;
    cp(string S):s(S){}
    cp(){}
};
map<string,cp> maps;
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
    printf("\n");
}
bool special_open(string s)
{
    if (s[1]=='r' || s[1]=='l') return 1;
  //  if (s=="ure" || s=="ere" || s==) return 1;
    return 0;
}
void dfs(int i)
{
    if (i+1>len) {
        print_division();
        return ;
    }
    int j;
    string tmp;
    int o;
    if (isVowel(s[i]) && !isVowel(s[i+1]) && s[i+2]=='e') { //开音节判定
        k++; End[k]=i;
        k++; End[k]=i+2;
        dfs(i+3);
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
            dfs(j+1);
            k--;
            for (o=1;o<tmp.length();o++)
                if (!isVowel(tmp[o]) && tmp[o]!='n' && tmp[o]!='m') continue; //有辅音，都可能继续分解
            if (tmp.length()==2 && isVowel(tmp[0]) && isVowel(tmp[1]) && tmp[0]!=tmp[1]) continue; //i a/ e a 
            if (tmp.length()==3 && isVowel(tmp[0]) && isVowel(tmp[2]) && (tmp[1]=='l' || tmp[1]=='r')) continue;
            //ere
            if (tmp=="tri") continue;
            if (tmp=="si" || tmp=="sai" || tmp=="psy") continue;
            if (tmp=="le") continue;
            break;
        }
}
int main()
{
    ifstream _in("combination.in");
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
        cs=0;
        cin>>s;
//    if (maps.find(s)==maps.end()) printf("Not found!\n");
//    else cout<<s<<endl;

        len=s.length();
        dfs(0);
    }
    return 0;
}
