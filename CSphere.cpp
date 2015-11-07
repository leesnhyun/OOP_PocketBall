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
	double xDistance = pow((this->center_x - ball.center_x), 2);
	double yDistance = pow((this->center_y - ball.center_y), 2);
	double zDistance = pow((this->center_z - ball.center_z), 2);
	double totalDistance = sqrt(xDistance + yDistance + zDistance);

	if (totalDistance <= (this->getRadius() + ball.getRadius()))
	{
		return true;
	}

	return false;
}

// ���� �浹�� ���, �� ���� ����� �ӵ��� �ٲ�.
void CSphere::hitBy(CSphere& ball)
{
	static const float LOSS_RATIO = 0.015;

	if (this->hasIntersected(ball))
	{
		//�� ������ �ս� �߻�
		this->setPower(ball.getVelocity_X() * (1 - LOSS_RATIO), ball.getVelocity_Z() * (1 - LOSS_RATIO));
		ball.setPower(ball.getVelocity_X() * (1 - LOSS_RATIO), ball.getVelocity_Z() * (1 - LOSS_RATIO));

		//temp�� ball �� ����
		CSphere temp_this, temp_ball;
		temp_this = *this;
		temp_ball = ball;

		float vector_size;
		//ball�� *this�� ħ
		//*this�� ���� : *this�� ��ǥ - ball�� ��ǥ --> ũ�⸦ 1�� ���� ���� ���ͷ� ���
		this->setPower(this->center_x - ball.center_x, this->center_z - ball.center_z);
		vector_size = sqrt(pow(this->m_velocity_x, 2) + pow(this->m_velocity_z, 2));
		this->setPower(this->m_velocity_x / vector_size, this->m_velocity_z / vector_size);
		//*this�� ũ�� : ball�� �ӵ��� *this�� ���� ���Ϳ� ���翵 ��Ų ��
		vector_size = (this->m_velocity_x * ball.m_velocity_x + this->m_velocity_x * ball.m_velocity_x) / 
			(sqrt(pow(this->m_velocity_x, 2) + pow(this->m_velocity_z, 2)));
		this->setPower(this->m_velocity_x / vector_size, this->m_velocity_z / vector_size);
		//���� ball�� �ӵ� : ball�� ���� �ӵ� - *this�� ���� �ӵ�
		ball.setPower(ball.m_velocity_x - this->m_velocity_x, ball.m_velocity_z - this->m_velocity_z);

		//*this�� ball�� ħ
		//ball�� ���� : ball�� ��ǥ - *this�� ��ǥ --> ũ�⸦ 1�� ���� ���� ���ͷ� ���
		temp_ball.setPower(temp_ball.center_x - temp_this.center_x, temp_ball.center_z - temp_this.center_z);
		vector_size = sqrt(pow(temp_ball.m_velocity_x, 2) + pow(temp_ball.m_velocity_z, 2));
		temp_ball.setPower(temp_ball.m_velocity_x / vector_size, temp_ball.m_velocity_z / vector_size);
		//ball�� ũ�� : *this�� �ӵ��� ball�� ���� ���Ϳ� ���翵 ��Ų ��
		vector_size = (temp_ball.m_velocity_x * temp_this.m_velocity_x + temp_ball.m_velocity_x * temp_this.m_velocity_x) /
			(sqrt(pow(temp_ball.m_velocity_x, 2) + pow(temp_ball.m_velocity_z, 2)));
		temp_ball.setPower(temp_ball.m_velocity_x / vector_size, temp_ball.m_velocity_z / vector_size);
		//���� *this�� �ӵ� : *this�� ���� �ӵ� - ball�� ���� �ӵ�
		temp_this.setPower(temp_this.m_velocity_x - temp_ball.m_velocity_x, temp_this.m_velocity_z - temp_ball.m_velocity_z);
		//�շ� ��� (*this -> ball ���� ���� ������ ���� �ӵ� + ball -> *this ���� ���� ������ ���� �ӵ�)
		this->setPower(this->m_velocity_x + temp_this.m_velocity_x, this->m_velocity_z + temp_this.m_velocity_z);
		ball.setPower(ball.m_velocity_x + temp_ball.m_velocity_x, ball.m_velocity_z + temp_ball.m_velocity_z);
	}
	/* �ݵ�� �浹�� Ȯ���ϴ� �ڵ带 ���⿡ �־�� �� */
}

void CSphere::ballUpdate(float timeDiff) // ���� �߽� ��ǥ�� �ӵ��� ���缭 �� �ð� ���ݸ��� ������
{
	const float TIME_SCALE = 3.3;
	D3DXVECTOR3 cord = this->getCenter();
	double vx = abs(this->getVelocity_X());
	double vz = abs(this->getVelocity_Z());

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