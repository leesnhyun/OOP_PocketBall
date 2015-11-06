#include "Player.h"

/* Static Definition Area */
TargetBall Player::FirstPlayerTargetBall = TargetBall::NONE;

/* Instance Definition Area */
Player::Player(unsigned int playerID)
{
	this->playerID = playerID;
	this->numTakenBall = 0;
}

void Player::addTakenBall(unsigned int numBall)
{
	this->numTakenBall += numTakenBall;
}

unsigned int Player::getNumTakenBall() const
{
	return numTakenBall;
}