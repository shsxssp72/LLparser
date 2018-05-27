/********************************************************************************
** Form generated from reading UI file 'displaysymboltable.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAYSYMBOLTABLE_H
#define UI_DISPLAYSYMBOLTABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DisplaySymbolTable
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *SymbolTableListText;
    QHBoxLayout *horizontalLayout;
    QPushButton *ConfirmButton;
    QPushButton *CancelButton;

    void setupUi(QMainWindow *DisplaySymbolTable)
    {
        if (DisplaySymbolTable->objectName().isEmpty())
            DisplaySymbolTable->setObjectName(QStringLiteral("DisplaySymbolTable"));
        DisplaySymbolTable->resize(365, 349);
        centralwidget = new QWidget(DisplaySymbolTable);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        SymbolTableListText = new QPlainTextEdit(centralwidget);
        SymbolTableListText->setObjectName(QStringLiteral("SymbolTableListText"));
        QFont font;
        font.setFamily(QStringLiteral("Consolas"));
        font.setPointSize(10);
        SymbolTableListText->setFont(font);
        SymbolTableListText->setReadOnly(true);

        verticalLayout->addWidget(SymbolTableListText);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        ConfirmButton = new QPushButton(centralwidget);
        ConfirmButton->setObjectName(QStringLiteral("ConfirmButton"));
        ConfirmButton->setFont(font);

        horizontalLayout->addWidget(ConfirmButton);

        CancelButton = new QPushButton(centralwidget);
        CancelButton->setObjectName(QStringLiteral("CancelButton"));
        CancelButton->setFont(font);

        horizontalLayout->addWidget(CancelButton);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);

        DisplaySymbolTable->setCentralWidget(centralwidget);

        retranslateUi(DisplaySymbolTable);

        QMetaObject::connectSlotsByName(DisplaySymbolTable);
    } // setupUi

    void retranslateUi(QMainWindow *DisplaySymbolTable)
    {
        DisplaySymbolTable->setWindowTitle(QApplication::translate("DisplaySymbolTable", "SymbolTable", nullptr));
        ConfirmButton->setText(QApplication::translate("DisplaySymbolTable", "OK", nullptr));
        CancelButton->setText(QApplication::translate("DisplaySymbolTable", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DisplaySymbolTable: public Ui_DisplaySymbolTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYSYMBOLTABLE_H
