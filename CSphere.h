#ifndef _CSPHERE_
#define _CSPHERE_

#include "d3dUtility.h"
#include "Player.h"
#include "CObject.h"
#include "Hittable.h"

// -----------------------------------------------------------------------------
// CSphere class definition
// ��ü(��) Ŭ������ ���� ����
// -----------------------------------------------------------------------------
class CSphere : public CObject, public Hittable{
protected:
	float m_radius;						// ������
	float m_velocity_x;
	float m_velocity_z;
	IDirect3DTexture9* Tex = nullptr;			// �ؽ���

	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pSphereMesh;
	// DirectX������ ��ǥ�� ȭ�鿡 ���ؼ� ȭ���� ���� ���� ����� ���忡�� 
	// ������ z��, ���� y��, ���������� x���̴�.

	const char* ballNumber;
	BallType ballType;
	float pre_center_x, pre_center_z; // ���� ��ġ ����, �浹 �ÿ� ����ؾ� ��

public:
	CSphere(BallType ballType, const char* number);
	~CSphere();
	bool create(IDirect3DDevice9* pDevice);				// ���� ȭ�鿡 ������
	void destroy();															// ���� ȭ�鿡�� �Ҹ��Ŵ
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);			// ���� ȭ�鿡 �׷���
	virtual bool hasIntersected(CSphere& ball) = 0;							// �� ���� �浹 �ߴ��� Ȯ��
	virtual void hitBy(CSphere& ball) = 0;									// ���� �浹�� ���, �� ���� ����� �ӵ��� �ٲ�.
	void ballUpdate(float timeDiff);										// ���� �߽� ��ǥ�� �ӵ��� ���缭 �� �ð� ���ݸ��� ������
	double getVelocity_X();													// ���� x�� �ӵ��� ��ȯ��
	double getVelocity_Z();													// ���� z�� �ӵ��� ��ȯ��
	void setPower(double vx, double vz);									// ���� �ӵ��� �ٲ�
	float getRadius() const;												// ���� �������� �޾ƿ�
	BallType getBallType() const;

private:
	LPD3DXMESH _createMappedSphere(IDirect3DDevice9* pDev);

};

#endif