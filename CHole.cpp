#include "d3dUtility.h"
#include "CHole.h"
#include <cmath>

// 구멍의 생성자를 정의
CHole::CHole(float radius, D3DXCOLOR color)
{
	D3DXMatrixIdentity(&mLocal);				// Transform Matrix를 단위행렬로 초기화
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));		// memset을 통해 모두 0으로 초기화
	m_radius = 0;
	m_pSphereMesh = nullptr;

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;
	m_radius = radius;
}

// 구멍의 소멸자를 정의
CHole::~CHole()
{

}

// 구멍을 화면에 생성함
bool CHole::create(IDirect3DDevice9* pDevice)
{
	if (pDevice == nullptr)
	{
		return false;
	}

	// 입체를 그리고 회전시킵니다.
	HRESULT hr = D3DXCreateCylinder(pDevice, m_radius, m_radius, 0.5f, 50, 50, &m_pSphereMesh, nullptr);
	
	D3DXMATRIX m;
	D3DXMatrixRotationX(&m, 33);
	this->setLocalTransform(m);
	
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void CHole::destroy()// 구멍을 화면에서 소멸시킴 
{
	if (m_pSphereMesh != nullptr)
	{
		m_pSphereMesh->Release();
		m_pSphereMesh = nullptr;
	}
}

void CHole::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)// 구멍을 화면에 그려냄
{
	if (NULL == pDevice) return;

	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &mLocal);
	pDevice->SetMaterial(&m_mtrl);
	m_pSphereMesh->DrawSubset(0);
}

// 구멍에 공이 들어가야 하는지 확인
bool CHole::hasIntersected(CSphere& ball)
{
	// TODO : Type checking..
	double xDistance = pow((this->center_x - ball.getPosition().x), 2);
	double yDistance = 0;
	double zDistance = pow((this->center_z - ball.getPosition().z), 2);
	double totalDistance = sqrt(xDistance + yDistance + zDistance);

	if (totalDistance < (this->getRadius()))
	{
		return true;
	}

	return false;
}

// 구멍에 넣어야 하는 경우 공을 제거한다.
void CHole::hitBy(CSphere& ball)
{
	if (this->hasIntersected(ball))
	{
		ball.setPosition(100000, -100.0f, 100000);
		ball.setPower(0.0f, 0.0f);
		ball.die();
	}
}

float CHole::getRadius(void) const
{
	return m_radius;
}// 구멍의 반지름을 받아옴