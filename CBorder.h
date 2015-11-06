#ifndef _CBORDER_
#define _CBORDER_
#include "d3dUtility.h"
#include <vector>

// -----------------------------------------------------------------------------
// CBorder class definition
// 당구대(다이)의 테두리에 관련한 클래스에 대한 정의 (3ds max와 maya로 작업)
// -----------------------------------------------------------------------------
class CBorder {

private:
	float m_x;							// 테두리의 중심 x 좌표
	float m_z;							// 테두리의 중심 z 좌표
	void setLocalTransform(const D3DXMATRIX& mLocal);

	D3DXMATRIX m_mLocal;
	D3DMATERIAL9 m_mtrl;

	ID3DXMesh* mesh;
	std::vector<D3DMATERIAL9>       Mtrls;
	std::vector<IDirect3DTexture9*> Textures;

public:
	CBorder();							// 테두리의 생성자
	~CBorder();							// 테두리의 소멸자
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float idepth, D3DXCOLOR color = d3d::WHITE); // 벽을 화면에 생성함
	void destroy();																													 // 벽을 화면에서 소멸시킴
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);																		 // 벽을 화면에 그려냄																										 // 벽에 공이 충돌할 경우 공의 방향과 속도를 바꿈
	void setPosition(float x, float y, float z);																					     // 벽의 위치를 바꿈
};

#endif