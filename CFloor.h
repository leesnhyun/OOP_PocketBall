#ifndef _CFLOOR_
#define _CFLOOR_

#include "d3dUtility.h"
#include "CObject.h"

// -----------------------------------------------------------------------------
// CWall class definition
// �� Ŭ������ ���� ����
// -----------------------------------------------------------------------------
class CFloor : public CObject {

private:
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pBoundMesh;

protected:
	float m_width;							// �ٴ��� �ʺ�
	float m_depth;							// �ٴ��� �β�
	float m_height;							// �ٴ��� ����

public:
	CFloor(float iwidth, float iheight, float idepth, D3DXCOLOR color);				// �ٴ��� ������
	virtual ~CFloor();																// �ٴ��� �Ҹ���
	bool create(IDirect3DDevice9* pDevice);											// �ٴ��� ȭ�鿡 ������
	void destroy();																	// �ٴ��� ȭ�鿡�� �Ҹ��Ŵ
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);					// �ٴ��� ȭ�鿡 �׷���
};

#endif