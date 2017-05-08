#ifndef CARDPIC_H
#define CARDPIC_H

#include <QFrame>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPixmap>
#include "Cards.h"

class Player;

class CardPic : public QFrame
{
	Q_OBJECT

public:
	CardPic(QWidget *parent);
	~CardPic();

	void SetPic(const QPixmap& pic, const QPixmap& back);
	QPixmap GetPic();

	void SetOwner(Player* owner);
	Player* GetOwner();

	void SetCard(Card card);
	Card GetCard();

	void SetFrontSide(bool frontSide);
	bool IsFrontSide();

	void SetSelected(bool selected);
	bool IsSelected();

signals:
	void NotifySelected(Qt::MouseButton);

protected:
	virtual void paintEvent(QPaintEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);

protected:
	QPixmap m_pic;
	QPixmap m_back;
	Player* m_owner;
	Card m_card;
	bool m_selected;

	bool m_frontSide;
};

#endif // CARDPIC_H
