#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <QWidget>

class myPlugin : public QWidget
{
    Q_OBJECT

public:
    myPlugin(QWidget *parent = 0);
};

#endif
