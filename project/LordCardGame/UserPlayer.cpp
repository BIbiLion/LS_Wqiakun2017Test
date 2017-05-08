#include "UserPlayer.h"

UserPlayer::UserPlayer(QObject* parent)
	: Player(parent)
{
	m_type = Player::User;
}

UserPlayer::~UserPlayer()
{

}

void UserPlayer::StartCallLord()
{
	//CallLord(3);
}

void UserPlayer::StartPlayHand()
{

}
