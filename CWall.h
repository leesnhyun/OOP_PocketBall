#ifndef _CWALL_
#define _CWALL_
#include "d3dUtility.h"
#include "CSphere.h"

// -----------------------------------------------------------------------------
// CWall class definition
// 벽 클래스에 대한 정의
// -----------------------------------------------------------------------------
class CWall {

private:
	float m_x;							// 벽의 중심 x 좌표
	float m_z;							// 벽의 중심 z 좌표
	float m_width;						// 벽의 너비
	float m_depth;						// 벽의 두께
	float m_height;						// 벽의 높이
	void setLocalTransform(const D3DXMATRIX& mLocal);
	D3DXMATRIX m_mLocal;
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pBoundMesh;

public:
	CWall();							// 벽의 생성자
	~CWall();							// 벽의 소멸자
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE); // 벽을 화면에 생성함
	void destroy();																														 // 벽을 화면에서 소멸시킴
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);																		 // 벽을 화면에 그려냄
	bool hasIntersected(CSphere& ball);																									 // 벽에 공이 충돌 했는지 확인
	void hitBy(CSphere& ball);																											 // 벽에 공이 충돌할 경우 공의 방향과 속도를 바꿈
	void setPosition(float x, float y, float z);																					     // 벽의 위치를 바꿈
	float getHeight(void) const;																										 // 벽의 높이를 바꿈
};

#endif