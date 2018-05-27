#ifndef DISPLAYSYMBOLTABLE_H
#define DISPLAYSYMBOLTABLE_H

#include <QMainWindow>
#include "TokenParser.h"


namespace Ui
{
	class DisplaySymbolTable;
}

class DisplaySymbolTable:public QMainWindow
{
Q_OBJECT

public:
	explicit DisplaySymbolTable(QWidget *parent=nullptr);
	~DisplaySymbolTable() override;
	void setTokenList(const Sequence<TokenInf> &tokenList);

public slots:
	void ClickConfirm();
	void ClickCancel();
	void show();
private:
	Ui::DisplaySymbolTable *ui;
	Sequence<TokenInf> tokenList;
};

#endif // DISPLAYSYMBOLTABLE_H
