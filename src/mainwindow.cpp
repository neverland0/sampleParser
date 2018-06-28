#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myparserplus.h"
#include "re2dfa.h"

#include <iostream>
#include <cctype>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QTextBlock>
#include <sstream>
#include <stddef.h>
#include<iomanip>

using namespace std ;

KeyWord::KeyWord()
{
    //keyword
    KEY s1={"program",1};
    KEY s2={"var",2};
    KEY s3={"integer",3};
    KEY s4={"bool",4};
    KEY s5={"real",5};
    KEY s6={"char",6};
    KEY s7={"const",7};
    KEY s8={"begin",8};
    KEY s9={"if",9};
    KEY s10={"then",10};
    KEY s11={"else",11};
    KEY s12={"while",12};
    KEY s13={"do",13};
    KEY s14={"for",14};
    KEY s15={"to",15};
    KEY s16={"end",16};
    KEY s17={"read",17};
    KEY s18={"write",18};
    KEY s19={"true",19};
    KEY s20={"false",20};
    //operator
    KEY s21={"not",21};
    KEY s22={"and",22};
    KEY s23={"or",23};
    KEY s24={"+",24};
    KEY s25={"-",25};
    KEY s26={"*",26};
    KEY s27={"/",27};
    KEY s28={"<",28};
    KEY s29={">",29};
    KEY s30={"<=",30};
    KEY s31={">=",31};
    KEY s32={"==",32};
    KEY s33={"<>",33};

    //jiefu
    KEY s39={":=",39};
    KEY s40={";",40};
    KEY s41={",",41};
    KEY s42={"'",42};
    KEY s43={"/*",43};
    KEY s44={"*/",44};
    KEY s45={":",45};
    KEY s46={"(",46};
    KEY s47={")",47};
    KEY s48={".",48};
    KEY s49={"repeat",49};
    KEY s50={"until",50};



    keyList.push_back(s1);
    keyList.push_back(s2);
    keyList.push_back(s3);
    keyList.push_back(s4);
    keyList.push_back(s5);
    keyList.push_back(s6);
    keyList.push_back(s7);
    keyList.push_back(s8);
    keyList.push_back(s9);
    keyList.push_back(s10);
    keyList.push_back(s11);
    keyList.push_back(s12);
    keyList.push_back(s13);
    keyList.push_back(s14);
    keyList.push_back(s15);
    keyList.push_back(s16);
    keyList.push_back(s17);
    keyList.push_back(s18);
    keyList.push_back(s19);
    keyList.push_back(s20);
    keyList.push_back(s20);
    keyList.push_back(s21);
    keyList.push_back(s22);
    keyList.push_back(s23);
    keyList.push_back(s24);
    keyList.push_back(s25);
    keyList.push_back(s26);
    keyList.push_back(s27);
    keyList.push_back(s28);
    keyList.push_back(s29);
    keyList.push_back(s30);
    keyList.push_back(s31);
    keyList.push_back(s32);
    keyList.push_back(s33);


    keyList.push_back(s39);
    keyList.push_back(s40);
    keyList.push_back(s41);
    keyList.push_back(s42);
    keyList.push_back(s43);
    keyList.push_back(s44);
    keyList.push_back(s45);
    keyList.push_back(s46);
    keyList.push_back(s47);
    keyList.push_back(s48);
    keyList.push_back(s49);
    keyList.push_back(s50);

}

int KeyWord::getNo(string token)
{
    for(auto a : keyList)
    {
        if(a.str==token)
        {
            return a.no;
        }
    }
    if(isdigit(token[0]) )
    {
        return 36;
    }
    return -1;
}

bool KeyWord::isKeyWord(string token)
{
    for(auto a:this->keyList)
    {
        if(a.str==token)
        {
            return true;
        }
    }
    return false;
}

myScanner::myScanner()
{


}

void myScanner::getSource(string source)
{
    this->strSource=source;
}

