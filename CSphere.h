#ifndef _CSPHERE_
#define _CSPHERE_

#include "d3dUtility.h"
#include "Player.h"
#include "CObject.h"
#include "CWall.h"
#include "Hittable.h"
#include <string>

using std::string;

// -----------------------------------------------------------------------------
// CSphere class definition
// 구체(공) 클래스에 대한 정의
// -----------------------------------------------------------------------------
class CSphere : public CObject, public Hittable
{
private:
	IDirect3DTexture9* Tex = nullptr;
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pSphereMesh;
	D3DXMATRIX matBallRoll;
	string ballImageFileName;
	BallType ballType;
protected:
	typedef CSphere super;
	int disableTurn = -1;
	float m_radius;
	float m_velocity_x;
	float m_velocity_z;
	float pre_center_x, pre_center_z; // 이전 위치 보관, 충돌 시에 사용해야 함

public:
	static const float COMMON_RADIUS;
	CSphere(BallType ballType, const char* ballImageFileName);
	virtual ~CSphere();
	bool create(IDirect3DDevice9* pDevice); // 공을 화면에 생성함
	void destroy(); // 공을 화면에서 소멸시킴
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld); // 공을 화면에 그려냄
	virtual bool hasIntersected(CSphere& ball) const noexcept; // 두 공이 충돌 했는지 확인
	virtual void hitBy(CSphere& ball) noexcept; // 공이 충돌한 경우, 두 공의 방향과 속도를 바꿈.
	void ballUpdate(float timeDiff); // 공의 중심 좌표를 속도에 맞춰서 매 시간 간격마다 갱신함
	double getVelocity_X() const; // 공의 x축 속도를 반환함
	double getVelocity_Z() const; // 공의 z축 속도를 반환함
	void setPower(double vx, double vz); // 공의 속도를 바꿈
	float getRadius() const; // 공의 반지름을 받아옴
	BallType getBallType() const; // 공의 종류를 반환
	void disable() noexcept; // 공을 비활성화 함
	void enable() noexcept; // 공을 활성화 함
	int getDisableTurn() const noexcept; // 공이 비활성화 된 턴을 반환
	bool isDisabled() const noexcept; // 공이 비활성화 되었는지 반환
	void adjustPosition(CSphere& ball); // 충돌 후, 공의 위치를 조정함
	double getPreCenter_x() const; // 공의 이전 x 위치를 받아옴
	double getPreCenter_z() const; // 공의 이전 z 위치를 받아옴


private:
	LPD3DXMESH _createMappedSphere(IDirect3DDevice9* pDev);
};

#endif

