#include "re2dfa.h"
#include <algorithm>

re2dfa::re2dfa(string regexp)
{
    this->regexp=regexp;
    nfa_size=0;
    dfa_size=0;
    dispregex="";
}

string re2dfa::insert_concat(string regexp)
{
    string ret="";
    char c,c2;
    for(unsigned int i=0; i<regexp.size(); i++)
    {
        c=regexp[i];
        if(i+1<regexp.size())
        {
            c2=regexp[i+1];
            ret+=c;
            if(c!='('&&c2!=')'&&c!='+'&&c2!='+'&&c2!='*')
            {
                ret+='.';
            }
        }
    }
    ret+=regexp[regexp.size()-1];
    return ret;
}

void re2dfa::character(int i)
{
    nfa.push_back(init_nfa_state);
    nfa.push_back(init_nfa_state);
    nfa[nfa_size].a[i].push_back(nfa_size+1);
    st.push(nfa_size);
    nfa_size++;
    st.push(nfa_size);
    nfa_size++;
}

void re2dfa::union_()
{
    nfa.push_back(init_nfa_state);
    nfa.push_back(init_nfa_state);
    int d = st.top(); st.pop();
    int c = st.top(); st.pop();
    int b = st.top(); st.pop();
    int a = st.top(); st.pop();
    nfa[nfa_size].e.push_back(a);
    nfa[nfa_size].e.push_back(c);
    nfa[b].e.push_back(nfa_size+1);
    nfa[d].e.push_back(nfa_size+1);
    st.push(nfa_size);
    nfa_size++;
    st.push(nfa_size);
    nfa_size++;
}

void re2dfa::concatenation()
{
    int d = st.top(); st.pop();
    int c = st.top(); st.pop();
    int b = st.top(); st.pop();
    int a = st.top(); st.pop();
    nfa[b].e.push_back(c);
    st.push(a);
    st.push(d);
}

void re2dfa::kleene_star()
{
    nfa.push_back(init_nfa_state);
    nfa.push_back(init_nfa_state);
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    nfa[nfa_size].e.push_back(a);
    nfa[nfa_size].e.push_back(nfa_size+1);
    nfa[b].e.push_back(a);
    nfa[b].e.push_back(nfa_size+1);
    st.push(nfa_size);
    nfa_size++;
    st.push(nfa_size);
    nfa_size++;
}

void re2dfa::postfix_to_nfa(string postfix)
{
    for(unsigned int i=0; i<postfix.size(); i++)
    {
        switch(postfix[i])
        {
        case 'a':
        case 'b': character(postfix[i]-'a'); break;
        case '*': kleene_star(); break;
        case '.': concatenation(); break;
        case '+': union_();
        }
    }
}

void re2dfa::display_nfa()
{
    dispregex=this->regexp;
    dispregex=insert_concat(dispregex);
    dispregex=regexp_to_postfix(dispregex);
    postfix_to_nfa(dispregex);
    final_state=st.top();st.pop();
    start_state=st.top();st.pop();
    nfa[final_state].f=1;
    ss<<endl<<endl;
    ss<<"------------------------------------------------------------------------\n";
    ss<<"State\ta\tb\teps\taccepting state"<<endl;
    ss<<"------------------------------------------------------------------------\n";
    for(unsigned int i=0; i<nfa.size(); i++)
    {
        ss<<i<<"\t";
        for(unsigned int j=0; j<nfa[i].a[0].size(); j++)ss<<nfa[i].a[0][j]<<' ';
        ss<<"\t";
        for(unsigned int j=0; j<nfa[i].a[1].size(); j++)ss<<nfa[i].a[1][j]<<' ';
        ss<<"\t";
        for(unsigned int j=0; j<nfa[i].e.size(); j++)ss<<nfa[i].e[j]<<' ';
        ss<<"\t";
        if(nfa[i].f)ss<<"Yes"; else ss<<"No";
        ss<<"\n";
    }
    ss<<"------------------------------------------------------------------------\n";
}

int re2dfa::priority(char c)
{
    switch(c)
    {
        case '*': return 3;
        case '.': return 2;
        case '+': return 1;
        default: return 0;
    }
}

