#ifndef _CHOLE_
#define _CHOLE_
#include "d3dUtility.h"
#include "CSphere.h"
#include "CObject.h"
#include "Hittable.h"

// -----------------------------------------------------------------------------
// CHole class definition
// ���� Ŭ������ ���� ����
// -----------------------------------------------------------------------------
class CHole : public CObject, public Hittable
{
private:
	float m_radius;
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pSphereMesh;
	D3DXMATRIX rotationMat;
	// DirectX������ ��ǥ�� ȭ�鿡 ���ؼ� ������ x��, ���� y��, ���������� z���̴�.

public:
	CHole();
	~CHole();

	bool create(IDirect3DDevice9* pDevice); // ������ ȭ�鿡 ������
	void destroy(); // ������ ȭ�鿡�� �Ҹ��Ŵ
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld); // ������ ȭ�鿡 �׷���
	bool hasIntersected(CSphere& ball) const; // ���ۿ� ���� ���� �ϴ��� Ȯ��
	void hitBy(CSphere& ball); // ������ �־�� �ϴ� ��� ���� �����ϰ� ���ÿ� �ִ´�.

	float getRadius() const; // ������ �������� �޾ƿ�						
};
#endif

