#include "d3dUtility.h"
#include "CSphere.h"
#include <cmath>

// ���� �����ڸ� ����
CSphere::CSphere()
{
	D3DXMatrixIdentity(&m_mLocal);				// Transform Matrix�� ������ķ� �ʱ�ȭ
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));		// memset�� ���� ��� 0���� �ʱ�ȭ
	m_radius = 0;
	m_velocity_x = 0;
	m_velocity_z = 0;
	m_pSphereMesh = NULL;
}

// ���� �Ҹ��ڸ� ����
CSphere::~CSphere()
{

}

// ���� ȭ�鿡 ������
bool CSphere::create(IDirect3DDevice9* pDevice, D3DXCOLOR color)
{
	if (NULL == pDevice)
		return false;

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 100.0f;
	
	if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL)))
	{
		return false;
	}

	return true;
}

void CSphere::destroy()// ���� ȭ�鿡�� �Ҹ��Ŵ 
{
	if (m_pSphereMesh != NULL) 
	{
		m_pSphereMesh->Release();
		m_pSphereMesh = NULL;
	}
}

void CSphere::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)// ���� ȭ�鿡 �׷���
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

// �� ���� �浹 �ߴ��� Ȯ��
bool CSphere::hasIntersected(CSphere& ball)
{
	D3DXVECTOR3 cord = this->getCenter();
	D3DXVECTOR3 ball_cord = ball.getCenter();
	double xDistance = abs((cord.x - ball_cord.x) * (cord.x - ball_cord.x));
	double zDistance = abs((cord.z - ball_cord.z) * (cord.z - ball_cord.z));
	double totalDistance = sqrt(xDistance + zDistance);

	if (totalDistance < (this->getRadius() + ball.getRadius()))
	{
		return true;
	}

	return false;
}

// ���� �浹�� ���, �� ���� ����� �ӵ��� �ٲ�.
void CSphere::hitBy(CSphere& ball)
{
	if (this->hasIntersected(ball))
	{
		//������ �� �ڷ�: http://blog.hansune.com/106
		//this - ball�� ���� ���͸� ����
		D3DXVECTOR3 cord = this->getCenter();
		D3DXVECTOR3 ball_cord = ball.getCenter();
		//���������� �ٻ��Ͽ� �浹 ������ ��ǥ�� �̵���.
		this->setCenter((cord.x + this->pre_center_x) / 2, cord.y, (cord.z + this->pre_center_z) / 2);
		ball.setCenter((ball_cord.x + ball.pre_center_x) / 2, ball_cord.y, (ball_cord.z + ball.pre_center_z) / 2);
		if (this->hasIntersected(ball))
		{
			this->setCenter(this->pre_center_x, cord.y, this->pre_center_z);
			ball.setCenter(ball.pre_center_x, ball_cord.y, ball.pre_center_z);
		}

		cord = this->getCenter();
		ball_cord = ball.getCenter();
		//�� �� ������ ���� ����
		double d_x = cord.x - ball_cord.x;
		double d_z = cord.z - ball_cord.z;
		double size_d = sqrt((d_x*d_x) + (d_z*d_z));

		double vax = this->m_velocity_x;
		double vaz = this->m_velocity_z;
		double vbx = ball.m_velocity_x;
		double vbz = ball.m_velocity_z;

		double size_this_v = sqrt((vax*vax) + (vaz*vaz));

		double cos_t = d_x / size_d;
		double sin_t = d_z / size_d;

		double vaxp = vbx*cos_t + vbz*sin_t;
		double vbxp = vax*cos_t + vaz*sin_t;
		double vazp = vaz*cos_t - vax*sin_t;
		double vbzp = vbz*cos_t - vbx*sin_t;

		this->setPower(vaxp*cos_t - vazp*sin_t, vaxp*sin_t + vazp*cos_t);
		ball.setPower(vbxp*cos_t - vbzp*sin_t, vbxp*sin_t + vbzp*cos_t);
	}

}

void CSphere::ballUpdate(float timeDiff) // ���� �߽� ��ǥ�� �ӵ��� ���缭 �� �ð� ���ݸ��� ������
{
	const float TIME_SCALE = 3.3F;
	D3DXVECTOR3 cord = this->getCenter();
	double vx = abs(this->getVelocity_X());
	double vz = abs(this->getVelocity_Z());

	this->pre_center_x = cord.x;
	this->pre_center_z = cord.z;

	if (vx > 0.01 || vz > 0.01)
	{
		float tX = cord.x + TIME_SCALE*timeDiff*m_velocity_x;
		float tZ = cord.z + TIME_SCALE*timeDiff*m_velocity_z;

		//correction of position of ball
		/* Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
		if(tX >= (4.5 - M_RADIUS))
		tX = 4.5 - M_RADIUS;
		else if(tX <=(-4.5 + M_RADIUS))
		tX = -4.5 + M_RADIUS;
		else if(tZ <= (-3 + M_RADIUS))
		tZ = -3 + M_RADIUS;
		else if(tZ >= (3 - M_RADIUS))
		tZ = 3 - M_RADIUS;
		*/

		this->setCenter(tX, cord.y, tZ);
	}
	else { this->setPower(0, 0); }
	//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
	double rate = 1 - (1 - DECREASE_RATE)*timeDiff * 400;
	if (rate < 0)
		rate = 0;
	this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);// ���� ������ ������, �ӵ��� ����
}

double CSphere::getVelocity_X() 
{ 
	return this->m_velocity_x;
} 
// ���� x�� �ӵ��� ��ȯ��

double CSphere::getVelocity_Z() 
{
	return this->m_velocity_z;
} 
// ���� z�� �ӵ��� ��ȯ��

void CSphere::setPower(double vx, double vz) // ���� �ӵ��� �ٲ�
{
	this->m_velocity_x = vx;
	this->m_velocity_z = vz;
}

void CSphere::setCenter(float x, float y, float z) // ���� �߽� ��ǥ�� ������
{
	D3DXMATRIX m;

	this->center_x = x;
	this->center_y = y;
	this->center_z = z;
	
	D3DXMatrixTranslation(&m, x, y, z);
	this->setLocalTransform(m);
}

float CSphere::getRadius(void) const 
{ 
	return (float)(M_RADIUS);
}// ���� �������� �޾ƿ�

const D3DXMATRIX& CSphere::getLocalTransform(void) const 
{ 
	return m_mLocal; 
}

void CSphere::setLocalTransform(const D3DXMATRIX& mLocal)
{ 
	m_mLocal = mLocal; 
}

D3DXVECTOR3 CSphere::getCenter(void) const // ���� �߽� ��ǥ�� ��ȯ��
{
	D3DXVECTOR3 org(center_x, center_y, center_z);
	return org;
}