void myScanner::split()
{
    KeyWord *k=new KeyWord();
    int nu=1;
    int state=0;
    string tem="";
    for(auto i=strSource.begin();i!=strSource.end();i++)
    {
        switch(state)
        {
        case 0:
            if((*i)=='\'')
            {
                state=6;
                tem='\'';
                TOKEN temTOKEN={nu,tem,k->getNo(tem)};
                tokenList.push_back(temTOKEN);
                tem="";
            }
            else if( (*i) == '/' )
            {
                state=1;
            }
            else if( isalpha(*i) || ((*i)=='_') )
            {
                tem+=*i;
                state=2;
            }
            else if(isdigit(*i))
            {
                tem+=*i;
                state=3;
            }
            else if((*i)==' ' || (*i) == '\t')
            {
                state=0;
            }
            else if((*i) == '\n')
            {
                nu++;
                state=0;
            }
            else
            {
                tem+=*i;
                if((*i)=='<')
                {
                    //tem+=*i;
                    state=11;
                }
                else if( (*i)=='>' || (*i)=='=' || (*i)==':' )
                {
                    //tem+=*i;
                    state=12;
                }
                else if(k->isKeyWord(tem))
                {
                    //tem+=*i;
                    TOKEN temTOKEN={nu,tem,k->getNo(tem)};
                    tokenList.push_back(temTOKEN);
                    tem="";
                    state=0;
                }
                else
                {
                    //tem+=*i;
                    ERROR temERROR={nu,tem};
                    errorList.push_back(temERROR);
                    tem="";
                    state=0;
                }
            }
            break;
        case 1:
            if((*i)=='*')
            {
                state=8;
            }
            else
            {
                state=0;
                tem += '/';
                TOKEN temTOKEN={nu,tem,k->getNo(tem)};
                tokenList.push_back(temTOKEN);
                tem="";
                i--;
            }
            break;
        case 2:
            if( isalpha(*i) || isdigit(*i) || (*i)=='_' )
            {
                state=2;
                tem+=*i;
            }
            else
            {
                if(k->isKeyWord(tem))
                {
                    TOKEN temTOKEN={nu,tem,k->getNo(tem)};
                    tokenList.push_back(temTOKEN);
                }
                else
                {
                    TOKEN temTOKEN={nu,tem,34};
                    if(!isInSymbol(tem))
                    {
                        SYMBOL temSYMBOL={tem,tem.length(),34};
                        symbolTable.push_back(temSYMBOL);
                    }
                    tokenList.push_back(temTOKEN);
                }

                tem="";
                i--;
                state=0;
            }
            break;
        case 3:
            if(isdigit(*i))
            {
                state=3;
                tem+=*i;
            }
            else if((*i)=='.')
            {
                state=4;
                tem+=*i;
            }
            else
            {
                state=0;
                TOKEN temTOKEN={nu,tem,35};
                if(!isInSymbol(tem))
                {
                    SYMBOL temSYMBOL={tem,tem.length(),36};
                    symbolTable.push_back(temSYMBOL);
                }

                tokenList.push_back(temTOKEN);
                tem="";
                i--;
            }
            break;
        case 4:
            if(isdigit(*i))
            {
                state=5;
                tem+=*i;
            }
            else
            {
                state=0;
                tem+='0';
                TOKEN temTOKEN={nu,tem,36};
                if(!isInSymbol(tem))
                {
                    SYMBOL temSYMBOL={tem,tem.length(),36};
                    symbolTable.push_back(temSYMBOL);
                }

                tokenList.push_back(temTOKEN);
                tem="";
                i--;
            }
            break;
        case 5:
            if(isdigit(*i))
            {
                state=5;
                tem+=*i;
            }
            else
            {
                state=0;
                TOKEN temTOKEN={nu,tem,36};
                if(!isInSymbol(tem))
                {
                    SYMBOL temSYMBOL={tem,tem.length(),36};
                    symbolTable.push_back(temSYMBOL);
                }

                tokenList.push_back(temTOKEN);
                tem="";
                i--;
            }
            break;
        case 6:
            if((*i)=='\'')
            {
                TOKEN temTOKEN={nu,tem,37};
                tokenList.push_back(temTOKEN);
                tem='\'';
                temTOKEN={nu,tem,k->getNo(tem)};
                tokenList.push_back(temTOKEN);
                tem="";
                state=0;
            }
            else
            {
                tem=*i;
                state=6;
            }
            break;
        case 7:

            break;
        case 8:
            if((*i)=='*')
            {
                state=9;
            }
            else
            {
                state=8;
            }
            break;
        case 9:
            if((*i)=='/')
            {
                state=0;
            }
            break;
        case 10:

            break;
        case 11:
            if((*i)=='=')
            {
                tem+=*i;
                TOKEN temTOKEN={nu,tem,k->getNo(tem)};
                tokenList.push_back(temTOKEN);
                tem="";
                state=0;
            }
            else if((*i)=='>')
            {
                tem+=*i;
                TOKEN temTOKEN={nu,tem,k->getNo(tem)};
                tokenList.push_back(temTOKEN);
                tem="";
                state=0;
            }
            else
            {
                TOKEN temTOKEN={nu,tem,k->getNo(tem)};
                tokenList.push_back(temTOKEN);
                tem="";
                i--;
                state=0;
            }
            break;
        case 12:
            if((*i)=='=')
            {
                tem+=*i;
                TOKEN temTOKEN={nu,tem,k->getNo(tem)};
                tokenList.push_back(temTOKEN);
                tem="";
                state=0;
            }
            else
            {
                TOKEN temTOKEN={nu,tem,k->getNo(tem)};
                tokenList.push_back(temTOKEN);
                tem="";
                i--;
                state=0;
            }
            break;
        }
    }
    if(tem!="")
    {
        if(k->isKeyWord(tem)||isdigit(tem[0]))
        {
            TOKEN temTOKEN={nu,tem,k->getNo(tem)};
            tokenList.push_back(temTOKEN);
        }
        else
        {
            TOKEN temTOKEN={nu,tem,34};
            if(!isInSymbol(tem))
            {
                SYMBOL temSYMBOL={tem,tem.length(),34};
                symbolTable.push_back(temSYMBOL);
            }
            tokenList.push_back(temTOKEN);
        }
    }
    //tab1->setPlainText(QString::fromStdString(std::to_string(count)));
}



