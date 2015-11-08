#include "d3dUtility.h"
#include "CHole.h"
#include <cmath>
// 구멍의 생성자를 정의
CHole::CHole()
{
	D3DXMatrixIdentity(&m_mLocal);				// Transform Matrix를 단위행렬로 초기화
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));		// memset을 통해 모두 0으로 초기화
	m_radius = 0;
	m_pSphereMesh = NULL;
}

// 구멍의 소멸자를 정의
CHole::~CHole()
{

}

// 구멍을 화면에 생성함
bool CHole::create(IDirect3DDevice9* pDevice, D3DXCOLOR color)
{
	if (NULL == pDevice)
		return false;

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;
	m_radius = 0.28f;

	if (FAILED(D3DXCreateSphere(pDevice, 0.28f, 50, 50, &m_pSphereMesh, NULL)))
	{
		return false;
	}

	return true;
}

void CHole::destroy()// 구멍을 화면에서 소멸시킴 
{
	if (m_pSphereMesh != NULL)
	{
		m_pSphereMesh->Release();
		m_pSphereMesh = NULL;
	}
}

//@TODO : 추후에 다시 해야합니다. --> 구가 아니라 평면화 작업을 해야함
void CHole::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)// 구멍을 화면에 그려냄
{
	if (NULL == pDevice)
	{
		return;
	}

	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	pDevice->SetMaterial(&m_mtrl);
	m_pSphereMesh->DrawSubset(0);
}

// 구멍에 공이 들어가야 하는지 확인
bool CHole::hasIntersected(CSphere& ball)
{
	double xDistance = pow((this->center_x - ball.getCenter().x), 2);
	double yDistance = 0;
	double zDistance = pow((this->center_z - ball.getCenter().z), 2);
	double totalDistance = sqrt(xDistance + yDistance + zDistance);

	if (totalDistance < (this->getRadius()))
	{
		return true;
	}

	return false;
}

// @TODO 구멍을 넣어야 하는 경우 공을 제거하고 스택에 넣는다. --> 그래서 stack을 만들어야 합니다.
void CHole::hitBy(CSphere& ball)
{
	if (this->hasIntersected(ball))
	{
		// TODO : Destroy ball.
		ball.setCenter(ball.getVelocity_X() * 100000, 10.0f, ball.getVelocity_Z() * 100000);
		ball.setPower(0.0f, 0.0f);
		ball.holeIn();
	}
	//ball의 setPower를 0으로 설정해 줘야함. + 위치를 날려줘야 함(setCenter)
	/* 반드시 충돌을 확인하는 코드를 여기에 넣어야 함 */
	// Insert your code here.
}

void CHole::setCenter(float x, float y, float z) // 구멍의 중심 좌표를 변경함
{
	D3DXMATRIX m;
	this->center_x = x;
	this->center_y = y;
	this->center_z = z;
	D3DXMatrixTranslation(&m, x, y, z);
	this->setLocalTransform(m);
}

float CHole::getRadius(void) const
{
	return m_radius;
}// 구멍의 반지름을 받아옴

const D3DXMATRIX& CHole::getLocalTransform(void) const
{
	return m_mLocal;
}

void CHole::setLocalTransform(const D3DXMATRIX& mLocal)
{
	m_mLocal = mLocal;
}

D3DXVECTOR3 CHole::getCenter(void) const // 구멍의 중심 좌표를 반환함
{
	D3DXVECTOR3 org(center_x, center_y, center_z);
	return org;
}