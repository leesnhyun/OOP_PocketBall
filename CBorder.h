#ifndef _CBORDER_
#define _CBORDER_
#include "d3dUtility.h"
#include <vector>

// -----------------------------------------------------------------------------
// CBorder class definition
// �籸��(����)�� �׵θ��� ������ Ŭ������ ���� ���� (3ds max�� maya�� �۾�)
// -----------------------------------------------------------------------------
class CBorder {

private:
	float m_x;							// �׵θ��� �߽� x ��ǥ
	float m_z;							// �׵θ��� �߽� z ��ǥ
	void setLocalTransform(const D3DXMATRIX& mLocal);

	D3DXMATRIX m_mLocal;
	D3DMATERIAL9 m_mtrl;

	ID3DXMesh* mesh;
	std::vector<D3DMATERIAL9>       Mtrls;
	std::vector<IDirect3DTexture9*> Textures;

public:
	CBorder();							// �׵θ��� ������
	~CBorder();							// �׵θ��� �Ҹ���
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float idepth, D3DXCOLOR color = d3d::WHITE); // ���� ȭ�鿡 ������
	void destroy();																													 // ���� ȭ�鿡�� �Ҹ��Ŵ
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);																		 // ���� ȭ�鿡 �׷���																										 // ���� ���� �浹�� ��� ���� ����� �ӵ��� �ٲ�
	void setPosition(float x, float y, float z);																					     // ���� ��ġ�� �ٲ�
};

#endif