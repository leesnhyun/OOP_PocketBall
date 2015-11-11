#ifndef _CBOTTOMWALL_
#define _CBOTTOMWALL_
#include "CWall.h"

class CBottomWall : public CWall
{
public:
	CBottomWall(float iwidth, float iheight, float idepth, D3DXCOLOR color);
	bool hasIntersected(CSphere& ball) const noexcept;
	void hitBy(CSphere& ball) noexcept;
};
#endif