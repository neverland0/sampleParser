#ifndef MYPARSERPLUS_H
#define MYPARSERPLUS_H

#include "mainwindow.h"

typedef struct midNode
{
    int nu;
    string op;
    string arg1;
    string arg2;
    string result;
}MIDNODE;

class myParserPlus
{
public:
    myParserPlus(vector<SYMBOL> symbolTable,
             vector<TOKEN> tokenList,
             vector<ERROR> errorList,
             Ui::MainWindow *ui);
    void parser();
    std::stringstream ssInfo;
    std::stringstream ss;
    std::stringstream ss2;
    void /*myParserPlus::*/print();
private:
    vector<MIDNODE> midCode;
    vector<SYMBOL> symbolTable;
    vector<TOKEN> tokenList;
    vector<ERROR> errorList;
    stack<TOKEN> temTokenList;
    TOKEN getNextToken();
    TOKEN token;
    int count;
    Ui::MainWindow *ui;
    int temVarNo;
    int midNu;
    stack<string> temStrStack;
    stack<pair<int,int> > temNuStack;
    int head;
    string /*myParserPlus::*/getTemVar();
    int /*myParserPlus::*/getMidNu();
    void /*myParserPlus::*/addMidCode(string op,string arg1,string arg2,string result);
    int /*myParserPlus::*/getCurNu();
    int /*myParserPlus::*/getMax(int a,int b);
    int /*myParserPlus::*/getMin(int a,int b);

    bool /*myParserPlus::*/reachEOF();
    void /*myParserPlus::*/Fail();
    bool /*myParserPlus::*/endFlag();
    bool /*myParserPlus::*/Goal();
    bool /*myParserPlus::*/isName();
    bool /*myParserPlus::*/semiColon();
    bool /*myParserPlus::*/programHead();
    bool /*myParserPlus::*/var_conDef();
    bool /*myParserPlus::*/varPart_();
    bool /*myParserPlus::*/varPart();
    bool /*myParserPlus::*/varDef();
    bool /*myParserPlus::*/varDef_();
    bool /*myParserPlus::*/isType();
    bool /*myParserPlus::*/colon();
    bool /*myParserPlus::*/varItem();
    bool /*myParserPlus::*/varList();
    bool /*myParserPlus::*/varList_();
    bool /*myParserPlus::*/conPart_();
    bool /*myParserPlus::*/conPart();
    bool /*myParserPlus::*/conDef();
    bool /*myParserPlus::*/conDef_();
    bool /*myParserPlus::*/conItem();
    bool /*myParserPlus::*/conList();
    bool /*myParserPlus::*/conList_();
    bool /*myParserPlus::*/rChar();
    bool /*myParserPlus::*/rChar2();
    bool /*myParserPlus::*/isNum();
    bool /*myParserPlus::*/rValue();
    bool /*myParserPlus::*/lValue();
    bool /*myParserPlus::*/eqs();
    bool /*myParserPlus::*/conExpr();
    bool /*myParserPlus::*/programImp();
    bool /*myParserPlus::*/statementList();
    bool /*myParserPlus::*/isState();
    bool /*myParserPlus::*/statementList_();
    bool /*myParserPlus::*/pureStatement();
    bool /*myParserPlus::*/assignStatement();
    bool /*myParserPlus::*/statement();
    bool /*myParserPlus::*/boolExpr();
    bool /*myParserPlus::*/boolExpr_();
    bool /*myParserPlus::*/boolTerm();
    bool /*myParserPlus::*/ops();
    bool /*myParserPlus::*/ops2();
    bool /*myParserPlus::*/toDoThen();
    bool /*myParserPlus::*/boolFactor();
    bool /*myParserPlus::*/Expr();
    bool /*myParserPlus::*/Expr_();
    bool /*myParserPlus::*/Term();
    bool /*myParserPlus::*/Term_();
    bool /*myParserPlus::*/Factor();
};

#endif // MYPARSERPLUS_H
