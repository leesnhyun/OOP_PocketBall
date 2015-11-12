#ifndef _CLIGHT_
#define _CLIGHT_
#include "d3dUtility.h"
#include "CObject.h"

// -----------------------------------------------------------------------------
// CLight class definition
// ���� Ŭ������ ���� ����
// -----------------------------------------------------------------------------
class CLight : public CObject
{
private:
	DWORD m_index;
	D3DLIGHT9 m_lit;
	ID3DXMesh* m_pMesh;
	d3d::BoundingSphere m_bound;
public:
	CLight(float radius = 0.1f); // ������ ������
	~CLight(); // ������ �Ҹ���
	bool create(IDirect3DDevice9* pDevice); // ������ ȭ�鿡 �׷���
	void destroy(); // ������ ȭ�鿡�� �Ҹ��Ŵ
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld); // ������ �Ӽ����� ������
	D3DXVECTOR3 getPosition(); // ������ ��ġ�� ��
};

#endif

