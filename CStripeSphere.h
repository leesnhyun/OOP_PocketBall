#ifndef _CSTRIPESPHERE_
#define _CSTRIPESPHERE_
#include "CSphere.h"
#include "Removable.h"

class CStripeSphere : public CSphere, public Removable
{
public:
	CStripeSphere(const char * ballImageFileName);
	bool hasIntersected(CSphere& ball) const noexcept;
	void hitBy(CSphere& ball) noexcept;
	void disable() noexcept;
	int getDisableTurn() const noexcept;
	bool isDisabled() const noexcept;
};
#endif