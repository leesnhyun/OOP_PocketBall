#ifndef _CBORDER_
#define _CBORDER_

#include "d3dUtility.h"
#include "CObject.h"
#include <vector>

// -----------------------------------------------------------------------------
// CBorder class definition
// �籸��(����)�� �׵θ��� ������ Ŭ������ ���� ���� (3ds max�� maya�� �۾�)
// -----------------------------------------------------------------------------
class CBorder : public CObject
{
private:
	D3DMATERIAL9 m_mtrl;

	ID3DXMesh* mesh;
	std::vector<D3DMATERIAL9> Mtrls;
	std::vector<IDirect3DTexture9*> Textures;

public:
	CBorder(D3DXCOLOR color); // �׵θ��� ������
	~CBorder(); // �׵θ��� �Ҹ���

	bool create(IDirect3DDevice9* pDevice); // �׵θ��� ȭ�鿡 ������
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld); // �׵θ��� ȭ�鿡 �׷���
	void destroy(); // �׵θ��� ȭ�鿡�� �Ҹ��Ŵ
};

#endif

