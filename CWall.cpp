#include "d3dUtility.h"
#include "CWall.h"

// 벽의 생성자
CWall::CWall()
{
	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_width = 0;
	m_depth = 0;
	m_pBoundMesh = NULL;
}

// 벽의 소멸자
CWall::~CWall()
{

}

bool CWall::create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, bool iis_vertical, D3DXCOLOR color)
//벽을 화면에 생성함
{
	if (NULL == pDevice)
		return false;

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;

	m_width = iwidth;
	m_depth = idepth;
	m_is_vertical = iis_vertical;

	if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
		return false;

	return true;
}

// 벽을 화면에서 소멸시킴
void CWall::destroy()
{
	if (m_pBoundMesh != NULL) {
		m_pBoundMesh->Release();
		m_pBoundMesh = NULL;
	}
}

// 벽을 화면에 그려냄
void CWall::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (NULL == pDevice) return;
	
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	pDevice->SetMaterial(&m_mtrl);
	
	m_pBoundMesh->DrawSubset(0);
}

// 벽에 공이 충돌 했는지 확인
bool CWall::hasIntersected(CSphere& ball)
{
	if (ball.isDead()) return false;

	//세로벽에 충돌
	if (m_is_vertical == true) 
	{
		if (this->m_z + (this->m_depth / 2) >= ball.getCenter().z && this->m_z - (this->m_depth / 2) <= ball.getCenter().z) 
		{
			if ((this->m_x - (this->m_width / 2) <= ball.getCenter().x + ball.getRadius() &&
				this->m_x + (this->m_width / 2) >= ball.getCenter().x + ball.getRadius()))
			{
				ball.setCenter(this->m_x - (this->m_width / 2) - ball.getRadius(), ball.getCenter().y, ball.getCenter().z);
				return true;
			}
			if (this->m_x - (this->m_width / 2) <= ball.getCenter().x - ball.getRadius() &&
				this->m_x + (this->m_width / 2) >= ball.getCenter().x - ball.getRadius())
			{
				ball.setCenter(this->m_x + (this->m_width / 2) + ball.getRadius(), ball.getCenter().y, ball.getCenter().z);
				return true;
			}
		}
	}
	//가로벽에 충돌
	else 
	{
		if (this->m_x + (this->m_width / 2) >= ball.getCenter().x && this->m_x - (this->m_width / 2) <= ball.getCenter().x) 
		{
			if ((this->m_z - (this->m_depth / 2) <= ball.getCenter().z + ball.getRadius() &&
				this->m_z + (this->m_depth / 2) >= ball.getCenter().z + ball.getRadius()))
			{
				ball.setCenter(ball.getCenter().x, ball.getCenter().y, this->m_z - (this->m_depth / 2) - ball.getRadius());
				return true;
			}
			if (this->m_z - (this->m_depth / 2) <= ball.getCenter().z - ball.getRadius() &&
				this->m_z + (this->m_depth / 2) >= ball.getCenter().z - ball.getRadius())
			{
				ball.setCenter(ball.getCenter().x, ball.getCenter().y, this->m_z + (this->m_depth / 2) + ball.getRadius());
				return true;
			}
		}
	}
	return false;
}

// 벽에 공이 충돌할 경우 공의 방향과 속도를 바꿈
void CWall::hitBy(CSphere& ball)
{
	const float LOSS_RATIO = 0.006;
	
	if (this->hasIntersected(ball))
	{
		//부딪히면 LOSS_RATIO만큼 에너지 손실 발생
		ball.setPower(ball.getVelocity_X() * (1 - LOSS_RATIO), ball.getVelocity_Z() * (1 - LOSS_RATIO));
		//세로벽에 충돌
		if (m_is_vertical == true) 
		{
			ball.setPower(-ball.getVelocity_X(), ball.getVelocity_Z());
		}
		//가로벽에 충돌
		else 
		{
			ball.setPower(ball.getVelocity_X(), -ball.getVelocity_Z());
		}
	}
	// Insert your code here.
}

// 벽의 위치를 바꿈
void CWall::setPosition(float x, float y, float z)
{
	D3DXMATRIX m;
	this->m_x = x;
	this->m_z = z;

	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}

void CWall::setIsVertical(bool is_vertical) {
	this->m_is_vertical = is_vertical;
}

// 벽의 높이를 반환
float CWall::getHeight(void) const
{
	return M_HEIGHT;
}

void CWall::setLocalTransform(const D3DXMATRIX& mLocal)
{
	m_mLocal = mLocal;
}