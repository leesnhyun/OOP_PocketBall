#include "d3dUtility.h"
#include "CFloor.h"

// �ٴ��� ������
CFloor::CFloor(float iwidth, float iheight, float idepth, D3DXCOLOR color)
{
	D3DXMatrixIdentity(&mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;
	m_width = iwidth;
	m_depth = idepth;
	m_height = iheight;
	m_pBoundMesh = nullptr;
}

// �ٴ��� �Ҹ���
CFloor::~CFloor()
{

}

bool CFloor::create(IDirect3DDevice9* pDevice)
//�ٴ��� ȭ�鿡 ������
{
	if (pDevice == nullptr) return false;

	if (FAILED(D3DXCreateBox(pDevice, m_width, m_height, m_depth, &m_pBoundMesh, NULL)))
		return false;

	return true;
}

// �ٴ��� ȭ�鿡�� �Ҹ��Ŵ
void CFloor::destroy()
{
	if (m_pBoundMesh != nullptr) {
		m_pBoundMesh->Release();
		m_pBoundMesh = nullptr;
	}
}

// �ٴ��� ȭ�鿡 �׷���
void CFloor::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (pDevice == nullptr) return;
	
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &mLocal);
	pDevice->SetMaterial(&m_mtrl);

	m_pBoundMesh->DrawSubset(0);
}