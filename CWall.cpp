#include "d3dUtility.h"
#include "CWall.h"

const float CWall::LOSS_RATIO = 0.006;

// 벽의 생성자
CWall::CWall(float iwidth, float iheight, float idepth, D3DXCOLOR color)
{
	D3DXMatrixIdentity(&mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;
	m_width = iwidth;
	m_height = iheight;
	m_depth = idepth;

	m_pBoundMesh = nullptr;
}

// 벽의 소멸자
CWall::~CWall()
{
	
}

bool CWall::create(IDirect3DDevice9* pDevice)
//벽을 화면에 생성함
{
	if (pDevice == nullptr)
		return false;

	if (FAILED(D3DXCreateBox(pDevice, m_width, m_height, m_depth, &m_pBoundMesh, NULL)))
		return false;

	return true;
}

// 벽을 화면에서 소멸시킴
void CWall::destroy()
{
	if (m_pBoundMesh != nullptr) {
		m_pBoundMesh->Release();
		m_pBoundMesh = nullptr;
	}
}

// 벽을 화면에 그려냄
void CWall::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (pDevice == nullptr) return;
	
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &mLocal);
	pDevice->SetMaterial(&m_mtrl);
	
	m_pBoundMesh->DrawSubset(0);
}

void CWall::adjustPosition(CSphere& ball)
{
	//보간법으로 근사하여 충돌 시점의 좌표로 이동함.
	ball.setPosition((ball.getPosition().x + ball.getPreCenter_x()) / 2, ball.getPosition().y, (ball.getPosition().z + ball.getPreCenter_z()) / 2);
	if (this->hasIntersected(ball))
	{
		this->setPosition(ball.getPreCenter_x(), ball.getPosition().y, ball.getPreCenter_z());
	}
}