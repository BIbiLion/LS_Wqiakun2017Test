#pragma once
#include "Player.h"

class Robot : public Player
{
	Q_OBJECT
public:
	Robot(QObject* parent = NULL);
	virtual ~Robot();

public:
	virtual void StartCallLord();
	virtual void StartPlayHand();

	virtual void ThinkForCallLord();
	virtual void ThinkForPlayHand();

};