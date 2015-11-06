#include "Player.h"

/* Static Definition Area */
bool Player::isFirstPlayerTurn = true;
TargetBall Player::FirstPlayerTargetBall = TargetBall::NONE;

bool Player::IsFirstPlayerTurn()
{
	return Player::isFirstPlayerTurn;
}


/* Instance Definition Area */
Player::Player(unsigned int playerID)
{
	this->playerID = playerID;
	this->numTakenBall = 0;
}

bool Player::isAppropriatePlayer()
{
	if ((this->playerID == 1 && Player::isFirstPlayerTurn) ||
		(this->playerID == 2 && !Player::isFirstPlayerTurn))
	{
		return true;
	}

	return false;
}

void Player::addTakenBall(unsigned int numBall)
{
	this->numTakenBall += numTakenBall;
}

unsigned int Player::getNumTakenBall() const
{
	return numTakenBall;
}