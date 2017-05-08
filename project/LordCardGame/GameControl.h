#pragma once

#include <QObject>
#include "Robot.h"
#include "UserPlayer.h"
#include <QTimer>
#include <QVector>

class GameControl : public QObject
{
	Q_OBJECT

public:
	enum GameStatus
	{
		PickingCardStatus,
		CallingLordStatus,
		PlayingHandStatus,
	};

	enum PlayerStatus
	{
		ThinkingForCallLordStatus,
		ThinkingForPlayHandStatus,
		WinningStatus
	};

	GameControl(QObject* parent = NULL);
	~GameControl();
	
	void InitCards();		// ��ʼ�������ƣ���䵽m_allCards
	void AddCard(CardSuit suit, CardPoint point);
	void ShuffleCards();	// ϴ��
	void Init();		// ��ʼ����ң��źŲ�

	void Clear();
	
	Player* GetUserPlayer();
	Player* GetLeftRobot();
	Player* GetRightRobot();
	Player* GetCurrentPlayer();
	void SetCurrentPlayer(Player* curPlayer);

	Cards GetRestCards();
	Card PickOneCard();

	Player* GetPunchPlayer();
	Cards GetPunchCards();

	int GetCurBet();
	void SetCurBet(int curBet);

	void ClearPlayerMarks();

signals:
	// �����ź�֪ͨ��GamePanel
	void NotifyPlayerCalledLord(Player*, int);	// ��ҽз�
	void NotifyPlayerPlayHand(Player*, const Cards& cards);
	void NotityPlayerPickCards(Player*, const Cards& cards);
	void NotifyPlayerPunch(Player*, const Cards& cards);

	void NotifyGameStatus(GameControl::GameStatus);
	void NotifyPlayerStatus(Player*, GameControl::PlayerStatus);

protected slots:
	void OnPlayerCalledLord(Player* player, int bet);
	void OnPlayerPlayedHand(Player* player, const Cards& cards);
	void OnPlayerPickCards(Player* player, const Cards& cards);

public slots:
	void StartCallLord();	// ��ʼ�з�
	void StartPlayHand();	// ��ʼ����

protected:
	struct BetRecrod
	{
		Player* player;
		int bet;
	};

protected:
	Robot* m_robotLeft;
	Robot* m_robotRight;
	UserPlayer* m_user;

	Player* m_curPlayer;
	int m_curBet;

	Cards m_allCards;

	// ��ǰ����Ч�Ƶ���Һ���
	Player* m_punchPlayer;
	Cards m_punchCards;

	QVector<BetRecrod> m_betList;

};


