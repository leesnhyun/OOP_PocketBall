#include "d3dUtility.h"
#include "CHole.h"
#include <cmath>

// ������ �����ڸ� ����
CHole::CHole(float radius, D3DXCOLOR color)
{
	D3DXMatrixIdentity(&mLocal);				// Transform Matrix�� ������ķ� �ʱ�ȭ
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));		// memset�� ���� ��� 0���� �ʱ�ȭ
	m_radius = 0;
	m_pSphereMesh = nullptr;

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;
	m_radius = radius;
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
	
	D3DXMATRIX m;
	D3DXMatrixRotationX(&m, 33);
	this->setLocalTransform(m);
	
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

// ���ۿ� �־�� �ϴ� ��� ���� �����Ѵ�.
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
}// ������ �������� �޾ƿ