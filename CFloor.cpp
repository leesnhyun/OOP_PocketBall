#include "d3dUtility.h"
#include "CFloor.h"

// ¹Ù´ÚÀÇ »ý¼ºÀÚ
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

// ¹Ù´ÚÀÇ ¼Ò¸êÀÚ
CFloor::~CFloor()
{

}

bool CFloor::create(IDirect3DDevice9* pDevice)
//¹Ù´ÚÀ» È­¸é¿¡ »ý¼ºÇÔ
{
	if (pDevice == nullptr) return false;

	if (FAILED(D3DXCreateBox(pDevice, m_width, m_height, m_depth, &m_pBoundMesh, NULL)))
		return false;

	return true;
}

// ¹Ù´ÚÀ» È­¸é¿¡¼­ ¼Ò¸ê½ÃÅ´
void CFloor::destroy()
{
	if (m_pBoundMesh != nullptr) {
		m_pBoundMesh->Release();
		m_pBoundMesh = nullptr;
	}
}

// ¹Ù´ÚÀ» È­¸é¿¡ ±×·Á³¿
void CFloor::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (pDevice == nullptr) return;
	
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &mLocal);
	pDevice->SetMaterial(&m_mtrl);

	m_pBoundMesh->DrawSubset(0);
}