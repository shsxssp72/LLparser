/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>


QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
	QWidget *centralWidget;
	QHBoxLayout *horizontalLayout_4;
	QHBoxLayout *horizontalLayout_3;
	QVBoxLayout *verticalLayout;
	QLabel *inst_FilePath;
	QHBoxLayout *horizontalLayout;
	QLabel *filePathLabel;
	QPushButton *openFileButton;
	QTextEdit *inputFileText;
	QLabel *inst_ErrStmts;
	QTextEdit *errorStmtsText;
	QHBoxLayout *horizontalLayout_2;
	QPushButton *parseButton;
	QPushButton *showSymbolTableButton;
	QVBoxLayout *verticalLayout_3;
	QLabel *inst_ParseProcess;
	QTextEdit *parseProcessText;

	void setupUi(QMainWindow *MainWindow)
	{
		if(MainWindow->objectName().isEmpty())
			MainWindow->setObjectName(QStringLiteral("MainWindow"));
		MainWindow->resize(1035,750);
		centralWidget=new QWidget(MainWindow);
		centralWidget->setObjectName(QStringLiteral("centralWidget"));
		horizontalLayout_4=new QHBoxLayout(centralWidget);
		horizontalLayout_4->setSpacing(6);
		horizontalLayout_4->setContentsMargins(11,11,11,11);
		horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
		horizontalLayout_3=new QHBoxLayout();
		horizontalLayout_3->setSpacing(6);
		horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
		verticalLayout=new QVBoxLayout();
		verticalLayout->setSpacing(6);
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		inst_FilePath=new QLabel(centralWidget);
		inst_FilePath->setObjectName(QStringLiteral("inst_FilePath"));
		QFont font;
		font.setFamily(QStringLiteral("Consolas"));
		font.setPointSize(10);
		inst_FilePath->setFont(font);

		verticalLayout->addWidget(inst_FilePath);

		horizontalLayout=new QHBoxLayout();
		horizontalLayout->setSpacing(6);
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
		filePathLabel=new QLabel(centralWidget);
		filePathLabel->setObjectName(QStringLiteral("filePathLabel"));
		filePathLabel->setFont(font);

		horizontalLayout->addWidget(filePathLabel);

		openFileButton=new QPushButton(centralWidget);
		openFileButton->setObjectName(QStringLiteral("openFileButton"));
		openFileButton->setFont(font);

		horizontalLayout->addWidget(openFileButton);


		verticalLayout->addLayout(horizontalLayout);

		inputFileText=new QTextEdit(centralWidget);
		inputFileText->setObjectName(QStringLiteral("inputFileText"));
		inputFileText->setFont(font);
		inputFileText->setReadOnly(true);

		verticalLayout->addWidget(inputFileText);

		inst_ErrStmts=new QLabel(centralWidget);
		inst_ErrStmts->setObjectName(QStringLiteral("inst_ErrStmts"));
		inst_ErrStmts->setFont(font);

		verticalLayout->addWidget(inst_ErrStmts);

		errorStmtsText=new QTextEdit(centralWidget);
		errorStmtsText->setObjectName(QStringLiteral("errorStmtsText"));
		QFont font1;
		font1.setFamily(QStringLiteral("Consolas"));
		errorStmtsText->setFont(font1);
		errorStmtsText->setReadOnly(true);

		verticalLayout->addWidget(errorStmtsText);

		horizontalLayout_2=new QHBoxLayout();
		horizontalLayout_2->setSpacing(6);
		horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
		parseButton=new QPushButton(centralWidget);
		parseButton->setObjectName(QStringLiteral("parseButton"));
		parseButton->setFont(font);

		horizontalLayout_2->addWidget(parseButton);

		showSymbolTableButton=new QPushButton(centralWidget);
		showSymbolTableButton->setObjectName(QStringLiteral("showSymbolTableButton"));
		showSymbolTableButton->setFont(font);

		horizontalLayout_2->addWidget(showSymbolTableButton);


		verticalLayout->addLayout(horizontalLayout_2);


		horizontalLayout_3->addLayout(verticalLayout);

		verticalLayout_3=new QVBoxLayout();
		verticalLayout_3->setSpacing(6);
		verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
		inst_ParseProcess=new QLabel(centralWidget);
		inst_ParseProcess->setObjectName(QStringLiteral("inst_ParseProcess"));
		inst_ParseProcess->setFont(font);

		verticalLayout_3->addWidget(inst_ParseProcess);

		parseProcessText=new QTextEdit(centralWidget);
		parseProcessText->setObjectName(QStringLiteral("parseProcessText"));
		parseProcessText->setFont(font);
		parseProcessText->setReadOnly(true);

		verticalLayout_3->addWidget(parseProcessText);


		horizontalLayout_3->addLayout(verticalLayout_3);


		horizontalLayout_4->addLayout(horizontalLayout_3);

		MainWindow->setCentralWidget(centralWidget);

		retranslateUi(MainWindow);

		QMetaObject::connectSlotsByName(MainWindow);
	} // setupUi

	void retranslateUi(QMainWindow *MainWindow)
	{
		MainWindow->setWindowTitle(QApplication::translate("MainWindow","SimpleLLParser",nullptr));
		inst_FilePath->setText(QApplication::translate("MainWindow","File Path:",nullptr));
		filePathLabel->setText(QApplication::translate("MainWindow","NULL",nullptr));
		openFileButton->setText(QApplication::translate("MainWindow","Open",nullptr));
		inst_ErrStmts->setText(QApplication::translate("MainWindow","Error Statments:",nullptr));
		parseButton->setText(QApplication::translate("MainWindow","Parse",nullptr));
		showSymbolTableButton->setText(QApplication::translate("MainWindow","ShowSymbolTable",nullptr));
		inst_ParseProcess->setText(QApplication::translate("MainWindow","Parse Process:",nullptr));
	} // retranslateUi

};

namespace Ui
{
	class MainWindow:public Ui_MainWindow
	{
	};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
