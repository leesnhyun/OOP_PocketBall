#ifndef _CRIGHTWALL_
#define _CRIGHTWALL_
#include "CWall.h"

class CRightWall : public CWall
{
public:
	CRightWall(float iwidth, float iheight, float idepth, D3DXCOLOR color);
	bool hasIntersected(CSphere& ball) const noexcept;
	void hitBy(CSphere& ball) noexcept;
};
#endif