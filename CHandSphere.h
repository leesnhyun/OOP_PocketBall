#ifndef _CHANDSPHERE_
#define _CHANDSPHERE_
#include "CSphere.h"

class CHandSphere : public CSphere
{
private:
	BallType firstHitBallType;
public:
	CHandSphere(const char* ballImageFileName);
	BallType getFirstHitBallType() const noexcept; // 처음 충돌한 공의 종류 반환
	void setFirstHitBallType(BallType ballType) noexcept; // 처음 충돌한 공의 종류 설정
	void hitBy(CSphere& ball) noexcept; // 충돌 후, 만약 처음 충돌한 공이라면 그 공의 종류를 저장
};
#endif

