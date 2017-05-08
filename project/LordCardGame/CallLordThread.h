#ifndef CALLLORDTHREAD_H
#define CALLLORDTHREAD_H

#include <QThread>

class Player;

class CallLordThread : public QThread
{
	Q_OBJECT

public:
	CallLordThread(Player* player);
	~CallLordThread();

protected:
	virtual void run();

private:
	Player* m_player;
	
};

#endif // CALLLORDTHREAD_H
