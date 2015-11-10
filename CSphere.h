#ifndef _CSPHERE_
#define _CSPHERE_

#include "d3dUtility.h"
#include "Player.h"
#include "CObject.h"
#include "Hittable.h"

// -----------------------------------------------------------------------------
// CSphere class definition
// 구체(공) 클래스에 대한 정의
// -----------------------------------------------------------------------------
class CSphere : public CObject, public Hittable{
protected:
	float m_radius;						// 반지름
	float m_velocity_x;
	float m_velocity_z;
	IDirect3DTexture9* Tex = nullptr;			// 텍스쳐

	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pSphereMesh;
	// DirectX에서의 좌표는 화면에 대해서 화면을 마주 보는 사람의 입장에서 
	// 앞으로 z축, 위로 y축, 오른쪽으로 x축이다.

	const char* ballNumber;
	BallType ballType;
	float pre_center_x, pre_center_z; // 이전 위치 보관, 충돌 시에 사용해야 함

public:
	CSphere(BallType ballType, const char* number);
	~CSphere();
	bool create(IDirect3DDevice9* pDevice);				// 공을 화면에 생성함
	void destroy();															// 공을 화면에서 소멸시킴
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);			// 공을 화면에 그려냄
	virtual bool hasIntersected(CSphere& ball) = 0;							// 두 공이 충돌 했는지 확인
	virtual void hitBy(CSphere& ball) = 0;									// 공이 충돌한 경우, 두 공의 방향과 속도를 바꿈.
	void ballUpdate(float timeDiff);										// 공의 중심 좌표를 속도에 맞춰서 매 시간 간격마다 갱신함
	double getVelocity_X();													// 공의 x축 속도를 반환함
	double getVelocity_Z();													// 공의 z축 속도를 반환함
	void setPower(double vx, double vz);									// 공의 속도를 바꿈
	float getRadius() const;												// 공의 반지름을 받아옴
	BallType getBallType() const;

private:
	LPD3DXMESH _createMappedSphere(IDirect3DDevice9* pDev);

};

#endif