#include "d3dUtility.h"
#include "CLight.h"

// ±¤¿øÀÇ »ý¼ºÀÚ
CLight::CLight(float radius)
{
	D3DLIGHT9 lit;
	::ZeroMemory(&lit, sizeof(lit));
	lit.Type = D3DLIGHT_POINT;
	lit.Diffuse = d3d::WHITE;
	//lit.Diffuse      = d3d::BLACK; 
	lit.Specular = d3d::BLACK * 0.9f;
	lit.Ambient = d3d::WHITE * 0.9f;
	lit.Position = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	lit.Range = 80.0f;
	lit.Attenuation0 = 0.0f;
	lit.Attenuation1 = 0.9f;
	lit.Attenuation2 = 0.0f;

	static DWORD i = 0;
	m_index = i++;
	D3DXMatrixIdentity(&mLocal);
	::ZeroMemory(&m_lit, sizeof(m_lit));
	m_pMesh = nullptr;
	m_bound._center = lit.Position;
	m_bound._radius = radius;

	m_lit.Type = lit.Type;
	m_lit.Diffuse = lit.Diffuse;
	m_lit.Specular = lit.Specular;
	m_lit.Ambient = lit.Ambient;
	m_lit.Position = lit.Position;
	m_lit.Direction = lit.Direction;
	m_lit.Range = lit.Range;
	m_lit.Falloff = lit.Falloff;
	m_lit.Attenuation0 = lit.Attenuation0;
	m_lit.Attenuation1 = lit.Attenuation1;
	m_lit.Attenuation2 = lit.Attenuation2;
	m_lit.Theta = lit.Theta;
	m_lit.Phi = lit.Phi;
}

// ±¤¿øÀÇ ¼Ò¸êÀÚ
CLight::~CLight()
{
}

// ±¤¿øÀ» È­¸é¿¡ ±×·Á³¿
bool CLight::create(IDirect3DDevice9* pDevice)
{
	if (pDevice == nullptr)
	{
		return false;
	}

	if (FAILED(D3DXCreateSphere(pDevice, m_bound._radius, 10, 10, &m_pMesh, NULL)))
	{
		return false;
	}

	return true;
}

// ±¤¿øÀ» È­¸é¿¡¼­ ¼Ò¸ê½ÃÅ´
void CLight::destroy()
{
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
}

// ±¤¿øÀÇ ¼Ó¼º°ªÀ» º¯°æÇÔ
void CLight::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (pDevice == nullptr)
	{
		return;
	}

	D3DXVECTOR3 pos(m_bound._center);
	D3DXVec3TransformCoord(&pos, &pos, &mLocal);
	D3DXVec3TransformCoord(&pos, &pos, &mWorld);
	m_lit.Position = pos;

	pDevice->SetLight(m_index, &m_lit);
	pDevice->LightEnable(m_index, TRUE);
}

// ±¤¿øÀÇ À§Ä¡¸¦ ¾ò¾î³¿
D3DXVECTOR3 CLight::getPosition()
{
	return D3DXVECTOR3(m_lit.Position);
}

