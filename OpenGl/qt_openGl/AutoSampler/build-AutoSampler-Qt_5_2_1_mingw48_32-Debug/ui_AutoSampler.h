/********************************************************************************
** Form generated from reading UI file 'AutoSampler.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTOSAMPLER_H
#define UI_AUTOSAMPLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AutoSampler
{
public:
    QWidget *centralWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QPlainTextEdit *plainTextEdit;
    QLabel *lab_pic;
    QGroupBox *groupBox;
    QLabel *label_7;
    QGroupBox *groupBox_4;
    QRadioButton *rad_2;
    QGroupBox *groupBox_13;
    QRadioButton *rad_11;
    QGroupBox *groupBox_14;
    QRadioButton *rad_12;
    QGroupBox *groupBox_15;
    QRadioButton *rad_13;
    QGroupBox *groupBox_7;
    QRadioButton *rad_5;
    QLabel *label_4;
    QLabel *label_2;
    QGroupBox *groupBox_3;
    QRadioButton *rad_1;
    QGroupBox *groupBox_17;
    QRadioButton *rad_15;
    QGroupBox *groupBox_16;
    QRadioButton *rad_14;
    QGroupBox *groupBox_8;
    QRadioButton *rad_6;
    QGroupBox *groupBox_10;
    QRadioButton *rad_8;
    QLabel *label;
    QLabel *label_5;
    QLabel *label_6;
    QGroupBox *groupBox_11;
    QRadioButton *rad_9;
    QGroupBox *groupBox_12;
    QRadioButton *rad_10;
    QGroupBox *groupBox_2;
    QRadioButton *rad_0;
    QGroupBox *groupBox_6;
    QRadioButton *rad_4;
    QGroupBox *groupBox_9;
    QRadioButton *rad_7;
    QLabel *label_3;
    QGroupBox *groupBox_5;
    QRadioButton *rad_3;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AutoSampler)
    {
        if (AutoSampler->objectName().isEmpty())
            AutoSampler->setObjectName(QStringLiteral("AutoSampler"));
        AutoSampler->resize(748, 462);
        centralWidget = new QWidget(AutoSampler);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(9, 9, 481, 231));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 479, 229));
        scrollArea->setWidget(scrollAreaWidgetContents);
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(500, 0, 241, 241));
        lab_pic = new QLabel(centralWidget);
        lab_pic->setObjectName(QStringLiteral("lab_pic"));
        lab_pic->setGeometry(QRect(10, 250, 481, 141));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lab_pic->sizePolicy().hasHeightForWidth());
        lab_pic->setSizePolicy(sizePolicy);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(500, 250, 241, 151));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(130, 40, 31, 16));
        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(160, 20, 16, 21));
        groupBox_4->setStyleSheet(QStringLiteral("border:none"));
        rad_2 = new QRadioButton(groupBox_4);
        rad_2->setObjectName(QStringLiteral("rad_2"));
        rad_2->setGeometry(QRect(0, 0, 16, 16));
        groupBox_13 = new QGroupBox(groupBox);
        groupBox_13->setObjectName(QStringLiteral("groupBox_13"));
        groupBox_13->setGeometry(QRect(130, 70, 16, 21));
        groupBox_13->setStyleSheet(QStringLiteral("border:none"));
        rad_11 = new QRadioButton(groupBox_13);
        rad_11->setObjectName(QStringLiteral("rad_11"));
        rad_11->setGeometry(QRect(0, 0, 16, 16));
        groupBox_14 = new QGroupBox(groupBox);
        groupBox_14->setObjectName(QStringLiteral("groupBox_14"));
        groupBox_14->setGeometry(QRect(100, 70, 16, 21));
        groupBox_14->setStyleSheet(QStringLiteral("border:none"));
        rad_12 = new QRadioButton(groupBox_14);
        rad_12->setObjectName(QStringLiteral("rad_12"));
        rad_12->setGeometry(QRect(0, 0, 16, 16));
        groupBox_15 = new QGroupBox(groupBox);
        groupBox_15->setObjectName(QStringLiteral("groupBox_15"));
        groupBox_15->setGeometry(QRect(70, 70, 16, 21));
        groupBox_15->setStyleSheet(QStringLiteral("border:none"));
        rad_13 = new QRadioButton(groupBox_15);
        rad_13->setObjectName(QStringLiteral("rad_13"));
        rad_13->setGeometry(QRect(0, 0, 16, 16));
        groupBox_7 = new QGroupBox(groupBox);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(70, 20, 16, 21));
        groupBox_7->setStyleSheet(QStringLiteral("border:none"));
        rad_5 = new QRadioButton(groupBox_7);
        rad_5->setObjectName(QStringLiteral("rad_5"));
        rad_5->setGeometry(QRect(0, 0, 16, 16));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(100, 90, 16, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(190, 40, 31, 16));
        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(190, 20, 16, 21));
        groupBox_3->setStyleSheet(QStringLiteral("border:none"));
        rad_1 = new QRadioButton(groupBox_3);
        rad_1->setObjectName(QStringLiteral("rad_1"));
        rad_1->setGeometry(QRect(0, 0, 16, 16));
        groupBox_17 = new QGroupBox(groupBox);
        groupBox_17->setObjectName(QStringLiteral("groupBox_17"));
        groupBox_17->setGeometry(QRect(10, 70, 16, 21));
        groupBox_17->setStyleSheet(QStringLiteral("border:none"));
        rad_15 = new QRadioButton(groupBox_17);
        rad_15->setObjectName(QStringLiteral("rad_15"));
        rad_15->setGeometry(QRect(0, 0, 16, 16));
        groupBox_16 = new QGroupBox(groupBox);
        groupBox_16->setObjectName(QStringLiteral("groupBox_16"));
        groupBox_16->setGeometry(QRect(40, 70, 16, 21));
        groupBox_16->setStyleSheet(QStringLiteral("border:none"));
        rad_14 = new QRadioButton(groupBox_16);
        rad_14->setObjectName(QStringLiteral("rad_14"));
        rad_14->setGeometry(QRect(0, 0, 16, 16));
        groupBox_8 = new QGroupBox(groupBox);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(40, 20, 16, 21));
        groupBox_8->setStyleSheet(QStringLiteral("border:none"));
        rad_6 = new QRadioButton(groupBox_8);
        rad_6->setObjectName(QStringLiteral("rad_6"));
        rad_6->setGeometry(QRect(0, 0, 16, 16));
        groupBox_10 = new QGroupBox(groupBox);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));
        groupBox_10->setGeometry(QRect(220, 70, 16, 21));
        groupBox_10->setStyleSheet(QStringLiteral("border:none"));
        rad_8 = new QRadioButton(groupBox_10);
        rad_8->setObjectName(QStringLiteral("rad_8"));
        rad_8->setGeometry(QRect(0, 0, 16, 16));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(220, 40, 31, 16));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(70, 90, 31, 16));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(160, 40, 31, 16));
        groupBox_11 = new QGroupBox(groupBox);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        groupBox_11->setGeometry(QRect(190, 70, 16, 21));
        groupBox_11->setStyleSheet(QStringLiteral("border:none"));
        rad_9 = new QRadioButton(groupBox_11);
        rad_9->setObjectName(QStringLiteral("rad_9"));
        rad_9->setGeometry(QRect(0, 0, 16, 16));
        groupBox_12 = new QGroupBox(groupBox);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        groupBox_12->setGeometry(QRect(160, 70, 16, 21));
        groupBox_12->setStyleSheet(QStringLiteral("border:none"));
        rad_10 = new QRadioButton(groupBox_12);
        rad_10->setObjectName(QStringLiteral("rad_10"));
        rad_10->setGeometry(QRect(0, 0, 16, 16));
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(220, 20, 16, 21));
        groupBox_2->setStyleSheet(QStringLiteral("border:none"));
        rad_0 = new QRadioButton(groupBox_2);
        rad_0->setObjectName(QStringLiteral("rad_0"));
        rad_0->setGeometry(QRect(0, 0, 16, 16));
        groupBox_6 = new QGroupBox(groupBox);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(100, 20, 16, 21));
        groupBox_6->setStyleSheet(QStringLiteral("border:none"));
        rad_4 = new QRadioButton(groupBox_6);
        rad_4->setObjectName(QStringLiteral("rad_4"));
        rad_4->setGeometry(QRect(0, 0, 16, 16));
        groupBox_9 = new QGroupBox(groupBox);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        groupBox_9->setGeometry(QRect(10, 20, 16, 21));
        groupBox_9->setStyleSheet(QStringLiteral("border:none"));
        rad_7 = new QRadioButton(groupBox_9);
        rad_7->setObjectName(QStringLiteral("rad_7"));
        rad_7->setGeometry(QRect(0, 0, 16, 16));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(130, 90, 21, 16));
        groupBox_5 = new QGroupBox(groupBox);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(130, 20, 16, 21));
        groupBox_5->setStyleSheet(QStringLiteral("border:none"));
        rad_3 = new QRadioButton(groupBox_5);
        rad_3->setObjectName(QStringLiteral("rad_3"));
        rad_3->setGeometry(QRect(0, 0, 16, 16));
        AutoSampler->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AutoSampler);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 748, 23));
        AutoSampler->setMenuBar(menuBar);
        mainToolBar = new QToolBar(AutoSampler);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        AutoSampler->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(AutoSampler);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        AutoSampler->setStatusBar(statusBar);

        retranslateUi(AutoSampler);

        QMetaObject::connectSlotsByName(AutoSampler);
    } // setupUi

    void retranslateUi(QMainWindow *AutoSampler)
    {
        AutoSampler->setWindowTitle(QApplication::translate("AutoSampler", "AutoSampler", 0));
        lab_pic->setText(QApplication::translate("AutoSampler", "TextLabel", 0));
        groupBox->setTitle(QApplication::translate("AutoSampler", "\345\274\225\350\204\232\347\224\265\345\271\263", 0));
        label_7->setText(QApplication::translate("AutoSampler", "Fpul", 0));
        groupBox_4->setTitle(QString());
        rad_2->setText(QString());
        groupBox_13->setTitle(QString());
        rad_11->setText(QString());
        groupBox_14->setTitle(QString());
        rad_12->setText(QString());
        groupBox_15->setTitle(QString());
        rad_13->setText(QString());
        groupBox_7->setTitle(QString());
        rad_5->setText(QString());
        label_4->setText(QApplication::translate("AutoSampler", "up", 0));
        label_2->setText(QApplication::translate("AutoSampler", "Mpul", 0));
        groupBox_3->setTitle(QString());
        rad_1->setText(QString());
        groupBox_17->setTitle(QString());
        rad_15->setText(QString());
        groupBox_16->setTitle(QString());
        rad_14->setText(QString());
        groupBox_8->setTitle(QString());
        rad_6->setText(QString());
        groupBox_10->setTitle(QString());
        rad_8->setText(QString());
        label->setText(QApplication::translate("AutoSampler", "Mdir", 0));
        label_5->setText(QApplication::translate("AutoSampler", "down", 0));
        label_6->setText(QApplication::translate("AutoSampler", "Fdir", 0));
        groupBox_11->setTitle(QString());
        rad_9->setText(QString());
        groupBox_12->setTitle(QString());
        rad_10->setText(QString());
        groupBox_2->setTitle(QString());
        rad_0->setText(QString());
        groupBox_6->setTitle(QString());
        rad_4->setText(QString());
        groupBox_9->setTitle(QString());
        rad_7->setText(QString());
        label_3->setText(QApplication::translate("AutoSampler", "pos", 0));
        groupBox_5->setTitle(QString());
        rad_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AutoSampler: public Ui_AutoSampler {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTOSAMPLER_H
