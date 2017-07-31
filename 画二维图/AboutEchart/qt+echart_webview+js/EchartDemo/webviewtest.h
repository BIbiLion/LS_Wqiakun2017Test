#ifndef WEBVIEWTEST_H
#define WEBVIEWTEST_H

#include <QWidget>

namespace Ui {
class WebViewTest;
}

class WebViewTest : public QWidget
{
    Q_OBJECT

public:
    explicit WebViewTest(QWidget *parent = 0);
    ~WebViewTest();

public slots:
    void loadFinshed_slot(bool);
private:
    Ui::WebViewTest *ui;
};

#endif // WEBVIEWTEST_H
