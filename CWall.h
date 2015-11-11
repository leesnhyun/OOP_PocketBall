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

private:
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pBoundMesh;

protected:
	float m_width;						// ���� �ʺ�
	float m_depth;						// ���� �β�
	float m_height;						// ���� ����

public:
	static const float LOSS_RATIO;

	CWall(float iwidth, float iheight, float idepth, D3DXCOLOR color);							// ���� ������
	virtual ~CWall();							// ���� �Ҹ���
	bool create(IDirect3DDevice9* pDevice); // ���� ȭ�鿡 ������
	void destroy();							// ���� ȭ�鿡�� �Ҹ��Ŵ
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);// ���� ȭ�鿡 �׷���

	virtual bool hasIntersected(CSphere& ball) const noexcept = 0;// ���� ���� �浹 �ߴ��� Ȯ��
	virtual void hitBy(CSphere& ball) noexcept = 0;
	void adjustPosition(CSphere& ball);
};

#endif