#include "d3dUtility.h"
#include "CSphere.h"
#include <cmath>
#include "TurnManager.h"

struct _VERTEX
{
	D3DXVECTOR3 pos;     // vertex position
	D3DXVECTOR3 norm;    // vertex normal
	float tu;            // texture coordinates
	float tv;
};

#define FVF_VERTEX    D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1

extern TurnManager turnManager;
const float CSphere::STOP_SPEED = 0.01f;


// ���� �����ڸ� ����
CSphere::CSphere(BallType ballType)
{
	D3DXMatrixIdentity(&m_mLocal); // Transform Matrix�� ������ķ� �ʱ�ȭ
	ZeroMemory(&m_mtrl, sizeof(m_mtrl)); // memset�� ���� ��� 0���� �ʱ�ȭ
	m_radius = (float) M_RADIUS ;
	m_velocity_x = 0;
	m_velocity_z = 0;
	m_pSphereMesh = NULL;
	this->deadDate = -1;
	this->ballType = ballType;
}

// ���� �Ҹ��ڸ� ����
CSphere::~CSphere()
{
}

bool CSphere::isDead() const
{
	return (this->deadDate >= 0);
}

int CSphere::getDeadDate() const
{
	return this->deadDate;
}

void CSphere::die()
{
	this->deadDate = turnManager.getCurrentTurnNumber();
}

// ���� ȭ�鿡 ������
bool CSphere::create(IDirect3DDevice9* pDevice, D3DXCOLOR color)
{
	if (NULL == pDevice) return false;

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	//m_mtrl.Diffuse = d3d::WHITE;
	//m_mtrl.Ambient = d3d::WHITE;
	m_mtrl.Specular = d3d::WHITE;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 100.0f;
	
	// �ؽ��ĸ� �ε巴�� ����.
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	this->m_pSphereMesh = _createMappedSphere(pDevice);

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "s10.bmp", &Tex)))
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
		d3d::Release<IDirect3DTexture9*>(Tex);
		m_pSphereMesh = NULL;
	}
}

void CSphere::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)// ���� ȭ�鿡 �׷���
{

	if (NULL == pDevice || isDead()){
		return;
	}

	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	pDevice->SetTexture(0, Tex);
	pDevice->SetMaterial(&m_mtrl);
	//pDevice->SetMaterial(&d3d::WHITE_MTRL);
	

	//D3DXMATRIX m;
	//D3DXMatrixRotationY(&m, 30);

	m_pSphereMesh->DrawSubset(0);
}

// �� ���� �浹 �ߴ��� Ȯ��
bool CSphere::hasIntersected(CSphere& ball)
{
	if (this->isDead() || ball.isDead())
	{
		return false;
	}

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
		double size_d = sqrt((d_x * d_x) + (d_z * d_z));

		double vax = this->m_velocity_x;
		double vaz = this->m_velocity_z;
		double vbx = ball.m_velocity_x;
		double vbz = ball.m_velocity_z;

		double size_this_v = sqrt((vax * vax) + (vaz * vaz));

		double cos_t = d_x / size_d;
		double sin_t = d_z / size_d;

		double vaxp = vbx * cos_t + vbz * sin_t;
		double vbxp = vax * cos_t + vaz * sin_t;
		double vazp = vaz * cos_t - vax * sin_t;
		double vbzp = vbz * cos_t - vbx * sin_t;

		this->setPower(vaxp * cos_t - vazp * sin_t, vaxp * sin_t + vazp * cos_t);
		ball.setPower(vbxp * cos_t - vbzp * sin_t, vbxp * sin_t + vbzp * cos_t);
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
		float tX = cord.x + TIME_SCALE * timeDiff * m_velocity_x;
		float tZ = cord.z + TIME_SCALE * timeDiff * m_velocity_z;

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
	if (rate < 0) rate = 0;

	this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);// ���� ������ ������, �ӵ��� ����
	
	// �� ȸ�����Ѻ���
	D3DXMATRIX matBallRoll;
	//D3DXMatrixRotationYawPitchRoll(&matBallRoll, 0.0f, timeDiff, 0.0f);
	//D3DXMatrixRotationY(&matBallRoll, timeDiff);
	//this->m_mLocal *= matBallRoll;

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
	return (this->m_radius);
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

BallType CSphere::getBallType() const
{
	return this->ballType;
}

// private function
LPD3DXMESH CSphere::_createMappedSphere(IDirect3DDevice9* pDev)
{
	// create the sphere
	LPD3DXMESH mesh;
	if (FAILED(D3DXCreateSphere(pDev, this->getRadius(), 50, 50, &mesh, NULL)))
		return NULL;

	// create a copy of the mesh with texture coordinates,
	// since the D3DX function doesn't include them
	LPD3DXMESH texMesh;
	if (FAILED(mesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX, pDev, &texMesh)))
		// failed, return un-textured mesh
		return mesh;

	// finished with the original mesh, release it
	mesh->Release();

	// lock the vertex buffer
	//LPVERTEX pVerts;
	struct _VERTEX* pVerts;
	if (SUCCEEDED(texMesh->LockVertexBuffer(0, (void **)&pVerts))) {

		// get vertex count
		int numVerts = texMesh->GetNumVertices();

		// loop through the vertices
		for (int i = 0; i < numVerts; i++) {

			// calculate texture coordinates
			pVerts->tu = asinf(pVerts->norm.x) / D3DX_PI + 0.5f;
			pVerts->tv = asinf(pVerts->norm.y) / D3DX_PI + 0.5f;

			// go to next vertex
			pVerts++;
		}

		// unlock the vertex buffer
		texMesh->UnlockVertexBuffer();
	}

	// return pointer to caller
	return texMesh;
}