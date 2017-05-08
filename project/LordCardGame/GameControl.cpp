#include "GameControl.h"
#include <QMetaType>
#include "Hand.h"
#include <QMessageBox>
#include <QtNetwork/QAbstractSocket>


GameControl::GameControl(QObject* parent)
	: QObject(parent)
{
	qRegisterMetaType<Hand>("Hand");
	qRegisterMetaType<Card>("Card");
	qRegisterMetaType<Cards>("Cards");

	m_punchPlayer = NULL;
	m_punchCards = Cards();

	m_curBet = 0;
}

GameControl::~GameControl()
{

}

void GameControl::Init()
{
	m_robotLeft = new Robot(this);
	m_robotRight = new Robot(this);
	m_user = new UserPlayer(this);

	m_robotLeft->SetName(tr("left robot"));
	m_robotRight->SetName(tr("right robot"));
	m_user->SetName(tr("me"));

	// ��������ҶԽ�
	connect(m_robotLeft, SIGNAL(NotifyCallLord(Player*, int)), this, SLOT(OnPlayerCalledLord(Player*, int)));
	connect(m_robotRight, SIGNAL(NotifyCallLord(Player*, int)), this, SLOT(OnPlayerCalledLord(Player*, int)));
	connect(m_user, SIGNAL(NotifyCallLord(Player*, int)), this, SLOT(OnPlayerCalledLord(Player*, int)));

	connect(m_robotLeft, SIGNAL(NotifyPlayHand(Player*, const Cards&)), this, SLOT(OnPlayerPlayedHand(Player*, const Cards&)));
	connect(m_robotRight, SIGNAL(NotifyPlayHand(Player*, const Cards&)), this, SLOT(OnPlayerPlayedHand(Player*, const Cards&)));
	connect(m_user, SIGNAL(NotifyPlayHand(Player*, const Cards&)), this, SLOT(OnPlayerPlayedHand(Player*, const Cards&)));

	connect(m_robotLeft, SIGNAL(NotifyPickCards(Player*, const Cards&)), this, SLOT(OnPlayerPickCards(Player*, const Cards&)));
	connect(m_robotRight, SIGNAL(NotifyPickCards(Player*, const Cards&)), this, SLOT(OnPlayerPickCards(Player*, const Cards&)));
	connect(m_user, SIGNAL(NotifyPickCards(Player*, const Cards&)), this, SLOT(OnPlayerPickCards(Player*, const Cards&)));

	connect(this, SIGNAL(NotifyPlayerPunch(Player*, const Cards&)), 
		m_robotLeft, SLOT(OnPlayerPunch(Player*, const Cards&)));
	connect(this, SIGNAL(NotifyPlayerPunch(Player*, const Cards&)), 
		m_robotRight, SLOT(OnPlayerPunch(Player*, const Cards&)));
	connect(this, SIGNAL(NotifyPlayerPunch(Player*, const Cards&)), 
		m_user, SLOT(OnPlayerPunch(Player*, const Cards&)));

	m_robotLeft->SetPrevPlayer(m_robotRight);
	m_robotLeft->SetNextPlayer(m_user);
	m_robotRight->SetPrevPlayer(m_user);
	m_robotRight->SetNextPlayer(m_robotLeft);
	m_user->SetPrevPlayer(m_robotLeft);
	m_user->SetNextPlayer(m_robotRight);

	m_curPlayer = m_user;
}

void GameControl::AddCard(CardSuit suit, CardPoint point)
{
	Card card;
	card.suit = suit;
	card.point = point;

	m_allCards.Add(card);
}

void GameControl::InitCards()
{
	m_allCards.Clear();

	for (int p = Card_Begin + 1; p < Card_SJ; p++)
	{
		for (int s = Suit_Begin + 1; s < Suit_End; s++)
		{
			AddCard((CardSuit)s, (CardPoint)p);
		}
	}

	AddCard(Suit_Begin, Card_SJ);
	AddCard(Suit_Begin, Card_BJ);
}

void GameControl::ShuffleCards()
{
	
}

void GameControl::StartCallLord()
{
	m_betList.clear();

	emit NotifyPlayerStatus(m_curPlayer, GameControl::ThinkingForCallLordStatus);
	m_curPlayer->StartCallLord();
}

