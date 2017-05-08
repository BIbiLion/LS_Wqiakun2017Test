#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QFrame>
#include "GameControl.h"
#include "CardPic.h"
#include <QResizeEvent>
#include <QMap>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include "MarkBoard.h"

class GamePanel : public QFrame
{
	Q_OBJECT

public:
	GamePanel(QWidget *parent);
	~GamePanel();

protected slots:
	// �û���ť����Ӧ
	void OnStartBtnClicked();
	void OnContinueBtnClicked();

	void OnUserNoBet();
	void OnUserBet1();
	void OnUserBet2();
	void OnUserBet3();

	void OnUserPass();
	void OnUserPlayHand();
	//////////////////////////////////////////////////////////////////////////

	void ShowPickingCardStep(Player* player, int step);
	void OnCardPicking();

	void OnCardPicSelected(Qt::MouseButton mouseButton);

	// �Խ�Player
	void OnPlayerCalledLord(Player* player, int bet);
	void OnPlayerPlayHand(Player* player, const Cards& cards);
	void OnPlayerPickCards(Player* player, const Cards& cards);

	void OnPlayerStatus(Player* player, GameControl::PlayerStatus playerStatus);

	void StartPickCard();

	void HidePlayerLastCards(Player* player);
	void UpdatePlayerCards(Player* player);

public slots:
	void ProcessGameStatus(GameControl::GameStatus gameStatus);

protected:
	virtual void paintEvent(QPaintEvent* event);
	virtual void resizeEvent(QResizeEvent* event);

	void InitCardPicMap();
	void CutCardPic(const QPixmap& cardsPic, int x, int y, Card card);	// �ڿ�Ƭͼ���Ͻ�Ϊ(x, y)������ָ���ߴ磬��card��Ӧ����
	void InitControls();
	void InitPlayerContext();

	void UpdateMarkBoard();

protected:
	enum CardsAlign
	{
		Horizontal,
		Vertical
	};

	struct PlayerContext
	{
		QRect cardsRect;
		QRect playHandRect;
		CardsAlign cardsAlign;
		bool isFrontSide;
		Cards lastCards;
		QLabel* info;
		QLabel* rolePic;
	};

	QMap<Player*, PlayerContext> m_playerContextMap;
private:
	GameControl* m_gameControl;

	// ���ƶ���ʱ���ƺ��ƶ���ͼƬ
	CardPic* m_baseCard;
	CardPic* m_movingCard;
	
	QVector<CardPic*> m_restThreeCards;

	// �Ʊ����ͼ��
	QPixmap m_cardBackPic;

	QSize m_cardSize;
	QPoint m_baseCardPos;

	QMap<Card, CardPic*> m_cardPicMap;	// ÿ�ſ�Ƭ��Ӧһ��ͼƬ
	QSet<CardPic*> m_selectedCards;		// �û�����ǰѡ�����ͼƬ

	GameControl::GameStatus m_gameStatus;

	QTimer* m_pickCardTimer;	// ���Ƽ�ʱ��
	int m_curMoveStep;

	QFrame* m_userTool;
	QPushButton* m_startButton;
	QPushButton* m_continueButton;
	QPushButton* m_playHandButton;
	QPushButton* m_passButton;
	QPushButton* m_bet0Button;
	QPushButton* m_bet1Button;
	QPushButton* m_bet2Button;
	QPushButton* m_bet3Button;

	MarkBoard* m_markBoard;

};

#endif // GAMEPANEL_H
