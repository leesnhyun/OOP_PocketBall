#include "d3dUtility.h"
#include "CHole.h"
#include <cmath>
// ������ �����ڸ� ����
CHole::CHole()
{
	D3DXMatrixIdentity(&m_mLocal);				// Transform Matrix�� ������ķ� �ʱ�ȭ
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));		// memset�� ���� ��� 0���� �ʱ�ȭ
	m_radius = 0;
	m_pSphereMesh = NULL;
}

// ������ �Ҹ��ڸ� ����
CHole::~CHole()
{

}

// ������ ȭ�鿡 ������
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

void CHole::destroy()// ������ ȭ�鿡�� �Ҹ��Ŵ 
{
	if (m_pSphereMesh != NULL)
	{
		m_pSphereMesh->Release();
		m_pSphereMesh = NULL;
	}
}

//@TODO : ���Ŀ� �ٽ� �ؾ��մϴ�. --> ���� �ƴ϶� ���ȭ �۾��� �ؾ���
void CHole::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)// ������ ȭ�鿡 �׷���
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

// ���ۿ� ���� ���� �ϴ��� Ȯ��
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

// @TODO ������ �־�� �ϴ� ��� ���� �����ϰ� ���ÿ� �ִ´�. --> �׷��� stack�� ������ �մϴ�.
void CHole::hitBy(CSphere& ball)
{
	if (this->hasIntersected(ball))
	{
		// TODO : Destroy ball.
		ball.setCenter(ball.getVelocity_X() * 100000, 10.0f, ball.getVelocity_Z() * 100000);
		ball.setPower(0.0f, 0.0f);
		ball.holeIn();
	}
	//ball�� setPower�� 0���� ������ �����. + ��ġ�� ������� ��(setCenter)
	/* �ݵ�� �浹�� Ȯ���ϴ� �ڵ带 ���⿡ �־�� �� */
	// Insert your code here.
}

void CHole::setCenter(float x, float y, float z) // ������ �߽� ��ǥ�� ������
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
}// ������ �������� �޾ƿ�

const D3DXMATRIX& CHole::getLocalTransform(void) const
{
	return m_mLocal;
}

void CHole::setLocalTransform(const D3DXMATRIX& mLocal)
{
	m_mLocal = mLocal;
}

D3DXVECTOR3 CHole::getCenter(void) const // ������ �߽� ��ǥ�� ��ȯ��
{
	D3DXVECTOR3 org(center_x, center_y, center_z);
	return org;
}