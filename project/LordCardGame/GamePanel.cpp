#include "GamePanel.h"
#include <QPainter>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include "Hand.h"
#include "Strategy.h"

GamePanel::GamePanel(QWidget *parent)
	: QFrame(parent)
{
	m_gameControl = new GameControl(this);
	m_gameControl->Init();

	m_cardSize = QSize(80, 105);

	m_markBoard = new MarkBoard(this);
	m_markBoard->show();
	UpdateMarkBoard();

	InitCardPicMap();
	InitControls();
	InitPlayerContext();

	//////////////////////////////////////////////////////////////////////////
	
	connect(m_gameControl, SIGNAL(NotifyPlayerCalledLord(Player*, int)), 
		this, SLOT(OnPlayerCalledLord(Player*, int)));

	connect(m_gameControl, SIGNAL(NotifyPlayerPlayHand(Player*, const Cards&)),
		this, SLOT(OnPlayerPlayHand(Player*, const Cards&)));

	connect(m_gameControl, SIGNAL(NotityPlayerPickCards(Player*, const Cards&)),
		this, SLOT(OnPlayerPickCards(Player*, const Cards&)));

	connect(m_gameControl, SIGNAL(NotifyGameStatus(GameControl::GameStatus)), 
		this, SLOT(ProcessGameStatus(GameControl::GameStatus)));

	connect(m_gameControl, SIGNAL(NotifyPlayerStatus(Player*, GameControl::PlayerStatus)), 
		this, SLOT(OnPlayerStatus(Player*, GameControl::PlayerStatus)));
	

	m_baseCard = new CardPic(this);
	m_movingCard = new CardPic(this);
	m_baseCard->SetPic(m_cardBackPic, m_cardBackPic);
	m_movingCard->SetPic(m_cardBackPic, m_cardBackPic);

	// ʣ��3����
	CardPic* restCardPic = new CardPic(this);
	m_restThreeCards << restCardPic;
	restCardPic = new CardPic(this);
	m_restThreeCards << restCardPic;
	restCardPic = new CardPic(this);
	m_restThreeCards << restCardPic;

	// ����Ч�����õļ�ʱ��
	m_curMoveStep = 0;
	m_pickCardTimer = new QTimer(this);
	m_pickCardTimer->setInterval(8);
	connect(m_pickCardTimer, SIGNAL(timeout()), this, SLOT(OnCardPicking()));
}

GamePanel::~GamePanel()
{

}

void GamePanel::InitPlayerContext()
{
	PlayerContext context;

	context.cardsAlign = Vertical;
	context.isFrontSide = false;
	m_playerContextMap.insert(m_gameControl->GetLeftRobot(), context);

	context.cardsAlign = Vertical;
	context.isFrontSide = false;
	m_playerContextMap.insert(m_gameControl->GetRightRobot(), context);

	context.cardsAlign = Horizontal;
	context.isFrontSide = true;
	m_playerContextMap.insert(m_gameControl->GetUserPlayer(), context);

	QMap<Player*, PlayerContext>::Iterator it = m_playerContextMap.begin();
	for (; it != m_playerContextMap.end(); it++)
	{
		it->info = new QLabel(this);
		it->info->resize(100, 50);
		it->info->setStyleSheet(QString::fromLocal8Bit("QLabel { color: rgb(225, 30, 10); font-size: 30px; font-family:'��Բ'; }"));
		it->info->hide();
		it->rolePic = new QLabel(this);
		it->rolePic->resize(84, 120);
		it->rolePic->hide();
	}
}

