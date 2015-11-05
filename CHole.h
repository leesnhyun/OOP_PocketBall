#ifndef _CHOLE_
#define _CHOLE_
#include "d3dUtility.h"
#include "CSphere.h"

// -----------------------------------------------------------------------------
// CHole class definition
// ���� Ŭ������ ���� ����
// -----------------------------------------------------------------------------
class CHole {
private:
	float center_x, center_y, center_z;	// �߽� ��ǥ
	float m_radius;						// ������
	D3DXMATRIX m_mLocal;
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pSphereMesh;
	// DirectX������ ��ǥ�� ȭ�鿡 ���ؼ� ������ x��, ���� y��, ���������� z���̴�.

public:
	CHole();
	~CHole();
	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE);	// ������ ȭ�鿡 ������
	void destroy();															// ������ ȭ�鿡�� �Ҹ��Ŵ
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);			// ������ ȭ�鿡 �׷���
	bool hasIntersected(CSphere& ball);										// ���ۿ� ���� ���� �ϴ��� Ȯ��
	void hitBy(CSphere& ball);												// ������ �־�� �ϴ� ��� ���� �����ϰ� ���ÿ� �ִ´�.
	void setCenter(float x, float y, float z);								// ������ �߽� ��ǥ�� ������
	float getRadius() const;												// ������ �������� �޾ƿ�
	const D3DXMATRIX& getLocalTransform() const;							//
	void setLocalTransform(const D3DXMATRIX& mLocal);						//
	D3DXVECTOR3 getCenter(void) const;										// ������ �߽� ��ǥ�� ��ȯ��
};

#endif