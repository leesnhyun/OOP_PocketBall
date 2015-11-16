#ifndef _CHANDSPHERE_
#define _CHANDSPHERE_
#include "CSphere.h"

class CHandSphere : public CSphere
{
private:
	BallType firstHitBallType;
public:
	CHandSphere(const char* ballImageFileName);
	BallType getFirstHitBallType() const noexcept; // ó�� �浹�� ���� ���� ��ȯ
	void setFirstHitBallType(BallType ballType) noexcept; // ó�� �浹�� ���� ���� ����
	void hitBy(CSphere& ball) noexcept; // �浹 ��, ���� ó�� �浹�� ���̶�� �� ���� ������ ����
};
#endif

