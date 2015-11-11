#ifndef _CSOLIDSPHERE_
#define _CSOLIDSPHERE_
#include "CSphere.h"
#include "Removable.h"

class CSolidSphere : public CSphere, public Removable
{
public:
	CSolidSphere(const char * ballImageFileName);
	bool hasIntersected(CSphere& ball) const noexcept;
	void hitBy(CSphere& ball) noexcept;
	void disable() noexcept;
	int getDisableTurn() const noexcept;
	bool isDisabled() const noexcept;
};
#endif