#include "Player.h"

/* Instance Definition Area */
Player::Player(unsigned int playerID)
{
	this->playerID = playerID;
	this->numTakenBall = 0;
	this->playerBallType = BallType::NONE;
}

void Player::addTakenBall(unsigned int numBall)
{
	this->numTakenBall += numTakenBall;
}

unsigned int Player::getNumTakenBall() const
{
	return numTakenBall;
}

unsigned int Player::getPlayerId() const
{
	return this->playerID;
}