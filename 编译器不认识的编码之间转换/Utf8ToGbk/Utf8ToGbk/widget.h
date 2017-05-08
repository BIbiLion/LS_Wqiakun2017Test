#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_btnChange_clicked();
    void on_btnProject_clicked();

private:
    Ui::Widget *ui;

    QString m_strTrPath;
    void changeTextcodec(QString fileName);
};

#endif // WIDGET_H