string re2dfa::regexp_to_postfix(string regexp)
{
    string postfix="";
    stack<char> op;
    char c;
    for(unsigned int i=0; i<regexp.size(); i++)
    {
        switch(regexp[i])
        {
            case 'a':
            case 'b':
                postfix+=regexp[i]; break;
            case '(':
                op.push(regexp[i]); break;
            case ')':
                while(op.top()!='('){
                    postfix+=op.top();
                    op.pop();
                }
                op.pop();
                break;
            default:
                while(!op.empty())
                {
                    c=op.top();
                    if(priority(c)>=priority(regexp[i]))
                    {
                        postfix+=op.top();
                        op.pop();
                    }
                    else break;
                }
                op.push(regexp[i]);
        }
    }
    while(!op.empty())
    {
        postfix += op.top();
        op.pop();
    }
    return postfix;
}
//nfa 2 dfa
void re2dfa::print_dfa()
{
    set<int> si;
    queue<set<int> > que;
    nfa_to_dfa(si,que,start_state);
    ss2<<endl;
    ss2<<"NFA TO DFA CONVERSION"<<endl;
    ss2<<"---------------------------------------------------------"<<endl;
    ss2<<"STATE\t|\t"<<"a"<<"\t|\t"<<"b"<<"\t|\t"<<"FINAL"<<"\t|"<<endl;
    ss2<<"---------------------------------------------------------"<<endl;
    for(int i=0;i<dfa.size();i++)
    {
        ss2<<i;
        if(dfa[i].a[0]>-1)
        {
            ss2<<"\t|\t"<<dfa[i].a[0];
        }
        else
        {
            ss2<<"\t|\t"<<" ";
        }
        if(dfa[i].a[1]>-1)
        {
            ss2<<"\t|\t"<<dfa[i].a[1];
        }
        else
        {
            ss2<<"\t|\t"<<" ";
        }
        ss2<<"\t|\t"<<dfa[i].f<<"\t|"<<endl;
    }
    ss2<<"---------------------------------------------------------"<<endl;
}

void re2dfa::epsilon_closure(int state,set<int>&si)
{
    for(unsigned int i=0;i<nfa[state].e.size();i++)
    {
        if(si.count(nfa[state].e[i])==0){
            si.insert(nfa[state].e[i]);
            epsilon_closure(nfa[state].e[i],si);
        }
    }
}

set<int> re2dfa::state_change(int c,set<int>&si)
{
    set<int> temp;
    if(c==1)
    {
        for (std::set<int>::iterator it=si.begin(); it!=si.end(); ++it)
        {
            for(unsigned int j=0;j<nfa[*it].a[0].size();j++)
            {
                temp.insert(nfa[*it].a[0][j]);
            }
        }
    }
    else
    {
        for (std::set<int>::iterator it=si.begin(); it!=si.end(); ++it)
        {
            for(unsigned int j=0;j<nfa[*it].a[1].size();j++)
            {
                temp.insert(nfa[*it].a[1][j]);
            }
        }
    }
    return temp;
}

void re2dfa::nfa_to_dfa(set<int>&si,queue<set<int> >&que,int start_state)
{
    map<set<int>, int> mp;
    mp[si]=-1;
    set<int> temp1;
    set<int> temp2;
    int ct=0;
    si.clear();
    si.insert(0);
    epsilon_closure(start_state,si);
    if(mp.count(si)==0)
    {
        mp[si]=ct++;
        que.push(si);
    }
    int p=0;
    bool f1=false;
    while(que.size()!=0)
    {
        dfa.push_back(init_dfa_state);
        si.empty();
        si=que.front();
        f1=false;
        for (set<int>::iterator it=si.begin(); it!=si.end(); ++it)
        {
            if(nfa[*it].f==true)
                f1=true;
        }
        dfa[p].f=f1;
        temp1=state_change(1,si);
        si=temp1;
        for (set<int>::iterator it=si.begin(); it!=si.end(); ++it)
        {
            epsilon_closure(*it,si);
        }
        if(mp.count(si)==0){
            mp[si]=ct++;
            que.push(si);
            dfa[p].a[0]=ct-1;
        }
        else{
            dfa[p].a[0]=mp.find(si)->second;
        }
        temp1.clear();

        si=que.front();
        temp2=state_change(2,si);
        si=temp2;
        for (set<int>::iterator it=si.begin(); it!=si.end(); ++it)
        {
            epsilon_closure(*it,si);
        }
        if(mp.count(si)==0)
        {
            mp[si]=ct++;
            que.push(si);
            dfa[p].a[1]=ct-1;
        }
        else
        {
            dfa[p].a[1]=mp.find(si)->second;
        }
        temp2.clear();
        que.pop();
        p++;
    }

}


