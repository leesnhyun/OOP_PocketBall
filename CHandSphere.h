#ifndef _CHANDSPHERE_
#define _CHANDSPHERE_
#include "CSphere.h"

class CHandSphere : public CSphere
{
public:
	CHandSphere(const char * ballImageFileName);
	bool hasIntersected(CSphere& ball) const noexcept;
	void hitBy(CSphere& ball) noexcept;
};
#endif