vector<TOKEN> myScanner::paintToken()
{

    return tokenList;
}

vector<ERROR> myScanner::paintError()
{
    return errorList;
}

vector<SYMBOL> myScanner::paintSymbol()
{
    return symbolTable;
}

bool myScanner::isInSymbol(string str)
{
    for(auto a:this->symbolTable)
    {
        if(a.str==str)
        {
            return true;
        }
    }
    return false;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tab1=new CodeEditor();
    ui->tabWidget->insertTab(0,tab1,"new");
    ui->tabWidget->setCurrentIndex(0);
    connect(ui->actionopen, SIGNAL(triggered()), tab1, SLOT(on_actionopen_triggered()));
    connect(ui->actionsave, SIGNAL(triggered()), tab1, SLOT(on_actionsave_triggered()));
    connect(ui->actionone_step, SIGNAL(triggered()), this, SLOT(on_actionone_step_triggered()));
    connect(ui->actionsyntax_directed, SIGNAL(triggered()), this, SLOT(on_actionsyntax_directed_triggered()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(on_pushButton_click()));
    this->setGeometry(300,100,1000,700);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_click()//re 2 nfa
{
    string regex=ui->lineEdit->text().toStdString();
    re2dfa *r=new re2dfa(regex);
    r->display_nfa();
    ui->textEdit_7->setPlainText(QString::fromStdString(r->ss.str()));

    r->print_dfa();
    ui->textEdit_8->setPlainText(QString::fromStdString(r->ss2.str()));

    r->print();
    ui->textEdit_9->setPlainText(QString::fromStdString(r->ss3.str()));
}

void MainWindow::on_actionone_step_triggered()
{
    myScanner *s=new myScanner();
    s->getSource(this->outSource());
    s->split();
    this->outToken(s->paintToken());
    this->outError(s->paintError());
    this->outSymbol(s->paintSymbol());
}

void MainWindow::on_actiontoken_triggered()
{
    myScanner *s=new myScanner();
    s->getSource(this->outSource());
    s->split();
    this->outToken(s->paintToken());
}

void MainWindow::on_actionerror_triggered()
{
    myScanner *s=new myScanner();
    s->getSource(this->outSource());
    s->split();
    this->outError(s->paintError());
}

void MainWindow::on_actionsymbol_triggered()
{
    myScanner *s=new myScanner();
    s->getSource(this->outSource());
    s->split();
    this->outSymbol(s->paintSymbol());
}

void MainWindow::on_actionsyntax_directed_triggered()
{
    myScanner *s=new myScanner();
    s->getSource(this->outSource());
    s->split();
    this->outToken(s->paintToken());
    this->outError(s->paintError());
    this->outSymbol(s->paintSymbol());

    myParserPlus *p=new myParserPlus(this->symbolTable,this->tokenList,this->errorList,this->ui);
    p->parser();
    p->print();
    ui->textEdit_6->setPlainText(QString::fromStdString(p->ss2.str()));
}

void MainWindow::outToken(vector<TOKEN> tokenList)
{
    std::stringstream ss;
    ss << left << std::setw(10) << "nu" << std::setw(10) << "token" << std::setw(10) << "no" << std::endl<<std::endl;
    for(auto a:tokenList)
    {
        ss << left << std::setw(10) << a.nu << std::setw(10) << a.str << std::setw(10) << a.no << std::endl;
    }

    //std::cout << ss.str(); //Works fine
    this->tokenList=tokenList;
    ui->textEdit->setPlainText(QString::fromStdString(ss.str()));
}


void MainWindow::outError(vector<ERROR> errorList)
{
    std::stringstream ss;
    ss<<errorList.size()<<" error(s)"<<endl<<endl;
    for(auto a:errorList)
    {
        ss << "line:" << a.nu <<" unresolved symbol\"" << a.strWrong<<"\"" << std::endl;
    }

    //std::cout << ss.str(); //Works fine
    this->errorList=errorList;
    ui->textEdit_2->setPlainText(QString::fromStdString(ss.str()));
}

void MainWindow::outSymbol(vector<SYMBOL> symbolTable)
{
    std::stringstream ss;
    ss << left << std::setw(10) << "symbol" << std::setw(10) << "length" << std::setw(10) << "no" << std::endl<<std::endl;
    for(auto a:symbolTable)
    {
        ss << left << std::setw(10) << a.str << std::setw(10) << a.len << std::setw(10) << a.no << std::endl;
    }

    //std::cout << ss.str();
    this->symbolTable=symbolTable;
    ui->textEdit_3->setPlainText(QString::fromStdString(ss.str()));
}

string MainWindow::outSource()
{
    return tab1->toPlainText().toStdString();
}

//test
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    //
    QFont *font=new QFont("微软雅黑",15);
    this->setFont(*font);

}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::on_actionopen_triggered()
{
    QString path = QFileDialog::getOpenFileName(
                    this,   //父窗口
                    tr("Open File"),    //对话框名字
                    ".",    //默认目录
                    tr("Text Files(*.txt);;ini File(*.ini)") //过滤器 只打开txt类型 或 ini 使用;;分隔
                    );
        if (!path.isEmpty())
        {
            QFile file(path);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return; //打开失败

            QTextStream in(&file);

            this->setPlainText(in.readAll());
            file.close();
            //ui->tabWidget->setTabText(0,path);
        }
}

void CodeEditor::on_actionsave_triggered()
{
    QString path = QFileDialog::getSaveFileName(
                    this,   //父窗口
                    tr("Open File"),    //对话框名字
                    ".",    //默认目录
                    tr("Text Files(*.txt);;ini File(*.ini)") //过滤器 只打开txt类型 或 ini 使用;;分隔
                    );
        if (!path.isEmpty())
        {
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return; //打开失败

            QTextStream out(&file);
            out << this->toPlainText();

            file.close();
        }
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
        int blockNumber = block.blockNumber();
        int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
        int bottom = top + (int) blockBoundingRect(block).height();
        while (block.isValid() && top <= event->rect().bottom())
        {
                if (block.isVisible() && bottom >= event->rect().top())
                {
                    QString number = QString::number(blockNumber + 1);
                    painter.setPen(Qt::black);
                    painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                     Qt::AlignRight, number);
                }

                block = block.next();
                top = bottom;
                bottom = top + (int) blockBoundingRect(block).height();
                ++blockNumber;
         }
}



