#ifndef _CSTRIPESPHERE_
#define _CSTRIPESPHERE_
#include "CSphere.h"
#include "Removable.h"

class CStripeSphere : public CSphere, public Removable
{
private:
	int disableTurn;
public:
	CStripeSphere(const char * ballImageFileName);
	void disable() noexcept;
	int getDisableTurn() const noexcept;
	bool isDisabled() const noexcept;
};
#endif