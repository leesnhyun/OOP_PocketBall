#include "d3dUtility.h"
#include "CWall.h"

const float CWall::LOSS_RATIO = 0.006;

// ���� ������
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

// ���� �Ҹ���
CWall::~CWall()
{
	
}

bool CWall::create(IDirect3DDevice9* pDevice)
//���� ȭ�鿡 ������
{
	if (pDevice == nullptr)
		return false;

	if (FAILED(D3DXCreateBox(pDevice, m_width, m_height, m_depth, &m_pBoundMesh, NULL)))
		return false;

	return true;
}

// ���� ȭ�鿡�� �Ҹ��Ŵ
void CWall::destroy()
{
	if (m_pBoundMesh != nullptr) {
		m_pBoundMesh->Release();
		m_pBoundMesh = nullptr;
	}
}

// ���� ȭ�鿡 �׷���
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
	//���������� �ٻ��Ͽ� �浹 ������ ��ǥ�� �̵���.
	ball.setPosition((ball.getPosition().x + ball.getPreCenter_x()) / 2, ball.getPosition().y, (ball.getPosition().z + ball.getPreCenter_z()) / 2);
	if (this->hasIntersected(ball))
	{
		this->setPosition(ball.getPreCenter_x(), ball.getPosition().y, ball.getPreCenter_z());
	}
}