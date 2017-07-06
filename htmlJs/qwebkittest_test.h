#ifndef QWEBKITTEST_TEST_H
#define QWEBKITTEST_TEST_H

#include <QDialog>
#include <QString>
#include <QWidget>
#include <QWebView>
#include <QWebFrame>
#include <QTextEdit>
#include "mywebkit.h"
namespace Ui {
class QWebKitTest_test;
}

class QWebKitTest_test : public QDialog
{
    Q_OBJECT

public:
    explicit QWebKitTest_test(QWidget *parent = 0);
    ~QWebKitTest_test();
    void setValueFromWeb(const QString &strName,const QString &strPwd);

protected slots:
    void onBtnCallJSClicked();
    void populateJavaScriptWindowObject();

private slots:
    void on_pushButton_clicked();
private:
    Ui::QWebKitTest_test *ui;
      class MyWebKit* m_webObj;
};

#endif // QWEBKITTEST_TEST_H
