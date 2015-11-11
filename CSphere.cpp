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


// 공의 생성자를 정의
CSphere::CSphere(BallType ballType, const char* ballImageFileName)
{
	ZeroMemory(&m_mtrl, sizeof(m_mtrl)); // memset을 통해 모두 0으로 초기화
	this->m_radius = 0.16f;
	this->m_velocity_x = 0;
	this->m_velocity_z = 0;
	this->m_pSphereMesh = nullptr;
	this->ballType = ballType;
	
	D3DXMatrixIdentity(&mLocal);
	D3DXMatrixIdentity(&this->matBallRoll);

	strcpy(this->ballImageFileName, ballImageFileName);
}

// 공의 소멸자를 정의
CSphere::~CSphere()
{
}

// 공을 화면에 생성함(D3DXCOLOR color)
bool CSphere::create(IDirect3DDevice9* pDevice)
{
	if (pDevice == nullptr) return false;

	//m_mtrl.Ambient = color;
	//m_mtrl.Diffuse = color;
	m_mtrl.Diffuse = d3d::WHITE;
	m_mtrl.Ambient = d3d::WHITE;
	m_mtrl.Specular = d3d::WHITE;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 100.0f;
	
	// 텍스쳐를 부드럽게 만듦.
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	this->m_pSphereMesh = _createMappedSphere(pDevice);

	char* filePath = new char[10];
	strcpy(filePath, "./res/");
	strcat(filePath, this->ballImageFileName);
	strcat(filePath, ".bmp");

	if (FAILED(D3DXCreateTextureFromFile(pDevice, filePath, &Tex)))
	{
		delete[] filePath;
		return false;
	}

	delete[] filePath;
	return true;
}

void CSphere::destroy()// 공을 화면에서 소멸시킴 
{
	if (m_pSphereMesh != nullptr)
	{
		m_pSphereMesh->Release();
		d3d::Release<IDirect3DTexture9*>(Tex);
		m_pSphereMesh = nullptr;
	}
}

void CSphere::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)// 공을 화면에 그려냄
{
	// TODO : Removerble의 disable 확인 추가
	if (pDevice == nullptr) return;
	

	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &mLocal);
	pDevice->MultiplyTransform(D3DTS_WORLD, &matBallRoll);
	
	pDevice->SetTexture(0, Tex);
	pDevice->SetMaterial(&m_mtrl);
	m_pSphereMesh->DrawSubset(0);
}


// 두 공이 충돌 했는지 확인
bool CSphere::hasIntersected(CSphere& ball) const noexcept
{
	D3DXVECTOR3 cord = this->getPosition();
	D3DXVECTOR3 ball_cord = ball.getPosition();
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
void CSphere::hitBy(CSphere& ball) noexcept
{
	if (this->hasIntersected(ball))
	{
		//도움을 준 자료: http://blog.hansune.com/106
		//this - ball로 양의 벡터를 정함

		D3DXVECTOR3 cord = this->getPosition();
		D3DXVECTOR3 ball_cord = ball.getPosition();
		//보간법으로 근사하여 충돌 시점의 좌표로 이동함.
		this->setPosition((cord.x + this->pre_center_x) / 2, cord.y, (cord.z + this->pre_center_z) / 2);
		ball.setPosition((ball_cord.x + ball.pre_center_x) / 2, ball_cord.y, (ball_cord.z + ball.pre_center_z) / 2);
		if (this->hasIntersected(ball))
		{
			this->setPosition(this->pre_center_x, cord.y, this->pre_center_z);
			ball.setPosition(ball.pre_center_x, ball_cord.y, ball.pre_center_z);
		}

		cord = this->getPosition();
		ball_cord = ball.getPosition();
		//두 공 사이의 방향 벡터
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

void CSphere::ballUpdate(float timeDiff) // 공의 중심 좌표를 속도에 맞춰서 매 시간 간격마다 갱신함
{
	const float TIME_SCALE = 3.3F;
	D3DXVECTOR3 cord = this->getPosition();
	double vx = abs(this->getVelocity_X());
	double vz = abs(this->getVelocity_Z());

	this->pre_center_x = cord.x;
	this->pre_center_z = cord.z;

	if (vx > 0.01 || vz > 0.01)
	{
		const float SPIN_RATIO = 0.01;

		float tX = cord.x + TIME_SCALE * timeDiff * m_velocity_x;
		float tZ = cord.z + TIME_SCALE * timeDiff * m_velocity_z;

		this->setPosition(tX, cord.y, tZ);

		D3DXMATRIX tmp;
		D3DXVECTOR3 c(this->m_velocity_z, 0, -this->m_velocity_x);

		float force = sqrt(pow(this->m_velocity_x, 2) + pow(this->m_velocity_z, 2));
		D3DXMatrixRotationAxis(&tmp, &c, force * SPIN_RATIO);
		matBallRoll *= tmp;
	}
	else { this->setPower(0, 0); }

	//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
	double rate = 1 - (1 - DECREASE_RATE)*timeDiff * 400;
	if (rate < 0) rate = 0;

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

float CSphere::getRadius(void) const
{
	return (this->m_radius);
}// 공의 반지름을 받아옴

BallType CSphere::getBallType() const
{
	return this->ballType;
}

void CSphere::disable() noexcept
{
	//TODO : get turn count to record disableTurn
}

int CSphere::getDisableTurn() const noexcept
{
	return this->disableTurn;
}

bool CSphere::isDisabled() const noexcept
{
	return (this->disableTurn != -1);
}

// private function
LPD3DXMESH CSphere::_createMappedSphere(IDirect3DDevice9* pDev)
{
	// create the sphere
	LPD3DXMESH mesh;
	if (FAILED(D3DXCreateSphere(pDev, this->getRadius(), 50, 50, &mesh, NULL)))
		return nullptr;

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
	if (SUCCEEDED(texMesh->LockVertexBuffer(0, reinterpret_cast<void **>(&pVerts)))) {

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