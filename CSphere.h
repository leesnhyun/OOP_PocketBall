#ifndef _CSPHERE_
#define _CSPHERE_

#include "d3dUtility.h"
#include "Player.h"

// -----------------------------------------------------------------------------
// CSphere class definition
// ��ü(��) Ŭ������ ���� ����
// -----------------------------------------------------------------------------
class CSphere {
private:
	float center_x, center_y, center_z;	// �߽� ��ǥ
	float m_radius;						// ������
	float m_velocity_x;					// x�� �ӵ�
	float m_velocity_y;					// y�� �ӵ�
	float m_velocity_z;					// z�� �ӵ�

	IDirect3DTexture9* Tex = 0;			// �ؽ���

	D3DXMATRIX m_mLocal;
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pSphereMesh;
	// DirectX������ ��ǥ�� ȭ�鿡 ���ؼ� ȭ���� ���� ���� ����� ���忡�� 
	// ������ z��, ���� y��, ���������� x���̴�.

	int deadDate;
	BallType ballType;
	float pre_center_x, pre_center_z;

public:
	static const float STOP_SPEED;
	CSphere(BallType ballType);
	~CSphere();
	bool isDead() const;													// ���� ���� ���θ� ������.
	int getDeadDate() const;												// ���� ���� ������ ������.
	void holeIn();															// ��
	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE);	// ���� ȭ�鿡 ������
	void destroy();															// ���� ȭ�鿡�� �Ҹ��Ŵ
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);			// ���� ȭ�鿡 �׷���
	bool hasIntersected(CSphere& ball);										// �� ���� �浹 �ߴ��� Ȯ��
	void hitBy(CSphere& ball);												// ���� �浹�� ���, �� ���� ����� �ӵ��� �ٲ�.
	void ballUpdate(float timeDiff);										// ���� �߽� ��ǥ�� �ӵ��� ���缭 �� �ð� ���ݸ��� ������
	double getVelocity_X();													// ���� x�� �ӵ��� ��ȯ��
	double getVelocity_Z();													// ���� z�� �ӵ��� ��ȯ��
	void setPower(double vx, double vz);									// ���� �ӵ��� �ٲ�
	void setCenter(float x, float y, float z);								// ���� �߽� ��ǥ�� ������
	float getRadius() const;												// ���� �������� �޾ƿ�
	const D3DXMATRIX& getLocalTransform() const;							//
	void setLocalTransform(const D3DXMATRIX& mLocal);						//
	D3DXVECTOR3 getCenter(void) const;										// ���� �߽� ��ǥ�� ��ȯ��

private:
	LPD3DXMESH _createMappedSphere(IDirect3DDevice9* pDev);

};

#endif