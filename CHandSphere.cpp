// TODO : Implementation
#include "CHandSphere.h"

CHandSphere::CHandSphere(const char * ballImageFileName) : CSphere(BallType::HAND, ballImageFileName)
{
	this->firstHitBallType = BallType::NONE;
}

BallType CHandSphere::getFirstHitBallType() const noexcept
{
	return this->firstHitBallType;
}

void CHandSphere::setFirstHitBallType(BallType ballType) noexcept
{
	this->firstHitBallType = ballType;
}