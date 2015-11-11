#include "d3dUtility.h"
#include "CHole.h"
#include <cmath>

// 구멍의 생성자를 정의
CHole::CHole() : m_radius(0.25)
{
	D3DXMatrixIdentity(&mLocal);				// Transform Matrix를 단위행렬로 초기화
	D3DXMatrixIdentity(&rotationMat);				// Transform Matrix를 단위행렬로 초기화
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));		// memset을 통해 모두 0으로 초기화
	m_pSphereMesh = nullptr;

	m_mtrl.Ambient = d3d::BLACK;
	m_mtrl.Diffuse = d3d::BLACK;
	m_mtrl.Specular = d3d::BLACK;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;
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
	D3DXMatrixRotationX(&rotationMat, 0.35);
	rotationMat *= rotationMat;

	//setLocalTransform(m*mLocal);
	
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
bool CHole::hasIntersected(CSphere& ball) const
{
	// TODO : Type checking..
	double xDistance = pow((this->center_x - ball.getPosition().x), 2);
	double yDistance = 0;
	double zDistance = pow((this->center_z - ball.getPosition().z), 2);
	double totalDistance = sqrt(xDistance + yDistance + zDistance);

	if (totalDistance < (this->getRadius()) && !ball.isDisabled())
	{
		return true;
	}
	//+-4.56은 세로벽의 좌표, +-3.06은 가로벽의 좌표, 0.15는 각각 가로벽과 세로벽의 depth와 width
	if (ball.getPosition().x + ball.getRadius() > 4.56f - (0.15f / 2) ||
		ball.getPosition().x - ball.getRadius() < -4.56f + (0.15f / 2) ||
		ball.getPosition().z + ball.getRadius() > 3.06f - (0.15f / 2) ||
		ball.getPosition().z - ball.getRadius() < -3.06f + (0.15f / 2))
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
		ball.disable();
	}
}

float CHole::getRadius(void) const
{
	return m_radius;
}// 구멍의 반지름을 받아옴