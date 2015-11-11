#include "d3dUtility.h"
#include "CHole.h"
#include <cmath>

// ������ �����ڸ� ����
CHole::CHole() : m_radius(0.25)
{
	D3DXMatrixIdentity(&mLocal);				// Transform Matrix�� ������ķ� �ʱ�ȭ
	D3DXMatrixIdentity(&rotationMat);				// Transform Matrix�� ������ķ� �ʱ�ȭ
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));		// memset�� ���� ��� 0���� �ʱ�ȭ
	m_pSphereMesh = nullptr;

	m_mtrl.Ambient = d3d::BLACK;
	m_mtrl.Diffuse = d3d::BLACK;
	m_mtrl.Specular = d3d::BLACK;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;
}

// ������ �Ҹ��ڸ� ����
CHole::~CHole()
{

}

// ������ ȭ�鿡 ������
bool CHole::create(IDirect3DDevice9* pDevice)
{
	if (pDevice == nullptr)
	{
		return false;
	}

	// ��ü�� �׸��� ȸ����ŵ�ϴ�.
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

void CHole::destroy()// ������ ȭ�鿡�� �Ҹ��Ŵ 
{
	if (m_pSphereMesh != nullptr)
	{
		m_pSphereMesh->Release();
		m_pSphereMesh = nullptr;
	}
}

void CHole::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)// ������ ȭ�鿡 �׷���
{
	if (NULL == pDevice) return;

	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &mLocal);
	
	pDevice->SetMaterial(&m_mtrl);
	m_pSphereMesh->DrawSubset(0);
}

// ���ۿ� ���� ���� �ϴ��� Ȯ��
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
	//+-4.56�� ���κ��� ��ǥ, +-3.06�� ���κ��� ��ǥ, 0.15�� ���� ���κ��� ���κ��� depth�� width
	if (ball.getPosition().x + ball.getRadius() > 4.56f - (0.15f / 2) ||
		ball.getPosition().x - ball.getRadius() < -4.56f + (0.15f / 2) ||
		ball.getPosition().z + ball.getRadius() > 3.06f - (0.15f / 2) ||
		ball.getPosition().z - ball.getRadius() < -3.06f + (0.15f / 2))
	{
		return true;
	}

	return false;
}

// ���ۿ� �־�� �ϴ� ��� ���� �����Ѵ�.
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
}// ������ �������� �޾ƿ