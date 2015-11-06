#ifndef _CWALL_
#define _CWALL_
#include "d3dUtility.h"
#include "CSphere.h"

// -----------------------------------------------------------------------------
// CWall class definition
// �� Ŭ������ ���� ����
// -----------------------------------------------------------------------------
class CWall {

private:
	float m_x;							// ���� �߽� x ��ǥ
	float m_z;							// ���� �߽� z ��ǥ
	float m_width;						// ���� �ʺ�
	float m_depth;						// ���� �β�
	float m_height;						// ���� ����
	void setLocalTransform(const D3DXMATRIX& mLocal);
	D3DXMATRIX m_mLocal;
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pBoundMesh;

public:
	CWall();							// ���� ������
	~CWall();							// ���� �Ҹ���
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE); // ���� ȭ�鿡 ������
	void destroy();																														 // ���� ȭ�鿡�� �Ҹ��Ŵ
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);																		 // ���� ȭ�鿡 �׷���
	bool hasIntersected(CSphere& ball);																									 // ���� ���� �浹 �ߴ��� Ȯ��
	void hitBy(CSphere& ball);																											 // ���� ���� �浹�� ��� ���� ����� �ӵ��� �ٲ�
	void setPosition(float x, float y, float z);																					     // ���� ��ġ�� �ٲ�
	float getHeight(void) const;																										 // ���� ���̸� �ٲ�
};

#endif