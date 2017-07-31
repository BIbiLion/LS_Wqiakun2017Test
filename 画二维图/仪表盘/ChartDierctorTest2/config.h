#ifndef CONFIG_H
#define CONFIG_H

#include <QWidget>

class config : public QWidget
{
    Q_OBJECT
public:
    explicit config(QWidget *parent = 0);
    QString GetSavePath();
    void SetSavePath(QString str);
signals:

public slots:
private:
    QString m_savePath;
};

#endif // CONFIG_H
