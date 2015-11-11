#ifndef _CTOPWALL_
#define _CTOPWALL_
#include "CWall.h"

class CTopWall : public CWall
{
public:
	CTopWall(float iwidth, float iheight, float idepth, D3DXCOLOR color);
	bool hasIntersected(CSphere& ball) const noexcept;
	void hitBy(CSphere& ball) noexcept;
};
#endif