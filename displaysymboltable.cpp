#include "displaysymboltable.h"
#include "ui_displaysymboltable.h"


DisplaySymbolTable::DisplaySymbolTable(QWidget *parent):
		QMainWindow(parent),
		ui(new Ui::DisplaySymbolTable)
{
	ui->setupUi(this);

	QObject::connect(ui->ConfirmButton,&QPushButton::clicked,this,&DisplaySymbolTable::ClickConfirm);
	QObject::connect(ui->CancelButton,&QPushButton::clicked,this,&DisplaySymbolTable::ClickConfirm);

}

DisplaySymbolTable::~DisplaySymbolTable()
{
	delete ui;
}
void DisplaySymbolTable::show()
{
	ui->SymbolTableListText->clear();
	for(long long i=0;i<tokenList.getUsage();i++)
	{
		ui->SymbolTableListText->appendPlainText(QString::fromStdString(tokenList[i].getTokenName())+
												 "              /           "+
												 QString::fromStdString(tokenList[i].getTokenType()));
	}
	QWidget::show();
}
void DisplaySymbolTable::ClickConfirm()
{
	this->close();
}
void DisplaySymbolTable::ClickCancel()
{
	this->close();
}
void DisplaySymbolTable::setTokenList(const Sequence<TokenInf> &tokenList)
{
	DisplaySymbolTable::tokenList=tokenList;
}
