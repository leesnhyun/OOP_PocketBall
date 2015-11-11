#ifndef _CTARGETSPHERE_
#define _CTARGETSPHERE_
#include "CSphere.h"
#include "Removable.h"

class CTargetSphere : public CSphere
{
private:
	int disableTurn;
public:
	CTargetSphere(const char * ballImageFileName);
};
#endif