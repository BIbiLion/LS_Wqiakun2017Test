#ifndef PLAYHANDTHREAD_H
#define PLAYHANDTHREAD_H

#include <QThread>

class Player;

class PlayHandThread : public QThread
{
	Q_OBJECT

public:
	PlayHandThread(Player* player);
	~PlayHandThread();

protected:
	virtual void run();

private:
	Player* m_player;
};

#endif // PLAYHANDTHREAD_H
