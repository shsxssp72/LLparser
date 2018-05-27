#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Initialize.h"
#include "displaysymboltable.h"


namespace Ui
{
	class MainWindow;
}

class MainWindow:public QMainWindow
{
Q_OBJECT

public:
	explicit MainWindow(QWidget *parent=nullptr);
	~MainWindow() override;
public slots:
	void ClickOpen();
	void ClickParse();
	void ClickShowSymbolTable();

private:
	Ui::MainWindow *ui;
	DisplaySymbolTable *displaySymbolTable;
	TokenParser tokenParser;
	LLParser SyntaxParser;
	Sequence<TokenInf> tokenList;
	string forSyntaxParse;
	string originalText;
};

#endif // MAINWINDOW_H
