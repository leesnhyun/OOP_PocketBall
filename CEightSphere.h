#ifndef _CEIGHTSPHERE_
#define _CEIGHTSPHERE_
#include "CSphere.h"
#include "Removable.h"

class CEightSphere : public CSphere, public Removable
{
private:
	int disableTurn;
public:
	CEightSphere(const char * ballImageFileName);
	void disable() noexcept;
	int getDisableTurn() const noexcept;
	bool isDisabled() const noexcept;
};
#endif