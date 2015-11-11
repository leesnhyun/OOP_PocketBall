#ifndef _CSOLIDSPHERE_
#define _CSOLIDSPHERE_
#include "CSphere.h"
#include "Removable.h"

class CSolidSphere : public CSphere, public Removable
{
private:
	int disableTurn;
public:
	CSolidSphere(const char * ballImageFileName);
	void disable() noexcept;
	int getDisableTurn() const noexcept;
	bool isDisabled() const noexcept;
};
#endif