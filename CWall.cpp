#include "d3dUtility.h"
#include "CWall.h"

static const float LOSS_RATIO = 0.006;

// 벽의 생성자
CWall::CWall(float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color)
{
	D3DXMATRIX mLocal = getLocalTransform();
	D3DXMatrixIdentity(&mLocal);
	this->setLocalTransform(mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;
	m_width = iwidth;
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
	D3DXMATRIX mLocal = getLocalTransform();
	pDevice->MultiplyTransform(D3DTS_WORLD, &mLocal);
	setLocalTransform(mLocal);
	pDevice->SetMaterial(&m_mtrl);
	
	m_pBoundMesh->DrawSubset(0);
}