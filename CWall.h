#ifndef _CWALL_
#define _CWALL_
#include "d3dUtility.h"
#include "CSphere.h"
#include "CObject.h"
#include "Hittable.h"

// -----------------------------------------------------------------------------
// CWall class definition
// �� Ŭ������ ���� ����
// -----------------------------------------------------------------------------
class CWall : public CObject, public Hittable {
protected:
	float m_width;						// ���� �ʺ�
	float m_depth;						// ���� �β�
	float m_height;						// ���� ����
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pBoundMesh;

public:
	static const float LOSS_RATIO;

	CWall(float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color);							// ���� ������
	~CWall();							// ���� �Ҹ���
	bool create(IDirect3DDevice9* pDevice); // ���� ȭ�鿡 ������
	void destroy();							// ���� ȭ�鿡�� �Ҹ��Ŵ
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);// ���� ȭ�鿡 �׷���
	virtual bool hasIntersected(CSphere& ball) = 0;// ���� ���� �浹 �ߴ��� Ȯ��
	virtual void hitBy(CSphere& ball) = 0;
};

#endif