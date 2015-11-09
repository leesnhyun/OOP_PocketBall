#ifndef _CSPHERE_
#define _CSPHERE_

#include "d3dUtility.h"
#include "Player.h"

// -----------------------------------------------------------------------------
// CSphere class definition
// 구체(공) 클래스에 대한 정의
// -----------------------------------------------------------------------------
class CSphere {
private:
	float center_x, center_y, center_z;	// 중심 좌표
	float m_radius;						// 반지름
	float m_velocity_x;					// x축 속도
	float m_velocity_y;					// y축 속도
	float m_velocity_z;					// z축 속도

	IDirect3DTexture9* Tex = 0;			// 텍스쳐

	D3DXMATRIX m_mLocal;
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pSphereMesh;
	// DirectX에서의 좌표는 화면에 대해서 화면을 마주 보는 사람의 입장에서 
	// 앞으로 z축, 위로 y축, 오른쪽으로 x축이다.

	int deadDate;
	BallType ballType;
	float pre_center_x, pre_center_z;

public:
	static const float STOP_SPEED;
	CSphere(BallType ballType);
	~CSphere();
	bool isDead() const;													// 공의 죽은 여부를 가져옴.
	int getDeadDate() const;												// 공이 죽은 시점을 가져옴.
	void holeIn();															// 들어감
	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE);	// 공을 화면에 생성함
	void destroy();															// 공을 화면에서 소멸시킴
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);			// 공을 화면에 그려냄
	bool hasIntersected(CSphere& ball);										// 두 공이 충돌 했는지 확인
	void hitBy(CSphere& ball);												// 공이 충돌한 경우, 두 공의 방향과 속도를 바꿈.
	void ballUpdate(float timeDiff);										// 공의 중심 좌표를 속도에 맞춰서 매 시간 간격마다 갱신함
	double getVelocity_X();													// 공의 x축 속도를 반환함
	double getVelocity_Z();													// 공의 z축 속도를 반환함
	void setPower(double vx, double vz);									// 공의 속도를 바꿈
	void setCenter(float x, float y, float z);								// 공의 중심 좌표를 변경함
	float getRadius() const;												// 공의 반지름을 받아옴
	const D3DXMATRIX& getLocalTransform() const;							//
	void setLocalTransform(const D3DXMATRIX& mLocal);						//
	D3DXVECTOR3 getCenter(void) const;										// 공의 중심 좌표를 반환함

private:
	LPD3DXMESH _createMappedSphere(IDirect3DDevice9* pDev);

};

#endif