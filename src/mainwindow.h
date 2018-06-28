#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <stddef.h>
#include <map>
#include<iomanip>
using namespace std;
//using namespace std::vector;
//using namespace std::string;

typedef struct keyWord
{
    string str;
    int no;
}KEY;

typedef struct token
{
    int nu;
    string str;
    int no;
}TOKEN;

typedef struct symbol
{
    string str;
    size_t len;
    int no;
    string type;
    string kind;
}SYMBOL;

typedef struct error
{
    int nu;
    string strWrong;
}ERROR;

namespace Ui {
class MainWindow;
}

class myParser
{
public:
    myParser(vector<SYMBOL> symbolTable,
             vector<TOKEN> tokenList,
             vector<ERROR> errorList,
             Ui::MainWindow *ui);
    void parser();

/*
private slots:
    void on_actionone_step_triggered();

    void on_actiontoken_triggered();
*/


private:
    vector<SYMBOL> symbolTable;
    vector<TOKEN> tokenList;
    vector<ERROR> errorList;
    stack<TOKEN> temTokenList;
    TOKEN getNextToken();
    TOKEN token;
    int count;
    Ui::MainWindow *ui;
    std::stringstream ssInfo;
    std::stringstream ss;
    bool /*myParser::*/reachEOF();
    void /*myParser::*/Fail();
    bool /*myParser::*/endFlag();
    bool /*myParser::*/Goal();
    bool /*myParser::*/isName();
    bool /*myParser::*/semiColon();
    bool /*myParser::*/programHead();
    bool /*myParser::*/var_conDef();
    bool /*myParser::*/varPart_();
    bool /*myParser::*/varPart();
    bool /*myParser::*/varDef();
    bool /*myParser::*/varDef_();
    bool /*myParser::*/isType();
    bool /*myParser::*/colon();
    bool /*myParser::*/varItem();
    bool /*myParser::*/varList();
    bool /*myParser::*/varList_();
    bool /*myParser::*/conPart_();
    bool /*myParser::*/conPart();
    bool /*myParser::*/conDef();
    bool /*myParser::*/conDef_();
    bool /*myParser::*/conItem();
    bool /*myParser::*/conList();
    bool /*myParser::*/conList_();
    bool /*myParser::*/rChar();
    bool /*myParser::*/isNum();
    bool /*myParser::*/rValue();
    bool /*myParser::*/lValue();
    bool /*myParser::*/eqs();
    bool /*myParser::*/conExpr();
    bool /*myParser::*/programImp();
    bool /*myParser::*/statementList();
    bool /*myParser::*/isState();
    bool /*myParser::*/statementList_();
    bool /*myParser::*/pureStatement();
    bool /*myParser::*/assignStatement();
    bool /*myParser::*/statement();
    bool /*myParser::*/boolExpr();
    bool /*myParser::*/boolExpr_();
    bool /*myParser::*/boolTerm();
    bool /*myParser::*/ops();
    bool /*myParser::*/toDoThen();
    bool /*myParser::*/boolFactor();
    bool /*myParser::*/Expr();
    bool /*myParser::*/Expr_();
    bool /*myParser::*/Term();
    bool /*myParser::*/Term_();
    bool /*myParser::*/Factor();
};

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void on_actionopen_triggered();
    void on_actionsave_triggered();
private:
    QWidget *lineNumberArea;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};


class KeyWord
{
private:

public:
    KeyWord();
    vector<KEY> keyList;
    bool isKeyWord(string token);
    int getNo(string token);
};

class myScanner:public QWidget
{
     Q_OBJECT

public:
    myScanner();
    void getSource(string source);
    void split();
    vector<TOKEN> paintToken();
    vector<SYMBOL> paintSymbol();
    vector<ERROR> paintError();
    bool isInSymbol(string);
    vector<SYMBOL> symbolTable;
    vector<TOKEN> tokenList;
    vector<ERROR> errorList;
private:
    string strSource;
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void outToken(vector<TOKEN>);
    void outSymbol(vector<SYMBOL>);
    void outError(vector<ERROR>);
    string outSource();
    vector<SYMBOL> symbolTable;
    vector<TOKEN> tokenList;
    vector<ERROR> errorList;
    ~MainWindow();

private slots:
    //void on_actionopen_triggered();

    //void on_actionsave_triggered();

    void on_actionone_step_triggered();

    void on_actiontoken_triggered();

    void on_actionerror_triggered();

    void on_actionsymbol_triggered();

    void on_actionparser_triggered();

    void on_actionsyntax_directed_triggered();

    void on_pushButton_click();

private:
    Ui::MainWindow *ui;
    CodeEditor *tab1;
    myParser *p1;

};



#endif // MAINWINDOW_H
