#include "myparserplus.h"

myParserPlus::myParserPlus(vector<SYMBOL> symbolTable, vector<TOKEN> tokenList, vector<ERROR> errorList,Ui::MainWindow *ui)
{
    this->symbolTable=symbolTable;
    this->tokenList=tokenList;
    this->errorList=errorList;
    this->ui=ui;
    this->count=0;
    this->temVarNo=0;
    this->midNu=0;
    this->head=1;
    stack<TOKEN> tem;
    for(auto a:tokenList)
    {
        tem.push(a);
    }
    while(!tem.empty())
    {
        temTokenList.push(tem.top());
        tem.pop();
    }
    MIDNODE m={0,"_","_","_","_"};
    this->midCode.push_back(m);
}

string myParserPlus::getTemVar()
{
    this->temVarNo++;
    return "T"+std::to_string(this->temVarNo);
}

int myParserPlus::getMidNu()
{
    this->midNu++;
    return this->midNu;
}

int myParserPlus::getCurNu()
{
    return this->midNu;
}

int myParserPlus::getMax(int a,int b)
{
    return a>b?a:b;
}

int myParserPlus::getMin(int a,int b)
{
    return a<b?a:b;
}

void myParserPlus::addMidCode(string op,string arg1,string arg2,string result)
{
    MIDNODE m={getMidNu(),op,arg1,arg2,result};
    this->midCode.push_back(m);
}

void myParserPlus::print()
{
    for(auto a=this->midCode.begin()+1;a!=this->midCode.end();a++)
    {
        ss2<<(*a).nu<<"\t"<<(*a).op<<"\t"<<(*a).arg1<<"\t"<<(*a).arg2<<"\t"<<(*a).result<<endl;
    }
}

TOKEN myParserPlus::getNextToken()
{
    TOKEN token;
    if(!temTokenList.empty())
    {
        token=temTokenList.top();
        temTokenList.pop();
        this->count++;
    }
    else
    {
        token={-1,"EOF",-1};
    }
    return token;
}

bool myParserPlus::reachEOF()
{
    if(temTokenList.empty())
    {
        return true;
    }
    return false;
}

void myParserPlus::Fail()
{
    if(token.str!="EOF")
    {
        ss<<"line:"<<token.nu<<" syntax error near \""<<token.str<<"\""<<endl;
    }
    else
    {
        ss<<" syntax error near "<<token.str<<endl;
    }

    //return;
    //exit(0);
}

void myParserPlus::parser()
{
    token=getNextToken();
    if(Goal())
    {
        if(reachEOF())
        {
            ss<<"parsering completed";
        }
        else
        {
            Fail();

        }
    }
    else
    {
        Fail();

        ss<<"parsering uncompleted";
    }


}

bool myParserPlus::endFlag()
{
    if(token.str==".")
    {
        //token=getNextToken();
        return true;
    }
    return false;
}

