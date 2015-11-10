#include "d3dUtility.h"
#include "CLight.h"

// 광원의 생성자
CLight::CLight(const D3DLIGHT9& lit, float radius)
{
	static DWORD i = 0;
	m_index = i++;
	D3DXMatrixIdentity(&m_mLocal);
	::ZeroMemory(&m_lit, sizeof(m_lit));
	m_pMesh = NULL;
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

// 광원의 소멸자
CLight::~CLight()
{

}

// 광원을 화면에 그려냄
bool CLight::create(IDirect3DDevice9* pDevice)
{
	if (NULL == pDevice)
	{
		return false;
	}

	if (FAILED(D3DXCreateSphere(pDevice, m_bound._radius, 10, 10, &m_pMesh, NULL)))
	{
		return false;
	}

	return true;
}

// 광원을 화면에서 소멸시킴
void CLight::destroy()
{
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
}

// 광원의 속성값을 변경함
void CLight::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (NULL == pDevice)
	{
		return;
	}

	D3DXVECTOR3 pos(m_bound._center);
	D3DXVec3TransformCoord(&pos, &pos, &m_mLocal);
	D3DXVec3TransformCoord(&pos, &pos, &mWorld);
	m_lit.Position = pos;

	pDevice->SetLight(m_index, &m_lit);
	pDevice->LightEnable(m_index, TRUE);
}

// 광원의 위치를 얻어냄
D3DXVECTOR3 CLight::getPosition()
{
	return D3DXVECTOR3(m_lit.Position);
}