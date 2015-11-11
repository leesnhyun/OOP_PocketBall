// TODO : Implementaion
#include "CBottomWall.h"

CBottomWall::CBottomWall(float iwidth, float iheight, float idepth, D3DXCOLOR color)
: CWall(iwidth, iheight, idepth, color)
{

}

bool CBottomWall::hasIntersected(CSphere& ball) const noexcept
{
	if (ball.getPosition().x > this->center_x - (this->m_width / 2) &&
	ball.getPosition().x < this->center_x + (this->m_width / 2))
	{
		if (ball.getPosition().z - ball.getRadius() < this->center_z + (this->m_depth / 2))
		{
			return true;
		}
	}

	return false;
}

void CBottomWall::hitBy(CSphere& ball) noexcept
{
	if (hasIntersected(ball))
	{
		this->adjustPosition(ball);
		ball.setPower(ball.getVelocity_X() * (1 - LOSS_RATIO), -ball.getVelocity_Z() * (1 - LOSS_RATIO));
	}
}