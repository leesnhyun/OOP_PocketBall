#ifndef _CLEFTWALL_
#define _CLEFTWALL_
#include "CWall.h"

class CLeftWall : public CWall
{
public:
	CLeftWall(float iwidth, float iheight, float idepth, D3DXCOLOR color);
	bool hasIntersected(CSphere& ball) const noexcept;
	void hitBy(CSphere& ball) noexcept;
};
#endif