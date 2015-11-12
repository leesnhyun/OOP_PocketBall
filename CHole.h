#ifndef _CHOLE_
#define _CHOLE_
#include "d3dUtility.h"
#include "CSphere.h"
#include "CObject.h"
#include "Hittable.h"

// -----------------------------------------------------------------------------
// CHole class definition
// 구멍 클래스에 대한 정의
// -----------------------------------------------------------------------------
class CHole : public CObject, public Hittable
{
private:
	float m_radius;
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pSphereMesh;
	D3DXMATRIX rotationMat;
	// DirectX에서의 좌표는 화면에 대해서 앞으로 x축, 위로 y축, 오른쪽으로 z축이다.

public:
	CHole();
	~CHole();

	bool create(IDirect3DDevice9* pDevice); // 구멍을 화면에 생성함
	void destroy(); // 구멍을 화면에서 소멸시킴
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld); // 구멍을 화면에 그려냄
	bool hasIntersected(CSphere& ball) const; // 구멍에 공이 들어가야 하는지 확인
	void hitBy(CSphere& ball); // 구멍을 넣어야 하는 경우 공을 제거하고 스택에 넣는다.

	float getRadius() const; // 구멍의 반지름을 받아옴						
};
#endif

