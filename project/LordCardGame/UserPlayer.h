#pragma once
#include "Player.h"

class UserPlayer : public Player
{
	Q_OBJECT
public:
	UserPlayer(QObject* parent = NULL);
	virtual ~UserPlayer();

public:
	virtual void StartCallLord();
	virtual void StartPlayHand();

};