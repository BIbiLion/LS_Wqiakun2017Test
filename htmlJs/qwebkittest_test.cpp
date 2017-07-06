#include "qwebkittest_test.h"
#include "ui_qwebkittest_test.h"
#include <QDebug>
QWebKitTest_test::QWebKitTest_test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWebKitTest_test)
{

    m_webObj = new MyWebKit(this);
    ui->setupUi(this);
    qDebug()<<QApplication::applicationDirPath();
    //ui->webView->setUrl((QUrl(QApplication::applicationDirPath()+ "\\test.html")));
     ui->webView->load((QUrl::fromLocalFile(QApplication::applicationDirPath()+ "/test.html")));

     // 在QtWebkit的javascript里访问QObject的
    connect(ui->webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(populateJavaScriptWindowObject()));
    //this->setFixedSize(900,900);
    this->show();
}

QWebKitTest_test::~QWebKitTest_test()
{
    delete ui;
}

void QWebKitTest_test::setValueFromWeb(const QString &strName, const QString &strPwd)
{
    ui->textEdit->setText(strName);
    ui->textEdit_2->setText(strPwd);
}

void QWebKitTest_test::onBtnCallJSClicked()
{
    QString strVal = QString("callfromqt(\"%1\",\"%2\");").arg(ui->textEdit->toPlainText()).arg(ui->textEdit_2->toPlainText() );
    ui->webView->page()->mainFrame()->evaluateJavaScript(strVal);
}

void QWebKitTest_test::populateJavaScriptWindowObject()
{
    qDebug()<<"populateJavaScriptWindowObject";
    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject(QString("mywebkit"),m_webObj);
}

void QWebKitTest_test::on_pushButton_clicked()
{
    onBtnCallJSClicked();
}
