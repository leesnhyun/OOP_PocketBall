#ifndef _CLIGHT_
#define _CLIGHT_
#include "d3dUtility.h"

// -----------------------------------------------------------------------------
// CLight class definition
// ���� Ŭ������ ���� ����
// -----------------------------------------------------------------------------
class CLight {
private:
	DWORD               m_index;
	D3DXMATRIX          m_mLocal;
	D3DLIGHT9           m_lit;
	ID3DXMesh*          m_pMesh;
	d3d::BoundingSphere m_bound;
public:
	CLight();																		   // ������ ������
	~CLight();																		   // ������ �Ҹ���
	bool create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius = 0.1f); // ������ ȭ�鿡 �׷���
	void destroy();																	   // ������ ȭ�鿡�� �Ҹ��Ŵ
	bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);				   // ������ �Ӽ����� ������
	void draw(IDirect3DDevice9* pDevice);											   // ������ �׷���
	D3DXVECTOR3 getPosition();														   // ������ ��ġ�� ��
};

#endif