void MainWindow::on_actionparser_triggered()
{
    myScanner *s=new myScanner();
    s->getSource(this->outSource());
    s->split();
    this->outToken(s->paintToken());
    this->outError(s->paintError());
    this->outSymbol(s->paintSymbol());

    myParser *p=new myParser(this->symbolTable,this->tokenList,this->errorList,this->ui);
    p->parser();
}
/*
void myParser::on_actionone_step_triggered()
{
    myScanner *s=new myScanner();
    s->getSource(this->outSource());
    s->split();
    this->outToken(s->paintToken());
    this->outError(s->paintError());
    this->outSymbol(s->paintSymbol());
}

void myParser::on_actiontoken_triggered()
{

}
*/
myParser::myParser(vector<SYMBOL> symbolTable, vector<TOKEN> tokenList, vector<ERROR> errorList,Ui::MainWindow *ui)
{
    this->symbolTable=symbolTable;
    this->tokenList=tokenList;
    this->errorList=errorList;
    this->ui=ui;
    this->count=0;
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
}

TOKEN myParser::getNextToken()
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

bool myParser::reachEOF()
{
    if(temTokenList.empty())
    {
        return true;
    }
    return false;
}

void myParser::Fail()
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

void myParser::parser()
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
    //ss<<std::to_string(count);
    ui->textEdit_4->setPlainText(QString::fromStdString(ssInfo.str()));
    ui->textEdit_5->setPlainText(QString::fromStdString(ss.str()));
}

