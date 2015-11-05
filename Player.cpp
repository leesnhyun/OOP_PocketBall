#include "Player.h"

bool Player::isFirstPlayerTurn = true;
targetBall Player::FirstPlayerTargetBall = NONE;
Player::Player()
{
	numTakenBall = 0;
}

void Player::addTakenBall(unsigned int numBall)
{
	this->numTakenBall += numTakenBall;
}

unsigned Player::getNumTakenBall() const
{
	return numTakenBall;
}
