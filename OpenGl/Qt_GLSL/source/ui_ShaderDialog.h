/********************************************************************************
** Form generated from reading UI file 'ShaderDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHADERDIALOG_H
#define UI_SHADERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include "ShaderEditor/ShaderEditor.h"

QT_BEGIN_NAMESPACE

class Ui_ShaderDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    ShaderEditor *pShaderEdit;
    QHBoxLayout *horizontalLayout;
    QLabel *pHintLabel;
    QPushButton *pCompileButton;
    QPushButton *pSaveIcon;

    void setupUi(QDialog *ShaderDialog)
    {
        if (ShaderDialog->objectName().isEmpty())
            ShaderDialog->setObjectName(QStringLiteral("ShaderDialog"));
        ShaderDialog->resize(400, 300);
        gridLayout = new QGridLayout(ShaderDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pShaderEdit = new ShaderEditor(ShaderDialog);
        pShaderEdit->setObjectName(QStringLiteral("pShaderEdit"));

        verticalLayout->addWidget(pShaderEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pHintLabel = new QLabel(ShaderDialog);
        pHintLabel->setObjectName(QStringLiteral("pHintLabel"));

        horizontalLayout->addWidget(pHintLabel);

        pCompileButton = new QPushButton(ShaderDialog);
        pCompileButton->setObjectName(QStringLiteral("pCompileButton"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pCompileButton->sizePolicy().hasHeightForWidth());
        pCompileButton->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icon/compileIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        pCompileButton->setIcon(icon);

        horizontalLayout->addWidget(pCompileButton);

        pSaveIcon = new QPushButton(ShaderDialog);
        pSaveIcon->setObjectName(QStringLiteral("pSaveIcon"));
        sizePolicy.setHeightForWidth(pSaveIcon->sizePolicy().hasHeightForWidth());
        pSaveIcon->setSizePolicy(sizePolicy);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Icon/saveIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        pSaveIcon->setIcon(icon1);

        horizontalLayout->addWidget(pSaveIcon);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(ShaderDialog);

        QMetaObject::connectSlotsByName(ShaderDialog);
    } // setupUi

    void retranslateUi(QDialog *ShaderDialog)
    {
        ShaderDialog->setWindowTitle(QApplication::translate("ShaderDialog", "\347\235\200\350\211\262\345\231\250\347\274\226\350\276\221\345\231\250", 0));
        pHintLabel->setText(QApplication::translate("ShaderDialog", "\346\255\244\345\244\204\346\214\207\346\230\216\347\235\200\350\211\262\345\231\250\347\232\204\347\261\273\345\236\213", 0));
        pCompileButton->setText(QApplication::translate("ShaderDialog", "\347\274\226\350\257\221\351\223\276\346\216\245", 0));
        pSaveIcon->setText(QApplication::translate("ShaderDialog", "\345\217\246\345\255\230\344\270\272\342\200\246\342\200\246", 0));
    } // retranslateUi

};

namespace Ui {
    class ShaderDialog: public Ui_ShaderDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHADERDIALOG_H
