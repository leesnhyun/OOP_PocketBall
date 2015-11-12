#ifndef _CHANDSPHERE_
#define _CHANDSPHERE_
#include "CSphere.h"

class CHandSphere : public CSphere
{
private:
	BallType firstHitBallType;
public:
	CHandSphere(const char * ballImageFileName);
	BallType getFirstHitBallType() const noexcept;
	void setFirstHitBallType(BallType ballType) noexcept;
	void hitBy(CSphere& ball) noexcept;
};
#endif