#pragma once
#include <QVector>
#include <QSet>
#include <QMap>
#include <QList>

class Hand;

enum CardPoint
{
	Card_Begin,

	Card_3,
	Card_4,
	Card_5,
	Card_6,
	Card_7,
	Card_8,
	Card_9,
	Card_10,
	Card_J,
	Card_Q,
	Card_K,
	Card_A,
	Card_2,

	Card_SJ,
	Card_BJ,

	Card_End
};

const int PointCount = 15;

enum CardSuit
{
	Suit_Begin,

	Diamond,	// ����
	Club,		// ÷��
	Heart,		// ����
	Spade,		// ���� 

	Suit_End
};

enum HandType
{
	Hand_Unknown,			// δ֪
	Hand_Pass,				// ��

	Hand_Single,			// ��
	Hand_Pair,				// ��

	Hand_Triple,			// ����
	Hand_Triple_Single,		// ����һ
	Hand_Triple_Pair,		// ������

	Hand_Plane,				// �ɻ���555_666
	Hand_Plane_Two_Single,	// �ɻ�������555_666_3_4
	Hand_Plane_Two_Pair,	// �ɻ���˫��555_666_33_44

	Hand_Seq_Pair,			// ���ԣ�33_44_55(_66...)
	Hand_Seq_Single,		// ˳�ӣ�34567(8...)

	Hand_Bomb,				// ը��
	Hand_Bomb_Single,		// ը����һ��
	Hand_Bomb_Pair,			// ը����һ��
	Hand_Bomb_Two_Single,	// ը��������

	Hand_Bomb_Jokers,			// ��ը
	Hand_Bomb_Jokers_Single,	// ��ը��һ��
	Hand_Bomb_Jokers_Pair,		// ��ը��һ��
	Hand_Bomb_Jokers_Two_Single	// ��ը������

};

struct Card
{
	CardPoint point;
	CardSuit suit;
};

inline bool operator ==(const Card& left, const Card& right)
{
	return (left.point == right.point && left.suit == right.suit);
}

inline bool operator <(const Card& left, const Card& right)
{
	if (left.point == right.point)
	{
		return left.suit < right.suit;
	}
	else
	{
		return left.point < right.point;
	}
}


inline uint qHash(const Card& card)
{
	return card.point * 100 + card.suit;
}

typedef QVector<Card> CardList;

class Cards
{
public:
	Cards();
	~Cards();

public:
	enum SortType
	{
		NoSort,
		Asc,
		Desc
	};

	void Add(const Cards& cards);
	void Add(const Card& card);
	void Add(const QVector<Cards>& cardsArray);

	void Remove(const Cards& cards);
	void Remove(const Card& card);
	void Remove(const QVector<Cards>& cardsArray);

	bool Contains(const Cards& cards) const;
	bool Contains(const Card& card) const;

	Cards& operator <<(const Cards& cards);
	Cards& operator <<(const Card& card);
	Cards& operator <<(const QVector<Cards>& cardsArray);

	CardList ToCardList(SortType sortType = Asc) const;

	int Count();
	int PointCount(CardPoint point);
	CardPoint MinPoint();
	CardPoint MaxPoint();
	
	Card TakeRandomCard();

	bool IsEmpty() const;
	void Clear();

protected:
	QSet<Card> m_cards;
};
