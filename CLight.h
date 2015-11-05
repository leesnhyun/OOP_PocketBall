#ifndef _CLIGHT_
#define _CLIGHT_
#include "d3dUtility.h"

// -----------------------------------------------------------------------------
// CLight class definition
// 광원 클래스에 대한 정의
// -----------------------------------------------------------------------------
class CLight {
private:
	DWORD               m_index;
	D3DXMATRIX          m_mLocal;
	D3DLIGHT9           m_lit;
	ID3DXMesh*          m_pMesh;
	d3d::BoundingSphere m_bound;
public:
	CLight();																		   // 광원의 생성자
	~CLight();																		   // 광원의 소멸자
	bool create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius = 0.1f); // 광원을 화면에 그려냄
	void destroy();																	   // 광원을 화면에서 소멸시킴
	bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);				   // 광원의 속성값을 변경함
	void draw(IDirect3DDevice9* pDevice);											   // 광원을 그려냄
	D3DXVECTOR3 getPosition();														   // 광원의 위치를 얻어냄
};

#endif