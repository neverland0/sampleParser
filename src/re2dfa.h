#ifndef RE2DFA_H
#define RE2DFA_H
#include <vector>
#include <stack>
#include <sstream>
#include <stddef.h>
#include <set>
#include <queue>
#include <map>
#include<iomanip>

using namespace std;

struct nst
{
    vector<int> a[2], e;
    bool f=0;
};

struct dst
{
    int a[2] ;
    bool f=0;
};

class re2dfa
{
public:
    re2dfa(string regex);
    stringstream ss;
    stringstream ss2;
    stringstream ss3;
    void /*re2dfa::*/display_nfa();
    void /*re2dfa::*/print_dfa();
    void /*re2dfa::*/print();
private:
    int final_state;
    int start_state;
    string regexp;
    vector<nst> nfa;
    vector<dst> dfa;
    stack<int> st;
    int nfa_size, dfa_size;
    string dispregex;
    struct nst init_nfa_state;
    struct dst init_dfa_state;
    string /*re2dfa::*/insert_concat(string regexp);
    void /*re2dfa::*/character(int i);
    void /*re2dfa::*/union_();
    void /*re2dfa::*/concatenation();
    void /*re2dfa::*/kleene_star();
    void /*re2dfa::*/postfix_to_nfa(string postfix);
    int /*re2dfa::*/priority(char c);
    string /*re2dfa::*/regexp_to_postfix(string regexp);

    void /*re2dfa::*/epsilon_closure(int state,set<int>&si);
    set<int> /*re2dfa::*/state_change(int c,set<int>&si);
    void /*re2dfa::*/nfa_to_dfa(set<int>&si,queue<set<int> >&que,int start_state);

    pair<int,vector<tuple<int,int,bool> > > /*re2dfa::*/minimize_dfa(vector<dst> dfa);
};

#endif // RE2DFA_H
