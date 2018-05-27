#include <QtWidgets/QFileDialog>
#include <QTextStream>
#include <QtWidgets/QMessageBox>
#include <c++/7.3.0/sstream>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent):
		QMainWindow(parent),
		ui(new Ui::MainWindow),tokenParser(InitTokenParser()),SyntaxParser(InitLLParser())
{
	ui->setupUi(this);
	QObject::connect(ui->openFileButton,&QPushButton::clicked,this,&MainWindow::ClickOpen);
	QObject::connect(ui->parseButton,&QPushButton::clicked,this,&MainWindow::ClickParse);
	displaySymbolTable=new DisplaySymbolTable;
	QObject::connect(ui->showSymbolTableButton,&QPushButton::clicked,this,&MainWindow::ClickShowSymbolTable);
}

MainWindow::~MainWindow()
{
	delete ui;
}
void MainWindow::ClickOpen()
{
	QFileDialog *fileDialog=new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Open File"));
	fileDialog->setDirectory(".");
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	fileDialog->setViewMode(QFileDialog::Detail);
	QString QStringFileName;
	if(fileDialog->exec())
	{
		QStringFileName=fileDialog->selectedFiles().first();
		ui->errorStmtsText->clear();
		ui->parseProcessText->clear();
		ui->inputFileText->clear();
		originalText.clear();
		SyntaxParser.getErrorLocationListR().Clear();
		tokenList.Clear();
		forSyntaxParse.clear();
	}
	ui->filePathLabel->setText(QStringFileName);
	QFile file(QStringFileName);

	if(file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		QTextStream textStream(&file);
		while(!textStream.atEnd())
		{
			ui->inputFileText->append(textStream.readLine());
		}
	}
	else
	{
		QMessageBox::information(nullptr,nullptr,"open file error");
		return;
	}
	string inputFileName=QStringFileName.toStdString();
	auto input=ReformatInput(inputFileName);
	originalText=input.first;
	forSyntaxParse=GenerateForSyntaxParser(tokenParser,input.second,tokenList);
}
void MainWindow::ClickParse()
{
	ui->errorStmtsText->clear();
	ui->parseProcessText->clear();
	auto parseResult=SyntaxParser.parse(forSyntaxParse);
	ui->parseProcessText->setText(QString::fromStdString(parseResult));

	if(SyntaxParser.getErrorLocationListR().getUsage()==0)
	{
		ui->errorStmtsText->setText("No error. Accepted.");
	}
	else
	{
		stringstream ss;
		ss<<SyntaxParser.getErrorLocationListR().getUsage();
		string errNum=ss.str();
		//列出错误个数，以及对应的语句
		ui->errorStmtsText->append(QString::fromStdString(errNum)+" errors occured.\n");
		auto errStmts=SyntaxParser.getErrorStmts(originalText);
		for(long long i=0;i<errStmts.getUsage();i++)
		{
			ui->errorStmtsText->append(QString::fromStdString(errStmts[i]));
			ui->errorStmtsText->append("\n");
		}
	}
}

void MainWindow::ClickShowSymbolTable()
{
	displaySymbolTable->setTokenList(this->tokenList);
	displaySymbolTable->show();

}
