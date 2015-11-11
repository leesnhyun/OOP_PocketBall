#ifndef _CFLOOR_
#define _CFLOOR_

#include "d3dUtility.h"
#include "CObject.h"

// -----------------------------------------------------------------------------
// CWall class definition
// 벽 클래스에 대한 정의
// -----------------------------------------------------------------------------
class CFloor : public CObject {

private:
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pBoundMesh;

protected:
	float m_width;							// 바닥의 너비
	float m_depth;							// 바닥의 두께
	float m_height;							// 바닥의 높이

public:
	CFloor(float iwidth, float iheight, float idepth, D3DXCOLOR color);				// 바닥의 생성자
	virtual ~CFloor();																// 바닥의 소멸자
	bool create(IDirect3DDevice9* pDevice);											// 바닥을 화면에 생성함
	void destroy();																	// 바닥을 화면에서 소멸시킴
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);					// 바닥을 화면에 그려냄
};

#endif