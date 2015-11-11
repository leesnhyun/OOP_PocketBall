#ifndef _CEIGHTSPHERE_
#define _CEIGHTSPHERE_
#include "CSphere.h"
#include "Removable.h"

class CEightSphere : public CSphere, public Removable
{
public:
	CEightSphere(const char * ballImageFileName);
	bool hasIntersected(CSphere& ball) const noexcept;
	void hitBy(CSphere& ball) noexcept;
	void disable() noexcept;
	int getDisableTurn() const noexcept;
	bool isDisabled() const noexcept;
};
#endif