void GameControl::OnPlayerCalledLord(Player* player, int bet)
{
	NotifyPlayerCalledLord(player, bet);

	if (bet == 3)	// ��3�֣�ֱ�ӵ�����
	{
		m_curBet = 3;

		player->SetRole(Player::Lord);
		player->GetPrevPlayer()->SetRole(Player::Farmer);
		player->GetNextPlayer()->SetRole(Player::Farmer);

		m_curPlayer = player;
		player->PickCards(m_allCards);
		m_allCards.Clear();

		// �ӳ�һ�£���ʼ���ƹ���
		QTimer::singleShot(1000, this, SLOT(StartPlayHand()));
		
		return;
	}

	BetRecrod record;
	record.player = player;
	record.bet = bet;
	m_betList.push_back(record);

	// ���ݽз�������ֵ���ũ���ɫ
	if (m_betList.size() == 3)	// ���˶�����һ��
	{
		if (m_betList[0].bet == m_betList[1].bet && 
			m_betList[1].bet == m_betList[2].bet)		// ���˽з�һ�������·���
		{
			emit NotifyGameStatus(PickingCardStatus);
		}
		else
		{
			BetRecrod record;
			if (m_betList[0].bet >= m_betList[1].bet)
			{
				record = m_betList[0];
			}
			else
			{
				record = m_betList[1];
			}

			if (record.bet < m_betList[2].bet)
			{
				record = m_betList[2];
			}

			record.player->SetRole(Player::Lord);
			record.player->GetPrevPlayer()->SetRole(Player::Farmer);
			record.player->GetNextPlayer()->SetRole(Player::Farmer);
			m_curPlayer = record.player;

			m_curBet = record.bet;

			m_curPlayer->PickCards(m_allCards);
			m_allCards.Clear();

			QTimer::singleShot(1000, this, SLOT(StartPlayHand()));
		}

		return;
	}

	// ����һ����ҽз�
	Player* nextPlayer = player->GetNextPlayer();
	m_curPlayer = nextPlayer;
	emit NotifyPlayerStatus(m_curPlayer, GameControl::ThinkingForCallLordStatus);
	m_curPlayer->StartCallLord();
}

void GameControl::StartPlayHand()
{
	emit NotifyGameStatus(PlayingHandStatus);

	emit NotifyPlayerStatus(m_curPlayer, GameControl::ThinkingForPlayHandStatus);
	m_curPlayer->StartPlayHand();
}

void GameControl::OnPlayerPlayedHand(Player* player, const Cards& cards)
{
	if (!cards.IsEmpty())
	{
		m_punchPlayer = player;
		m_punchCards = cards;

		NotifyPlayerPunch(m_punchPlayer, m_punchCards);
	}

	// ��ը���׷ַ���
	HandType handType = Hand(cards).GetType();
	if (handType == Hand_Bomb || handType == Hand_Bomb_Jokers)
	{
		m_curBet = m_curBet * 2;
	}

	emit NotifyPlayerPlayHand(m_curPlayer, cards);

	// player�Ѱ��ƶ����꣬���������÷�
	if (player->GetCards().IsEmpty())
	{
		Player* prev = player->GetPrevPlayer();
		Player* next = player->GetNextPlayer();

		if (player->GetRole() == Player::Lord)
		{
			player->SetMark(player->GetMark() + 2 * m_curBet);
			prev->SetMark(prev->GetMark() - m_curBet);
			next->SetMark(next->GetMark() - m_curBet);
		}
		else
		{
			player->SetMark(player->GetMark() + m_curBet);

			if (prev->GetRole() == Player::Lord)
			{
				prev->SetMark(prev->GetMark() - 2 * m_curBet);
				next->SetMark(next->GetMark() + m_curBet);
			}
			else
			{
				prev->SetMark(prev->GetMark() + m_curBet);
				next->SetMark(next->GetMark() - 2 * m_curBet);
			}
		}
		
		emit NotifyPlayerStatus(player, GameControl::WinningStatus);
		return;
	}

	// �����ƣ��ֵ���һ�����
	Player* nextPlayer = player->GetNextPlayer();
	m_curPlayer = nextPlayer;
	emit NotifyPlayerStatus(m_curPlayer, GameControl::ThinkingForPlayHandStatus);
	m_curPlayer->StartPlayHand();
}

void GameControl::OnPlayerPickCards(Player* player, const Cards& cards)
{
	emit NotityPlayerPickCards(player, cards);
}

void GameControl::Clear()
{
	InitCards();

	m_robotLeft->ClearCards();
	m_robotRight->ClearCards();
	m_user->ClearCards();

	m_punchPlayer = NULL;
	m_punchCards.Clear();
	NotifyPlayerPunch(m_punchPlayer, m_punchCards);

	m_betList.clear();
}

Player* GameControl::GetUserPlayer()
{
	return m_user;
}

Player* GameControl::GetLeftRobot()
{
	return m_robotLeft;
}

Player* GameControl::GetRightRobot()
{
	return m_robotRight;
}

Player* GameControl::GetCurrentPlayer()
{
	return m_curPlayer;
}

void GameControl::SetCurrentPlayer(Player* curPlayer)
{
	m_curPlayer = curPlayer;
}

Cards GameControl::GetRestCards()
{
	return m_allCards;
}

Card GameControl::PickOneCard()
{
	Card card = m_allCards.TakeRandomCard();
	return card;
}

Player* GameControl::GetPunchPlayer()
{
	return m_punchPlayer;
}

Cards GameControl::GetPunchCards()
{
	return m_punchCards;
}

int GameControl::GetCurBet()
{
	return m_curBet;
}

void GameControl::SetCurBet(int curBet)
{
	m_curBet = curBet;
}

void GameControl::ClearPlayerMarks()
{
	m_robotLeft->SetMark(0);
	m_robotRight->SetMark(0);
	m_user->SetMark(0);
}