bool myParserPlus::Goal()
{
    ssInfo<<"begin programHead"<<endl;
    if(!programHead())
    {
        //Fail();
        return false;

    }
    ssInfo<<"end programHead"<<endl;
    ssInfo<<endl;

    ssInfo<<"begin var and const define"<<endl;
    if(!var_conDef())
    {
        //Fail();
        return false;
    }
    ssInfo<<"end var and const define"<<endl;
    ssInfo<<endl;
    if(programImp())
    {
        return endFlag();
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::isName()
{
    if(token.no==34)
    {
        return true;
    }
    return false;
}

bool myParserPlus::semiColon()
{
    if(token.str==";")
    {
        return true;
    }
    return false;
}

bool myParserPlus::programHead()
{
    if(token.str=="program")
    {
        token=getNextToken();
        if(isName())
        {
            token=getNextToken();
            if(semiColon())
            {
                token=getNextToken();
                return true;
            }
            else
            {
                //Fail();
        return false;
            }
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::var_conDef()
{
    if(token.str=="var")
    {
        if(varPart())
        {
            return varPart_();
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else if(token.str=="const")
    {
        if(conPart())
        {
            return conPart_();
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::varPart_()
{
    if(token.str=="const")
    {
        return conPart();
    }
    else if(token.str=="begin")
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

bool myParserPlus::varPart()
{
    if(token.str=="var")
    {
        token=getNextToken();
        return varDef();
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::varDef()
{
    if(varItem())
    {
        return varDef_();
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::varDef_()
{
    if(isName())
    {
        if(varItem())
        {
            return varDef_();
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else if(token.str=="const"||token.str=="begin")
    {
        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::isType()
{
    if(token.no>=2&&token.no<=7)
    {
        return true;
    }
    return false;
}

bool myParserPlus::colon()
{
    if(token.str==":")
    {
        return true;
    }
    return false;
}

bool myParserPlus::varItem()
{
    if(!varList())
    {
        //Fail();
        return false;
    }
    if(colon())
    {
        token=getNextToken();
        if(isType())
        {
            token=getNextToken();
            if(semiColon())
            {
                token=getNextToken();
                return true;
            }
            else
            {
                //Fail();
        return false;
            }
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::varList()
{
    if(isName())
    {
        token=getNextToken();
        return varList_();
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::varList_()
{
    if(token.str==",")
    {
        token=getNextToken();
        if(isName())
        {
            token=getNextToken();
            return varList_();
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else if(token.str==":")
    {
        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::conPart_()
{
    if(token.str=="var")
    {
        return varPart();
    }
    else if(token.str=="begin")
    {
        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::conPart()
{
    if(token.str=="const")
    {
        token=getNextToken();
        return conDef();
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::conDef()
{
    if(conItem())
    {
        return conDef_();
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::conDef_()
{
    if(isName())
    {

        if(conItem())
        {
            return conDef_();
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else if(token.str=="var"||token.str=="begin")
    {
        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}



bool myParserPlus::conItem()
{
    if(!conList())
    {
        //Fail();
        return false;
    }
    if(colon())
    {
        token=getNextToken();
        if(isType())
        {
            token=getNextToken();
            if(semiColon())
            {
                token=getNextToken();
                return true;
            }
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::conList()
{
    if(conExpr())
    {
        return conList_();
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::conList_()
{
    if(token.str==",")
    {
        token=getNextToken();
        if(conExpr())
        {
            return conList_();
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else if(colon())
    {
        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::rChar2()
{
    if(token.str=="'")
    {
        token=getNextToken();
        if(isalpha(token.str[0])||isdigit(token.str[0]))
        {
            temStrStack.push("\'"+token.str+"\'");
            token=getNextToken();
            if(token.str=="'")
            {
                token=getNextToken();
                return true;
            }
            else
            {
                //Fail();
        return false;
            }
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::rChar()
{
    if(token.str=="'")
    {
        token=getNextToken();
        if(isalpha(token.str[0])||isdigit(token.str[0]))
        {
            //temStrStack.push(token.str);
            token=getNextToken();
            if(token.str=="'")
            {
                token=getNextToken();
                return true;
            }
            else
            {
                //Fail();
        return false;
            }
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::isNum()
{
    string tem=token.str;
    for(auto a:tem)
    {
        if(!isdigit(a))
        {
            return false;
        }
    }
    return true;
}

bool myParserPlus::rValue()
{
    if(isNum()||token.str=="true"||token.str=="false")
    {
        token=getNextToken();
        return true;
    }
    else if(token.str=="'")
    {
        return rChar();
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::lValue()
{
    if(token.str=="true"||token.str=="false")
    {
        temStrStack.push(token.str);
        token=getNextToken();
        return true;
    }
    else if(token.str=="'")
    {
        return rChar2();
    }
    else if(isName()||isNum())
    {
        return Expr();
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::eqs()
{
    if(token.str==":=")
    {
        return true;
    }
    return false;
}

bool myParserPlus::conExpr()
{
    if(isName())
    {
        token=getNextToken();
        if(eqs())
        {
            token=getNextToken();
            return rValue();
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::programImp()
{
    if(token.str=="begin")
    {
        token=getNextToken();
        if(statementList())
        {
            if(token.str=="end")
            {
                token=getNextToken();
                addMidCode("_","_","_","_");
                return true;
            }
            else
            {
                //Fail();
        return false;
            }
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::statementList()
{
    if(statement())
    {
        return statementList_();
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::isState()
{
    if(token.str=="while"||token.str=="if"||token.str=="for"||token.str=="repeat"||isName())
    {
        return true;
    }
    return false;
}

bool myParserPlus::statementList_()
{
    if(isState())
    {
        if(statement())
        {
            return statementList_();
        }
    }
    else if(token.str=="end")
    {
        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::pureStatement()
{
    if(token.str=="while")
    {
        int whileHead=getCurNu()+1;
        ssInfo<<"begin while statement"<<endl;
        token=getNextToken();
        if(!boolExpr())
        {
            //Fail();
        return false;
        }

        if(token.str!="do")
        {
            //Fail();
            return false;
        }
        ssInfo<<"do"<<endl;
        token=getNextToken();
        if(pureStatement())
        {
            ssInfo<<"end while statement"<<endl;
            addMidCode("J","_","_",std::to_string(whileHead));
            pair<int,int> b1=temNuStack.top();
            temNuStack.pop();
            midCode[b1.second].result=std::to_string(getCurNu()+1);

            return true;
        }
        return false;
    }
    else if(isName())
    {
        ssInfo<<"begin assign statement"<<endl;
        if(assignStatement())
        {
            string c=temStrStack.top();
            temStrStack.pop();
            string b=temStrStack.top();
            temStrStack.pop();
            string a=temStrStack.top();
            temStrStack.pop();
            addMidCode(b,c,"_",a);
            temStrStack.push(a);
            ssInfo<<"end assign statement"<<endl;
            return true;
        }
        //Fail();
        return false;
    }
    else if(token.str=="if")
    {
        ssInfo<<"begin if statement"<<endl;
        token=getNextToken();
        if(!boolExpr())
        {
            //Fail();
        return false;
        }
        if(token.str!="then")
        {

            //Fail();
        return false;
        }
        ssInfo<<"then"<<endl;
        ssInfo<<"-";
        token=getNextToken();
        if(pureStatement())
        {
            ssInfo<<"end if statement"<<endl;
            pair<int,int> b1=temNuStack.top();
            temNuStack.pop();
            midCode[b1.second].result=std::to_string(getCurNu()+1);
            return true;
        }
        return false;
    }
    else if(token.str=="for")
    {
        ssInfo<<"begin for statement"<<endl;
        token=getNextToken();

        if(!assignStatement())
        {
            //Fail();
        return false;
        }

        if(token.str!="to")
        {
            //Fail();
        return false;
        }
        ssInfo<<"to"<<endl;
        token=getNextToken();
        if(!lValue())
        {
            //Fail();
        return false;
        }

        string b=temStrStack.top();
        temStrStack.pop();
        string a=temStrStack.top();
        temStrStack.pop();
        addMidCode("J=",a,b,std::to_string(0));//fake exit
        addMidCode("J","_","_",std::to_string(getCurNu()+3));
        int forHead=getCurNu()+1;
        addMidCode("+",a,std::to_string(1),a);

        temNuStack.push(make_pair(getCurNu()-1,getCurNu()-2));
        if(token.str!="do")
        {
            //Fail();
        return false;
        }
        ssInfo<<"do"<<endl;
        token=getNextToken();
        if(pureStatement())
        {
            ssInfo<<"end for statement"<<endl;
            addMidCode("J","_","_",std::to_string(forHead));
            pair<int,int> b1=temNuStack.top();
            temNuStack.pop();
            midCode[b1.second].result=std::to_string(getCurNu()+1);
            return true;
        }
        return false;
    }
    else if(token.str=="repeat")
    {
        ssInfo<<"begin repeat statement"<<endl;
        token=getNextToken();
        int repeatHead=getCurNu()+1;
        if(!pureStatement())
        {
            //Fail();
        return false;
        }
        if(token.str!="until")
        {
            //Fail();
        return false;
        }
        ssInfo<<"until"<<endl;
        token=getNextToken();
        if(boolExpr())
        {
            ssInfo<<"end repeat statement"<<endl;
            pair<int,int> b1=temNuStack.top();
            temNuStack.pop();
            midCode[b1.second].result=std::to_string(repeatHead);
            return true;
        }
        return false;
    }
    else if(token.str==";")
    {
        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::assignStatement()
{

    if(isName())
    {
        temStrStack.push(token.str);
        token=getNextToken();
        if(eqs())
        {
            temStrStack.push(token.str);
            token=getNextToken();
            if(lValue())
            {

                return true;
            }

        }
        else
        {
            //Fail();
        return false;
        }
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::statement()
{
    if(pureStatement())
    {
        if(semiColon())
        {
            token=getNextToken();
            return true;
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::boolExpr()
{
    ssInfo<<"--";
    ssInfo<<"begin bool express"<<endl;
    if(boolTerm())
    {
        if(boolExpr_())
        {
            ssInfo<<"--";
            ssInfo<<"end bool express"<<endl;
            return true;
        }
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::boolExpr_()
{
    if(token.str=="and"||token.str=="or")
    {
        string temstr=token.str;
        token=getNextToken();
        if(boolTerm())
        {
            if(temstr=="and")
            {
                pair<int,int> b2=temNuStack.top();
                temNuStack.pop();
                pair<int,int> b1=temNuStack.top();
                temNuStack.pop();
                midCode[b1.first].result=std::to_string(b2.first);
                midCode[b2.second].result=std::to_string(b1.second);
                temNuStack.push(make_pair(b2.first,b1.second));
            }
            else//temstr=="or"
            {
                pair<int,int> b2=temNuStack.top();
                temNuStack.pop();
                pair<int,int> b1=temNuStack.top();
                temNuStack.pop();
                midCode[b1.first].result=std::to_string(getMax(b2.first,b2.second)+1);
                midCode[b2.second].result=std::to_string(b2.first);

                temNuStack.push(make_pair(b1.first,b2.second));
            }
            return boolExpr_();
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else if(token.str=="do"||token.str=="then"||token.str==";"||token.str==")")
    {
        return true;
    }
    return false;
}

bool myParserPlus::boolTerm()
{
    if(token.str=="not")
    {
        token=getNextToken();
        if(boolTerm())
        {
            string c=temStrStack.top();
            temStrStack.pop();
            string b=temStrStack.top();
            temStrStack.pop();
            string a=temStrStack.top();
            temStrStack.pop();

            addMidCode("J"+b,a,c,std::to_string(0));
            addMidCode("J","_","_",std::to_string(getCurNu()+2));
            temNuStack.push(make_pair(getCurNu(),getCurNu()-1));

            return true;
        }
        else
        {
            return false;
        }
    }
    else if(token.str=="(")
    {
        token=getNextToken();
        if(!boolFactor())
        {
            //Fail();
        return false;
        }
        if(token.str!=")")
        {
            //Fail();
        return false;
        }
        token=getNextToken();

        string c=temStrStack.top();
        temStrStack.pop();
        string b=temStrStack.top();
        temStrStack.pop();
        string a=temStrStack.top();
        temStrStack.pop();

        addMidCode("J"+b,a,c,std::to_string(getCurNu()+3));
        addMidCode("J","_","_",std::to_string(0));
        temNuStack.push(make_pair(getCurNu()-1,getCurNu()));


        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::toDoThen()
{
    if(token.str=="to"||token.str=="do"||token.str=="then"||token.str=="until")
    {
        return true;
    }
    return false;
}

bool myParserPlus::ops()
{
    if(token.str==">"||token.str=="<"||token.str==">="||token.str=="<="||token.str=="==")
    {
        return true;
    }
    return false;
}

bool myParserPlus::ops2()
{
    if(token.str==">"||token.str=="<"||token.str==">="||token.str=="<="||token.str=="==")
    {
        temStrStack.push(token.str);
        return true;
    }
    return false;
}

bool myParserPlus::boolFactor()
{
    if(!Expr())
    {
        //Fail();
        return false;
    }
    if(!ops2())
    {
        //Fail();
        return false;
    }
    token=getNextToken();
    if(Expr())
    {

        return true;
    }
    else
    {
        return false;
    }
    return false;
}

bool myParserPlus::Expr()
{
    ssInfo<<"--";
    ssInfo<<"begin express"<<endl;
    if(Term())
    {
        if(Expr_())
        {
            ssInfo<<"--";
            ssInfo<<"end express"<<endl;
            return true;
        }

    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::Expr_()
{
    if(token.str=="+"||token.str=="-")
    {
        temStrStack.push(token.str);
        token=getNextToken();
        if(Term())
        {
            string t=getTemVar();
            string c=temStrStack.top();
            temStrStack.pop();
            string b=temStrStack.top();
            temStrStack.pop();
            string a=temStrStack.top();
            temStrStack.pop();
            addMidCode(b,a,c,t);
            temStrStack.push(t);
            return Expr_();
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else if(toDoThen()||semiColon()||ops()||token.str==")")
    {
        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::Term()
{
    if(Factor())
    {
        return Term_();
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::Term_()
{
    if(token.str=="*"||token.str=="/")
    {
        temStrStack.push(token.str);
        token=getNextToken();
        if(Factor())
        {
            string t=getTemVar();
            string c=temStrStack.top();
            temStrStack.pop();
            string b=temStrStack.top();
            temStrStack.pop();
            string a=temStrStack.top();
            temStrStack.pop();
            addMidCode(b,a,c,t);
            temStrStack.push(t);
            return Term_();
        }
        else
        {
            //Fail();
        return false;
        }
    }
    else if(toDoThen()||semiColon()||ops()||token.str=="+"||token.str=="-"||token.str==")")
    {
        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParserPlus::Factor()
{
    if(token.str=="(")
    {
        token=getNextToken();
        if(!Expr())
        {
            //Fail();
        return false;
        }
        if(token.str!=")")
        {
            //Fail();
        return false;
        }
        token=getNextToken();
        return true;
    }
    else if(isNum()||isName())
    {
        temStrStack.push(token.str);
        token=getNextToken();
        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}