//dfa 2 min
pair<int,vector<tuple<int,int,bool> > > re2dfa::minimize_dfa(vector<dst> dfa)
{

    vector<int> grp(dfa.size());
    vector<vector<int> > part(2, vector<int>());

    /// Initializing the groups
    part[0].push_back(0);
    for(int i=1; i<(int)grp.size(); i++)
    {
        if(dfa[i].f==dfa[0].f)
        {
            grp[i]=0;
            part[0].push_back(i);
        } else
        {
            grp[i]=1;
            part[1].push_back(i);
        }
    }

    if(!part[1].size()) part.erase(part.end());

    bool chk=true;
    int strt = 0;
    while(chk)
    {
        chk=false;
        for(int i=0; i<part.size(); i++)
        {
            for(int j=0; j<2; j++)
            {
                vector<pair<int,int> > trans(part[i].size());
                for(int k=0; k<part[i].size(); k++)
                {
                    if(dfa[part[i][k]].a[j] >= 0)
                        trans[k] = make_pair(grp[dfa[part[i][k]].a[j]],part[i][k]);
                    else
                        trans[k] = make_pair(-1,part[i][k]);
                }
                sort(trans.begin(), trans.end());

                if(trans[0].first!=trans[trans.size()-1].first)
                {
                    chk=true;

                    int k, m = part.size()-1;

                    part[i].clear();
                    part[i].push_back(trans[0].second);
                    for(k=1; k<trans.size() && (trans[k].first==trans[k-1].first); k++)
                    {
                        part[i].push_back(trans[k].second);
                    }

                    while(k<trans.size())
                    {
                        if(trans[k].first!=trans[k-1].first)
                        {
                            part.push_back(vector<int> ());
                            m++;
                        }
                        grp[trans[k].second] = m;
                        part[m].push_back(trans[k].second);
                        k++;
                    }
                }
            }
        }
    }

    for(int i=0; i<part.size(); i++)
    {
        for(int j=0; j<part[i].size(); j++)
        {
            if(part[i][j]==0) strt=i;
        }
    }

    vector<tuple<int,int,bool> > ret(part.size());

    for(int i=0; i<(int)part.size(); i++)
    {

        get<0>(ret[i]) = (dfa[part[i][0]].a[0]>=0)?grp[dfa[part[i][0]].a[0]]:-1;
        get<1>(ret[i]) = (dfa[part[i][0]].a[1]>=0)?grp[dfa[part[i][0]].a[1]]:-1;
        get<2>(ret[i]) = dfa[part[i][0]].f;
    }

    return make_pair(strt, ret);
}

void re2dfa::print()
{
    pair<int,vector<tuple<int,int,bool> > > min_dfa_tmp = minimize_dfa(this->dfa);
    vector<tuple<int,int,bool> >  min_dfa= min_dfa_tmp.second;
    int start_st = min_dfa_tmp.first;
    ss3<<"---------------------------------------------------------"<<endl;
    ss3<<"State\t|\tA\t|\tB\t|\tStart\t|\tFinal\t|"<<endl;
    ss3<<"---------------------------------------------------------"<<endl;
    for(int i=0; i<(int)min_dfa.size(); i++)
    {
        ss3<<i<<"\t|\t";
        if(get<0>(min_dfa[i])>-1)
        {
            ss3<<get<0>(min_dfa[i])<<"\t|\t";
        }
        else
        {
            ss3<<" "<<"\t|\t";
        }
        if(get<1>(min_dfa[i])>-1)
        {
            ss3<<get<1>(min_dfa[i])<<"\t|\t";
        }
        else
        {
            ss3<<" "<<"\t|\t";
        }
        if(i==start_st) ss3<<"Yes\t|\t"; else ss3<<"No\t|\t";
        if(get<2>(min_dfa[i])) ss3<<"Yes\t|"; else ss3<<"No\t|";
        ss3<<endl;
    }
    ss3<<"---------------------------------------------------------"<<endl;
}