bool myParser::endFlag()
{
    if(token.str==".")
    {
        //token=getNextToken();
        return true;
    }
    return false;
}

bool myParser::Goal()
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

bool myParser::isName()
{
    if(token.no==34)
    {
        return true;
    }
    return false;
}

bool myParser::semiColon()
{
    if(token.str==";")
    {
        return true;
    }
    return false;
}

bool myParser::programHead()
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

bool myParser::var_conDef()
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

bool myParser::varPart_()
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

bool myParser::varPart()
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

bool myParser::varDef()
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

bool myParser::varDef_()
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

bool myParser::isType()
{
    if(token.no>=2&&token.no<=7)
    {
        return true;
    }
    return false;
}

bool myParser::colon()
{
    if(token.str==":")
    {
        return true;
    }
    return false;
}

bool myParser::varItem()
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

bool myParser::varList()
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

bool myParser::varList_()
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

bool myParser::conPart_()
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

bool myParser::conPart()
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

bool myParser::conDef()
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

bool myParser::conDef_()
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



bool myParser::conItem()
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

bool myParser::conList()
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

bool myParser::conList_()
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

bool myParser::rChar()
{
    if(token.str=="'")
    {
        token=getNextToken();
        if(isalpha(token.str[0]))
        {
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

bool myParser::isNum()
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

bool myParser::rValue()
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

bool myParser::lValue()
{
    if(token.str=="true"||token.str=="false")
    {
        token=getNextToken();
        return true;
    }
    else if(token.str=="'")
    {
        return rChar();
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

bool myParser::eqs()
{
    if(token.str==":=")
    {
        return true;
    }
    return false;
}

bool myParser::conExpr()
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

bool myParser::programImp()
{
    if(token.str=="begin")
    {
        token=getNextToken();
        if(statementList())
        {
            if(token.str=="end")
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

bool myParser::statementList()
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

bool myParser::isState()
{
    if(token.str=="while"||token.str=="if"||token.str=="for"||token.str=="repeat"||isName())
    {
        return true;
    }
    return false;
}

bool myParser::statementList_()
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

bool myParser::pureStatement()
{
    if(token.str=="while")
    {
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
            return true;
        }
        return false;
    }
    else if(isName())
    {
        ssInfo<<"begin assign statement"<<endl;
        if(assignStatement())
        {
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
            return true;
        }
        return false;
    }
    else if(token.str=="repeat")
    {
        ssInfo<<"begin repeat statement"<<endl;
        token=getNextToken();
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

bool myParser::assignStatement()
{

    if(isName())
    {
        token=getNextToken();
        if(eqs())
        {
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

bool myParser::statement()
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

bool myParser::boolExpr()
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

bool myParser::boolExpr_()
{
    if(token.str=="and"||token.str=="or")
    {
        token=getNextToken();
        if(boolTerm())
        {
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

bool myParser::boolTerm()
{
    if(token.str=="not")
    {
        token=getNextToken();
        return boolTerm();
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
        return true;
    }
    else
    {
        //Fail();
        return false;
    }
    return false;
}

bool myParser::toDoThen()
{
    if(token.str=="to"||token.str=="do"||token.str=="then"||token.str=="until")
    {
        return true;
    }
    return false;
}

bool myParser::ops()
{
    if(token.str==">"||token.str=="<"||token.str==">="||token.str=="<="||token.str=="==")
    {
        return true;
    }
    return false;
}

bool myParser::boolFactor()
{
    if(!Expr())
    {
        //Fail();
        return false;
    }
    if(!ops())
    {
        //Fail();
        return false;
    }
    token=getNextToken();
    return Expr();
}

bool myParser::Expr()
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

bool myParser::Expr_()
{
    if(token.str=="+"||token.str=="-")
    {
        token=getNextToken();
        if(Term())
        {
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

bool myParser::Term()
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

bool myParser::Term_()
{
    if(token.str=="*"||token.str=="/")
    {
        token=getNextToken();
        if(Factor())
        {
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

bool myParser::Factor()
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
