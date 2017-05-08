#pragma once

#include "Hand.h"
#include <QVector>

class Cards;
class Player;

class Strategy
{
public:
	Strategy(Player* player, const Cards& cards);
	~Strategy();

	Cards MakeStrategy();

public:
	Cards FindSamePointCards(CardPoint point, int count);	// �ҳ�count�ŵ���Ϊpoint����
	QVector<Cards> FindCardsByCount(int count);		// �ҳ���������Ϊcount������������
	Cards GetRangeCards(CardPoint beginPoint, CardPoint endPoint);	// ���ݵ�����Χ����
	Cards GetFirstSeqSingleRange();		// �ҵ���һ���ܹ���˳�ӵ��Ƽ���

	// ��cards������һ��˳�ӣ�seqInherited��¼�𲽲���˳�ӣ�allSeqRecord��¼���ܷ������˳��
	void PickSeqSingles(QVector<QVector<Cards> >& allSeqRecord, const QVector<Cards>& seqInherited, Cards cards);
	QVector<Cards> PickOptimalSeqSingles();

	// �������ҵ��������ƣ�beatΪtrue��ʾҪ���hand
	QVector<Cards> FindHand(Hand hand, bool beat);

	Cards PlayFirst();		// ��������
	Cards PlayBeatHand(Hand hand);		// ���ָ������

	bool WhetherToBeat(const Cards& myCards);	// �ܴ��ʱ���ж��Ǵ��Ƿ���
	
protected:
	Player* m_player;
	Cards m_cards;
};
