#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <QObject>
#include <QTimerEvent>
class TestObject : public QObject
{
    Q_OBJECT
public:
    explicit TestObject(QObject *parent = 0);
    ~TestObject();
protected:
    void timerEvent(QTimerEvent *event);
signals:

public slots:
    void timeSlot();


};

#endif // TESTOBJECT_H
