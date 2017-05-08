#ifndef QRCONDEZXING_H
#define QRCONDEZXING_H

#include <QWidget>
#include <QTextCodec>
#include <QFileDialog>

namespace Ui {
class QRCondeZxing;
}

class QRCondeZxing : public QWidget
{
    Q_OBJECT

public:
    explicit QRCondeZxing(QWidget *parent = 0);
    ~QRCondeZxing();

private slots:
    void on_pushButton_clicked();

private:
    Ui::QRCondeZxing *ui;
};

#endif // QRCONDEZXING_H