void GamePanel::InitControls()
{
	m_userTool = new QFrame(this);
	QHBoxLayout* horLayout = new QHBoxLayout(m_userTool);
	horLayout->setSpacing(9);

	QString styleSheet;
	styleSheet = 
		"QPushButton			{ border-image: url(:/res/button.png) 0 267 0 0; color: white; }"
		"QPushButton:hover		{ border-image: url(:/res/button.png) 0 178 0 89; }"
		"QPushButton:pressed	{ border-image: url(:/res/button.png) 0 89 0 178; }";

	horLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

	m_startButton = new QPushButton(m_userTool);
	m_startButton->setFocusPolicy(Qt::NoFocus);
	m_startButton->setText(tr("Start Game"));
	m_startButton->show();
	m_startButton->setFixedSize(89, 32);
	m_startButton->setStyleSheet(styleSheet);
	horLayout->addWidget(m_startButton);
	connect(m_startButton, SIGNAL(clicked()), this, SLOT(OnStartBtnClicked()));

	m_continueButton = new QPushButton(m_userTool);
	m_continueButton->setFocusPolicy(Qt::NoFocus);
	m_continueButton->setText(tr("Continue Game"));
	m_continueButton->hide();
	m_continueButton->setFixedSize(89, 32);
	m_continueButton->setStyleSheet(styleSheet);
	horLayout->addWidget(m_continueButton);
	connect(m_continueButton, SIGNAL(clicked()), this, SLOT(OnContinueBtnClicked()));

	m_passButton = new QPushButton(m_userTool);
	m_passButton->setFocusPolicy(Qt::NoFocus);
	m_passButton->setText(tr("btn_pass"));
	m_passButton->hide();
	m_passButton->setFixedSize(89, 32);
	m_passButton->setStyleSheet(styleSheet);
	horLayout->addWidget(m_passButton);
	connect(m_passButton, SIGNAL(clicked()), this, SLOT(OnUserPass()));

	m_playHandButton = new QPushButton(m_userTool);
	m_playHandButton->setFocusPolicy(Qt::NoFocus);
	m_playHandButton->setText(tr("play hand"));
	m_playHandButton->hide();
	m_playHandButton->setFixedSize(89, 32);
	m_playHandButton->setStyleSheet(styleSheet);
	QFont font = m_playHandButton->font();
	font.setBold(true);
	font.setPixelSize(14);
	m_playHandButton->setFont(font);
	horLayout->addWidget(m_playHandButton);
	connect(m_playHandButton, SIGNAL(clicked()), this, SLOT(OnUserPlayHand()));

	//////////////////////////////////////////////////////////////////////////

	m_bet0Button = new QPushButton(m_userTool);
	m_bet0Button->setFocusPolicy(Qt::NoFocus);
	m_bet0Button->setText(tr("bet0"));
	m_bet0Button->hide();
	m_bet0Button->setFixedSize(89, 32);
	m_bet0Button->setStyleSheet(styleSheet);
	horLayout->addWidget(m_bet0Button);
	connect(m_bet0Button, SIGNAL(clicked()), this, SLOT(OnUserNoBet()));

	m_bet1Button = new QPushButton(m_userTool);
	m_bet1Button->setFocusPolicy(Qt::NoFocus);
	m_bet1Button->setText(tr("bet1"));
	m_bet1Button->hide();
	m_bet1Button->setFixedSize(89, 32);
	m_bet1Button->setStyleSheet(styleSheet);
	horLayout->addWidget(m_bet1Button);
	connect(m_bet1Button, SIGNAL(clicked()), this, SLOT(OnUserBet1()));

	m_bet2Button = new QPushButton(m_userTool);
	m_bet2Button->setFocusPolicy(Qt::NoFocus);
	m_bet2Button->setText(tr("bet2"));
	m_bet2Button->hide();
	m_bet2Button->setFixedSize(89, 32);
	m_bet2Button->setStyleSheet(styleSheet);
	horLayout->addWidget(m_bet2Button);
	connect(m_bet2Button, SIGNAL(clicked()), this, SLOT(OnUserBet2()));

	m_bet3Button = new QPushButton(m_userTool);
	m_bet3Button->setFocusPolicy(Qt::NoFocus);
	m_bet3Button->setText(tr("bet3"));
	m_bet3Button->hide();
	m_bet3Button->setFixedSize(89, 32);
	m_bet3Button->setStyleSheet(styleSheet);
	horLayout->addWidget(m_bet3Button);
	connect(m_bet3Button, SIGNAL(clicked()), this, SLOT(OnUserBet3()));

	horLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

void GamePanel::CutCardPic(const QPixmap& cardsPic,int x, int y, Card card)
{
	QPixmap pic = cardsPic.copy(x, y, m_cardSize.width(), m_cardSize.height());

	CardPic* cardPic = new CardPic(this);
	cardPic->hide();
	cardPic->SetPic(pic, m_cardBackPic);

	cardPic->SetCard(card);
	m_cardPicMap.insert(card, cardPic);

	connect(cardPic, SIGNAL(NotifySelected(Qt::MouseButton)), this, SLOT(OnCardPicSelected(Qt::MouseButton)));
}

void GamePanel::InitCardPicMap()
{
	QPixmap cardsPic(":/res/card.png");

	m_cardBackPic = cardsPic.copy(2 * m_cardSize.width(), 4 * m_cardSize.height(), 
		m_cardSize.width(), m_cardSize.height());

	int i = 0, j = 0;
	for (int suit = Suit_Begin + 1, i = 0; suit < Suit_End; suit++, i++)
	{
		for (int pt = Card_Begin + 1, j = 0; pt < Card_SJ; pt++, j++)
		{
			Card card;
			card.point = (CardPoint)pt;
			card.suit = (CardSuit)suit;

			CutCardPic(cardsPic, j * m_cardSize.width(), i * m_cardSize.height(), card);
		}
	}

	// ��С�������⣬�ֱ���
	Card card;

	card.point = Card_SJ;
	card.suit = Suit_Begin;
	CutCardPic(cardsPic, 0, 4 * m_cardSize.height(), card);

	card.point = Card_BJ;
	card.suit = Suit_Begin;
	CutCardPic(cardsPic, m_cardSize.width(), 4 * m_cardSize.height(), card);
}

void GamePanel::paintEvent(QPaintEvent* event)
{
	static QPixmap bk(":/res/table.png");
	QPainter painter(this);
	painter.drawPixmap(rect(), bk);

/*	QMap<Player*, PlayerContext>::ConstIterator it = m_playerContextMap.constBegin();
	for (; it != m_playerContextMap.constEnd(); it++)
	{
		QPen pen1(QColor(0, 0, 0));
		painter.setPen(pen1);
		painter.drawRect(it->cardsRect);

		QPen pen2(QColor(255, 0, 0));
		painter.setPen(pen2);
		painter.drawRect(it->playHandRect);
	}
*/
}

void GamePanel::resizeEvent(QResizeEvent* event)
{
	QFrame::resizeEvent(event);

	m_markBoard->setGeometry(rect().right() - 150, 0, 130, 80);

	m_baseCardPos = QPoint((width() - m_cardSize.width()) / 2, height() / 2 - 100);
	m_baseCard->move(m_baseCardPos);
	m_movingCard->move(m_baseCardPos);

	//////////////////////////////////////////////////////////////////////////
	QMap<Player*, PlayerContext>::Iterator it;

	// �������
	it = m_playerContextMap.find(m_gameControl->GetLeftRobot());
	it->cardsRect = QRect(10, 180, 100, height() - 200);
	it->playHandRect = QRect(180, 150, 100, 100);
	it->rolePic->move(it->cardsRect.left(), it->cardsRect.top() - 100);

	// �һ�����
	it = m_playerContextMap.find(m_gameControl->GetRightRobot());
	it->cardsRect = QRect(rect().right() - 110, 180, 100, height() - 200);
	it->playHandRect = QRect(rect().right() - 280, 150, 100, 100);
	it->rolePic->move(it->cardsRect.left(), it->cardsRect.top() - 100);

	// ��
	it = m_playerContextMap.find(m_gameControl->GetUserPlayer());
	it->cardsRect = QRect(250, rect().bottom() - 120, width() - 500, 100);
	it->playHandRect = QRect(150, rect().bottom() - 280, width() - 300, 100);
	it->rolePic->move(it->cardsRect.right(), it->cardsRect.top() - 10);
	
	// 
	QRect rectUserTool(it->playHandRect.left(), it->playHandRect.bottom(), 
		it->playHandRect.width(), it->cardsRect.top() - it->playHandRect.bottom());

	m_userTool->setGeometry(rectUserTool);
	
	//
	for (it = m_playerContextMap.begin(); it != m_playerContextMap.end(); it++)
	{
		QRect playCardsRect = it->playHandRect;
		QLabel* infoLabel = it->info;

		QPoint pt( playCardsRect.left() + (playCardsRect.width() - infoLabel->width()) / 2,
			playCardsRect.top() + (playCardsRect.height() - infoLabel->height()) / 2 );
		infoLabel->move(pt);
	}

	// ��ʾʣ��������
	int base = (width() - 3 * m_cardSize.width() - 2 * 10) / 2;
	for (int i = 0; i < m_restThreeCards.size(); i++)
	{
		m_restThreeCards[i]->move(base + (m_cardSize.width() + 10) * i, 20);
	}

	UpdatePlayerCards(m_gameControl->GetUserPlayer());
	UpdatePlayerCards(m_gameControl->GetLeftRobot());
	UpdatePlayerCards(m_gameControl->GetRightRobot());
}

void GamePanel::UpdateMarkBoard()
{
	m_markBoard->SetMarks(m_gameControl->GetUserPlayer()->GetMark(),
						  m_gameControl->GetLeftRobot()->GetMark(),
						  m_gameControl->GetRightRobot()->GetMark());
}

void GamePanel::OnCardPicking()
{
	Player* curPlayer = m_gameControl->GetCurrentPlayer();

	if (m_curMoveStep >= 100)	// �����ƶ���ĩ��
	{
		Card card = m_gameControl->PickOneCard();
		curPlayer->PickCard(card);

		ShowPickingCardStep(curPlayer, m_curMoveStep);
		m_gameControl->SetCurrentPlayer(curPlayer->GetNextPlayer());
		
		m_curMoveStep = 0;

		if (m_gameControl->GetRestCards().Count() == 3)	// ��ʣ��3���ƣ����ƽ���
		{
			m_pickCardTimer->stop();
			ProcessGameStatus(GameControl::CallingLordStatus);
		}
	}

	ShowPickingCardStep(curPlayer, m_curMoveStep);
	m_curMoveStep += 14;
}

void GamePanel::ShowPickingCardStep(Player* player, int step)
{
	QRect cardsRect = m_playerContextMap[player].cardsRect;

	if (player == m_gameControl->GetLeftRobot())
	{
		int unit = (m_baseCardPos.x() - cardsRect.right()) / 100;
		m_movingCard->move(m_baseCardPos.x() - step * unit, m_baseCardPos.y());
	}
	else if (player == m_gameControl->GetRightRobot())
	{
		int unit = (cardsRect.left() - m_baseCardPos.x()) / 100;
		m_movingCard->move(m_baseCardPos.x() + step * unit, m_baseCardPos.y());
	}
	else if (player == m_gameControl->GetUserPlayer())
	{
		int unit = (cardsRect.top() - m_baseCardPos.y()) / 100;
		m_movingCard->move(m_baseCardPos.x(), m_baseCardPos.y() + step * unit);
	}

	if (step == 0)
	{
		m_movingCard->show();
	}

	if (step >= 100)
	{
		m_movingCard->hide();
	}

	update();
}

void GamePanel::OnCardPicSelected(Qt::MouseButton mouseButton)
{
	if (m_gameStatus == GameControl::PickingCardStatus) return;

	CardPic* cardPic = (CardPic*)sender();
	if (cardPic->GetOwner() != m_gameControl->GetUserPlayer()) return;

	if (mouseButton == Qt::LeftButton)
	{
		cardPic->SetSelected(!cardPic->IsSelected());
		UpdatePlayerCards(cardPic->GetOwner());

		QSet<CardPic*>::Iterator it = m_selectedCards.find(cardPic);
		if (it == m_selectedCards.end())		// ѡ����
		{
			m_selectedCards.insert(cardPic);
		}
		else	// ȡ��ѡ����
		{
			m_selectedCards.erase(it);
		}
	}
	else if (mouseButton == Qt::RightButton)	// �һ�����
	{
		OnUserPlayHand();
	}
}

void GamePanel::HidePlayerLastCards(Player* player)
{
	QMap<Player*, PlayerContext>::Iterator it = m_playerContextMap.find(player);
	if (it != m_playerContextMap.end())
	{
		if (it->lastCards.IsEmpty())	// ��һ�δ�Ŀ��ƣ���pass
		{
			it->info->hide();
		}
		else
		{
			CardList lastCardList = it->lastCards.ToCardList();
			CardList::ConstIterator itLast = lastCardList.constBegin();
			for (; itLast != lastCardList.constEnd(); itLast++)
			{
				m_cardPicMap[*itLast]->hide();
			}
		}
	}
}

void GamePanel::UpdatePlayerCards(Player* player)
{
	Cards restCards = player->GetCards();
	CardList restCardList = restCards.ToCardList(Cards::Desc);

	const int cardSpacing = 20;		// �Ƽ��

	// ��ʾʣ�µ���
	QRect cardsRect = m_playerContextMap[player].cardsRect;
	CardList::ConstIterator itRest = restCardList.constBegin();
	for (int i = 0; itRest != restCardList.constEnd(); itRest++, i++)
	{
		CardPic* cardPic = m_cardPicMap[*itRest];
		cardPic->SetFrontSide(m_playerContextMap[player].isFrontSide);
		cardPic->show();
		cardPic->raise();

		if (m_playerContextMap[player].cardsAlign == Horizontal)
		{
			int leftBase = cardsRect.left() + (cardsRect.width() - (restCardList.size() - 1) * cardSpacing - cardPic->width()) / 2;
			int top = cardsRect.top() + (cardsRect.height() - cardPic->height()) / 2;
			if (cardPic->IsSelected()) top -= 10;
			
			cardPic->move(leftBase + i * cardSpacing, top);
		}
		else
		{
			int left = cardsRect.left() + (cardsRect.width() - cardPic->width()) / 2;
			if (cardPic->IsSelected()) left += 10;
			int topBase = cardsRect.top() + (cardsRect.height() - (restCardList.size() - 1) * cardSpacing - cardPic->height()) / 2;
			cardPic->move(left, topBase + i * cardSpacing);
		}
	}

	// ��ʾ��һ�δ��ȥ����
	QRect playCardsRect = m_playerContextMap[player].playHandRect;
	if (!m_playerContextMap[player].lastCards.IsEmpty())		// ���ǿ���
	{
		int playSpacing = 24;
		CardList lastCardList = m_playerContextMap[player].lastCards.ToCardList();
		CardList::ConstIterator itPlayed = lastCardList.constBegin();
		for (int i = 0; itPlayed != lastCardList.constEnd(); itPlayed++, i++)
		{
			CardPic* cardPic = m_cardPicMap[*itPlayed];
			cardPic->SetFrontSide(true);
			cardPic->raise();

			if (m_playerContextMap[player].cardsAlign == Horizontal)
			{
				int leftBase = playCardsRect.left () + 
					(playCardsRect.width() - (lastCardList.size() - 1) * playSpacing - cardPic->width()) / 2;
				int top = playCardsRect.top() + (playCardsRect.height() - cardPic->height()) / 2;
				cardPic->move(leftBase + i * playSpacing, top);
			}
			else
			{
				int left = playCardsRect.left() + (playCardsRect.width() - cardPic->width()) / 2;
				int topBase = playCardsRect.top();
				cardPic->move(left, topBase + i * playSpacing);
			}
		}
	}
}

void GamePanel::OnPlayerCalledLord(Player* player, int bet)
{
	QString betInfo;
	if (bet == 0)
	{
		betInfo = tr("bet0");
	}
	else if (bet == 1)
	{
		betInfo = tr("bet1");
	}
	else if (bet == 2)
	{
		betInfo = tr("bet2");
	}
	else if (bet == 3)
	{
		betInfo = tr("bet3");
	}

	QLabel* infoLabel = m_playerContextMap[player].info;
	infoLabel->setText(betInfo);
	infoLabel->show();

	// �û�����֣����ؽзְ�ť
	if (player == m_gameControl->GetUserPlayer())
	{
		m_bet0Button->hide();
		m_bet1Button->hide();
		m_bet2Button->hide();
		m_bet3Button->hide();
	}
}

void GamePanel::OnPlayerPlayHand(Player* player, const Cards& cards)
{
	HidePlayerLastCards(player);

	// ��¼��һ�δ����
	QMap<Player*, PlayerContext>::Iterator itContext = m_playerContextMap.find(player);
	itContext->lastCards = cards;

	// ����ƣ���ʾ����Ҫ��
	if (cards.IsEmpty())
	{
		itContext->info->setText(tr("label_pass"));
		itContext->info->show();
	}

	UpdatePlayerCards(player);
}

void GamePanel::OnPlayerPickCards(Player* player, const Cards& cards)
{
	CardList cardList = cards.ToCardList();
	CardList::ConstIterator it = cardList.constBegin();
	for (; it != cardList.constEnd(); it++)
	{
		CardPic* pickedCard = m_cardPicMap[*it];
		pickedCard->SetOwner(player);
	}

	UpdatePlayerCards(player);
}

void GamePanel::OnUserNoBet()
{
	m_gameControl->GetUserPlayer()->CallLord(0);
}

void GamePanel::OnUserBet1()
{
	m_gameControl->GetUserPlayer()->CallLord(1);
}

void GamePanel::OnUserBet2()
{
	m_gameControl->GetUserPlayer()->CallLord(2);
}

void GamePanel::OnUserBet3()
{
	m_gameControl->GetUserPlayer()->CallLord(3);
}

void GamePanel::OnUserPass()
{
	if (m_gameControl->GetCurrentPlayer() != m_gameControl->GetUserPlayer()) return;

	Player* punchPlayer = m_gameControl->GetPunchPlayer();
	if (punchPlayer == m_gameControl->GetUserPlayer() || punchPlayer == NULL) return;

	m_gameControl->GetUserPlayer()->PlayHand(Cards());

	QSet<CardPic*>::ConstIterator it = m_selectedCards.constBegin();
	for (; it != m_selectedCards.constEnd(); it++)
	{
		(*it)->SetSelected(false);
	}
	m_selectedCards.clear();

	UpdatePlayerCards(m_gameControl->GetUserPlayer());
}

void GamePanel::OnUserPlayHand()
{
	if (m_gameStatus != GameControl::PlayingHandStatus)
	{
		return;
	}

	if (m_gameControl->GetCurrentPlayer() != m_gameControl->GetUserPlayer())
	{
		return;
	}

	if (m_selectedCards.isEmpty()) return;

	Cards playCards;
	QSet<CardPic*>::const_iterator it = m_selectedCards.constBegin();
	for (; it != m_selectedCards.constEnd(); it++)
	{
		playCards.Add((*it)->GetCard());
	}
	
	Hand hand(playCards);
	HandType ht = hand.GetType();
	CardPoint bp = hand.GetBasePoint();
	int ex = hand.GetExtra();

	if (ht == Hand_Unknown)		// δ֪���ͣ��������
	{
		return;
	}

	// �ܲ�ס�����˵���
	if (m_gameControl->GetPunchPlayer() != m_gameControl->GetUserPlayer())
	{
		if (!hand.CanBeat(Hand(m_gameControl->GetPunchCards())))
		{
			return;
		}
	}

	m_gameControl->GetUserPlayer()->PlayHand(playCards);
	m_selectedCards.clear();
}

void GamePanel::OnStartBtnClicked()
{
	m_startButton->hide();

	m_gameControl->ClearPlayerMarks();
	UpdateMarkBoard();

	ProcessGameStatus(GameControl::PickingCardStatus);
}

void GamePanel::OnContinueBtnClicked()
{
	m_continueButton->hide();
	ProcessGameStatus(GameControl::PickingCardStatus);
}

void GamePanel::StartPickCard()
{
	QMap<Card, CardPic*>::const_iterator it = m_cardPicMap.constBegin();
	for (; it != m_cardPicMap.constEnd(); it++)
	{
		it.value()->SetSelected(false);
		it.value()->SetFrontSide(true);
		it.value()->hide();
	}

	for (int i = 0; i < m_restThreeCards.size(); i++)
	{
		m_restThreeCards[i]->hide();
	}

	m_selectedCards.clear();
	m_playerContextMap[m_gameControl->GetLeftRobot()].lastCards.Clear();
	m_playerContextMap[m_gameControl->GetRightRobot()].lastCards.Clear();
	m_playerContextMap[m_gameControl->GetUserPlayer()].lastCards.Clear();

	m_playerContextMap[m_gameControl->GetLeftRobot()].info->hide();
	m_playerContextMap[m_gameControl->GetRightRobot()].info->hide();
	m_playerContextMap[m_gameControl->GetUserPlayer()].info->hide();

	m_playerContextMap[m_gameControl->GetLeftRobot()].isFrontSide = false;
	m_playerContextMap[m_gameControl->GetRightRobot()].isFrontSide = false;
	m_playerContextMap[m_gameControl->GetUserPlayer()].isFrontSide = true;

	m_playerContextMap[m_gameControl->GetLeftRobot()].rolePic->hide();
	m_playerContextMap[m_gameControl->GetRightRobot()].rolePic->hide();
	m_playerContextMap[m_gameControl->GetUserPlayer()].rolePic->hide();

	m_gameControl->Clear();

	m_baseCard->show();

	m_bet0Button->hide();
	m_bet1Button->hide();
	m_bet2Button->hide();
	m_bet3Button->hide();

	m_playHandButton->hide();
	m_passButton->hide();

	m_pickCardTimer->start();
}

void GamePanel::ProcessGameStatus(GameControl::GameStatus gameStatus)
{
	m_gameStatus = gameStatus;

	if (gameStatus == GameControl::PickingCardStatus)
	{
		StartPickCard();
	}
	else if (gameStatus == GameControl::CallingLordStatus)
	{
		CardList restThreeCards = m_gameControl->GetRestCards().ToCardList();

		for (int i = 0; i < m_restThreeCards.size(); i++)
		{
			m_restThreeCards[i]->SetPic(m_cardPicMap[restThreeCards[i]]->GetPic(), m_cardBackPic);
			m_restThreeCards[i]->hide();
		}

		m_gameControl->StartCallLord();
	}
	else if (gameStatus == GameControl::PlayingHandStatus)
	{
		m_baseCard->hide();
		m_movingCard->hide();

		for (int i = 0; i < m_restThreeCards.size(); i++)
		{
			m_restThreeCards[i]->show();
		}

		QMap<Player*, PlayerContext>::ConstIterator it = m_playerContextMap.constBegin();
		for (; it != m_playerContextMap.constEnd(); it++)
		{
			it.value().info->hide();

			if (it.key()->GetRole() == Player::Lord)
			{
				it.value().rolePic->setPixmap(QPixmap(":/res/lord.png"));
				it.value().rolePic->show();
			}
			else
			{
				it.value().rolePic->setPixmap(QPixmap(":/res/farmer.png"));
				it.value().rolePic->show();
			}
		}
	}
}

void GamePanel::OnPlayerStatus(Player* player, GameControl::PlayerStatus playerStatus)
{
	if (playerStatus == GameControl::ThinkingForCallLordStatus)
	{
		if (player == m_gameControl->GetUserPlayer())	// �û��з֣���ʾ�зְ�ť
		{
			m_bet0Button->show();
			m_bet1Button->show();
			m_bet2Button->show();
			m_bet3Button->show();
		}
	}
	else if (playerStatus == GameControl::ThinkingForPlayHandStatus)
	{
		HidePlayerLastCards(player);

		if (player == m_gameControl->GetUserPlayer())
		{
			m_playHandButton->show();

			Player* punchPlayer = m_gameControl->GetPunchPlayer();
			if (punchPlayer == m_gameControl->GetUserPlayer() || punchPlayer == NULL)
			{
				m_passButton->hide();
			}
			else
			{
				m_passButton->show();
			}
		}
		else
		{
			m_playHandButton->hide();
			m_passButton->hide();
		}
	}
	else if (m_gameStatus == GameControl::WinningStatus)
	{
		m_playerContextMap[m_gameControl->GetLeftRobot()].isFrontSide = true;
		m_playerContextMap[m_gameControl->GetRightRobot()].isFrontSide = true;
		UpdatePlayerCards(m_gameControl->GetLeftRobot());
		UpdatePlayerCards(m_gameControl->GetRightRobot());

		UpdateMarkBoard();

		QString str = tr("%1 wins the game.");
		str = str.arg(player->GetName());
		QMessageBox::information(this, tr("tip"), str);

		m_playHandButton->hide();
		m_passButton->hide();
		m_continueButton->show();

		// Ӯ����һ���Ƚз�
		m_gameControl->SetCurrentPlayer(player);
	}
}

