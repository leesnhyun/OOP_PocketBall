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

void CHandSphere::hitBy(CSphere& ball) noexcept
{
	if (this->hasIntersected(ball))
	{
		if (this->firstHitBallType == BallType::NONE)
		{
			this->firstHitBallType = ball.getBallType();
		}

		super::hitBy(ball);
	}
}