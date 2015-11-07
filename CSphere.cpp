#include "d3dUtility.h"
#include "CSphere.h"
#include <cmath>

// 공의 생성자를 정의
CSphere::CSphere()
{
	D3DXMatrixIdentity(&m_mLocal);				// Transform Matrix를 단위행렬로 초기화
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));		// memset을 통해 모두 0으로 초기화
	m_radius = 0;
	m_velocity_x = 0;
	m_velocity_z = 0;
	m_pSphereMesh = NULL;
}

// 공의 소멸자를 정의
CSphere::~CSphere()
{

}

// 공을 화면에 생성함
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

void CSphere::destroy()// 공을 화면에서 소멸시킴 
{
	if (m_pSphereMesh != NULL) 
	{
		m_pSphereMesh->Release();
		m_pSphereMesh = NULL;
	}
}

void CSphere::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)// 공을 화면에 그려냄
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

// 두 공이 충돌 했는지 확인
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

// 공이 충돌한 경우, 두 공의 방향과 속도를 바꿈.
void CSphere::hitBy(CSphere& ball)
{
	if (this->hasIntersected(ball))
	{
		//도움을 준 자료: http://blog.hansune.com/106
		//this - ball로 양의 벡터를 정함
		D3DXVECTOR3 cord = this->getCenter();
		D3DXVECTOR3 ball_cord = ball.getCenter();
		//보간법으로 근사하여 충돌 시점의 좌표로 이동함.
		this->setCenter((cord.x + this->pre_center_x) / 2, cord.y, (cord.z + this->pre_center_z) / 2);
		ball.setCenter((ball_cord.x + ball.pre_center_x) / 2, ball_cord.y, (ball_cord.z + ball.pre_center_z) / 2);
		if (this->hasIntersected(ball))
		{
			this->setCenter(this->pre_center_x, cord.y, this->pre_center_z);
			ball.setCenter(ball.pre_center_x, ball_cord.y, ball.pre_center_z);
		}

		cord = this->getCenter();
		ball_cord = ball.getCenter();
		//두 공 사이의 방향 벡터
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

void CSphere::ballUpdate(float timeDiff) // 공의 중심 좌표를 속도에 맞춰서 매 시간 간격마다 갱신함
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
	this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);// 공이 움직일 때마다, 속도를 낮춤
}

double CSphere::getVelocity_X() 
{ 
	return this->m_velocity_x;
} 
// 공의 x축 속도를 반환함

double CSphere::getVelocity_Z() 
{
	return this->m_velocity_z;
} 
// 공의 z축 속도를 반환함

void CSphere::setPower(double vx, double vz) // 공의 속도를 바꿈
{
	this->m_velocity_x = vx;
	this->m_velocity_z = vz;
}

void CSphere::setCenter(float x, float y, float z) // 공의 중심 좌표를 변경함
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
}// 공의 반지름을 받아옴

const D3DXMATRIX& CSphere::getLocalTransform(void) const 
{ 
	return m_mLocal; 
}

void CSphere::setLocalTransform(const D3DXMATRIX& mLocal)
{ 
	m_mLocal = mLocal; 
}

D3DXVECTOR3 CSphere::getCenter(void) const // 공의 중심 좌표를 반환함
{
	D3DXVECTOR3 org(center_x, center_y, center_z);
	return org;
}