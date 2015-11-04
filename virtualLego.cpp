////////////////////////////////////////////////////////////////////////////////
//
// File: virtualLego.cpp
//
// Original Author: 박창현 Chang-hyeon Park, 
// Modified by Bong-Soo Sohn and Dong-Jun Kim
// 
// Originally programmed for Virtual LEGO. 
// Modified later to program for Virtual Billiard.
//        
////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>

IDirect3DDevice9* Device = NULL;

// 창의 크기
const int Width  = 1024;
const int Height = 768;

// 4개의 공의 위치를 초기화 함.
const float spherePos[4][2] = { {-2.7f,0} , {+2.4f,0} , {3.3f,0} , {-2.7f,-0.9f}}; 
// 4개의 공의 색상을 초기화 함.
const D3DXCOLOR sphereColor[4] = {d3d::RED, d3d::RED, d3d::YELLOW, d3d::WHITE};

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 0.9982

// -----------------------------------------------------------------------------
// CSphere class definition
// 구체(공) 클래스에 대한 정의
// -----------------------------------------------------------------------------

class CSphere {
private :
	float					center_x, center_y, center_z;// 중심 좌표
	float                   m_radius;// 반지름
	float					m_velocity_x;// x축 속도
	float					m_velocity_z;// z축 속도
										// DirectX에서의 좌표는 화면에 대해서 앞으로 x축, 위로 y축, 오른쪽으로 z축이다.

public:
	CSphere(void)// 공의 생성자
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_radius = 0;
		m_velocity_x = 0;
		m_velocity_z = 0;
		m_pSphereMesh = NULL;
	}
	~CSphere(void) {}// 공의 소멸자

public:
	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE)// 공을 화면에 생성함
	{
		if (NULL == pDevice)
			return false;
		
		m_mtrl.Ambient  = color;
		m_mtrl.Diffuse  = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power    = 5.0f;
		
		if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL)))
			return false;
		return true;
	}
	
	void destroy(void)// 공을 화면에서 소멸시킴 
	{
		if (m_pSphereMesh != NULL) {
			m_pSphereMesh->Release();
			m_pSphereMesh = NULL;
		}
	}

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)// 공을 화면에 그려냄
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pSphereMesh->DrawSubset(0);
	}
	
	bool hasIntersected(CSphere& ball) // 두 공이 충돌 했는지 확인
	{
		// Insert your code here.
		return false;
	}
	
	void hitBy(CSphere& ball) // 공이 충돌한 경우, 두 공의 방향과 속도를 바꿈.
	{ 
		/* 반드시 충돌을 확인하는 코드를 여기에 넣어야 함 */
		// Insert your code here.
	}

	void ballUpdate(float timeDiff) // 공의 중심 좌표를 속도에 맞춰서 매 시간 간격마다 갱신함
	{
		const float TIME_SCALE = 3.3;
		D3DXVECTOR3 cord = this->getCenter();
		double vx = abs(this->getVelocity_X());
		double vz = abs(this->getVelocity_Z());

		if(vx > 0.01 || vz > 0.01)
		{
			float tX = cord.x + TIME_SCALE*timeDiff*m_velocity_x;
			float tZ = cord.z + TIME_SCALE*timeDiff*m_velocity_z;

			//correction of position of ball
			/* Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
			if(tX >= (4.5 - M_RADIUS))
				tX = 4.5 - M_RADIUS;
			else if(tX <=(-4.5 + M_RADIUS))
				tX = -4.5 + M_RADIUS;
			else if(tZ <= (-3 + M_RADIUS))
				tZ = -3 + M_RADIUS;
			else if(tZ >= (3 - M_RADIUS))
				tZ = 3 - M_RADIUS;
			*/
			this->setCenter(tX, cord.y, tZ);
		}
		else { this->setPower(0,0);}
		//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
		double rate = 1 -  (1 - DECREASE_RATE)*timeDiff * 400;
		if(rate < 0 )
			rate = 0;
		this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);// 공이 움직일 때마다, 속도를 낮춤
	}

	double getVelocity_X() { return this->m_velocity_x;	} // 공의 x축 속도를 반환함
	double getVelocity_Z() { return this->m_velocity_z; } // 공의 z축 속도를 반환함

	void setPower(double vx, double vz) // 공의 속도를 바꿈
	{
		this->m_velocity_x = vx;
		this->m_velocity_z = vz;
	}

	void setCenter(float x, float y, float z) // 공의 중심 좌표를 변경함
	{
		D3DXMATRIX m;
		center_x=x;	center_y=y;	center_z=z;
		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}
	
	float getRadius(void)  const { return (float)(M_RADIUS);  }// 공의 반지름을 받아옴
	const D3DXMATRIX& getLocalTransform(void) const { return m_mLocal; }
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	D3DXVECTOR3 getCenter(void) const // 공의 중심 좌표를 반환함
	{
		D3DXVECTOR3 org(center_x, center_y, center_z);
		return org;
	}
	
private:
	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh*              m_pSphereMesh;
	
};



// -----------------------------------------------------------------------------
// CWall class definition
// 벽 클래스에 대한 정의
// -----------------------------------------------------------------------------

class CWall {

private:
	
	float					m_x;// 벽의 중심 x 좌표
	float					m_z;// 벽의 중심 z 좌표
	float                   m_width;// 벽의 너비
	float                   m_depth;// 벽의 두께
	float					m_height;// 벽의 높이
	
public:
	CWall(void)// 벽의 생성자
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_width = 0;
		m_depth = 0;
		m_pBoundMesh = NULL;
	}
	~CWall(void) {}// 벽의 소멸자
public:
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE)
		//벽을 화면에 생성함
	{
		if (NULL == pDevice)
			return false;
		
		m_mtrl.Ambient  = color;
		m_mtrl.Diffuse  = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power    = 5.0f;
		
		m_width = iwidth;
		m_depth = idepth;
		
		if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
			return false;
		return true;
	}
	void destroy(void)
		// 벽을 화면에서 소멸시킴
	{
		if (m_pBoundMesh != NULL) {
			m_pBoundMesh->Release();
			m_pBoundMesh = NULL;
		}
	}
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)// 벽을 화면에 그려냄
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pBoundMesh->DrawSubset(0);
	}
	
	bool hasIntersected(CSphere& ball) {// 벽에 공이 충돌 했는지 확인
		// Insert your code here.
		return false;
	}

	void hitBy(CSphere& ball) {// 벽에 공이 충돌할 경우 공의 방향과 속도를 바꿈
		// Insert your code here.
	}    
	
	void setPosition(float x, float y, float z)// 벽의 위치를 바꿈
	{
		D3DXMATRIX m;
		this->m_x = x;
		this->m_z = z;

		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}
	
	float getHeight(void) const { return M_HEIGHT; }// 벽의 높이를 바꿈
	
	
	
private :
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	
	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh*              m_pBoundMesh;
};

// -----------------------------------------------------------------------------
// CLight class definition
// 광원 클래스에 대한 정의
// -----------------------------------------------------------------------------

class CLight {
public:
	CLight(void)// 광원의 생성자
	{
		static DWORD i = 0;
		m_index = i++;
		D3DXMatrixIdentity(&m_mLocal);
		::ZeroMemory(&m_lit, sizeof(m_lit));
		m_pMesh = NULL;
		m_bound._center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_bound._radius = 0.0f;
	}
	~CLight(void) {}// 광원의 소멸자
public:
	bool create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius = 0.1f)
		// 광원을 화면에 그려냄
	{
		if (NULL == pDevice)
			return false;
		if (FAILED(D3DXCreateSphere(pDevice, radius, 10, 10, &m_pMesh, NULL)))
			return false;
		
		m_bound._center = lit.Position;
		m_bound._radius = radius;
		
		m_lit.Type          = lit.Type;
		m_lit.Diffuse       = lit.Diffuse;
		m_lit.Specular      = lit.Specular;
		m_lit.Ambient       = lit.Ambient;
		m_lit.Position      = lit.Position;
		m_lit.Direction     = lit.Direction;
		m_lit.Range         = lit.Range;
		m_lit.Falloff       = lit.Falloff;
		m_lit.Attenuation0  = lit.Attenuation0;
		m_lit.Attenuation1  = lit.Attenuation1;
		m_lit.Attenuation2  = lit.Attenuation2;
		m_lit.Theta         = lit.Theta;
		m_lit.Phi           = lit.Phi;
		return true;
	}
	void destroy(void)
		// 광원을 화면에서 소멸시킴
	{
		if (m_pMesh != NULL) {
			m_pMesh->Release();
			m_pMesh = NULL;
		}
	}
	bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
		// 광원의 속성값을 변경함
	{
		if (NULL == pDevice)
			return false;
		
		D3DXVECTOR3 pos(m_bound._center);
		D3DXVec3TransformCoord(&pos, &pos, &m_mLocal);
		D3DXVec3TransformCoord(&pos, &pos, &mWorld);
		m_lit.Position = pos;
		
		pDevice->SetLight(m_index, &m_lit);
		pDevice->LightEnable(m_index, TRUE);
		return true;
	}

	void draw(IDirect3DDevice9* pDevice)
		// 광원을 그려냄
	{
		if (NULL == pDevice)
			return;
		D3DXMATRIX m;
		D3DXMatrixTranslation(&m, m_lit.Position.x, m_lit.Position.y, m_lit.Position.z);
		pDevice->SetTransform(D3DTS_WORLD, &m);
		pDevice->SetMaterial(&d3d::WHITE_MTRL);
		m_pMesh->DrawSubset(0);
	}

	D3DXVECTOR3 getPosition(void) const { return D3DXVECTOR3(m_lit.Position); }// 광원의 위치를 얻어냄

private:
	DWORD               m_index;
	D3DXMATRIX          m_mLocal;
	D3DLIGHT9           m_lit;
	ID3DXMesh*          m_pMesh;
	d3d::BoundingSphere m_bound;
};


// -----------------------------------------------------------------------------
// Global variables
// 전역 변수
// -----------------------------------------------------------------------------
CWall	g_legoPlane;
CWall	g_legowall[4];
CSphere	g_sphere[4];
CSphere	g_target_blueball;
CLight	g_light;

double g_camera_pos[3] = {0.0, 5.0, -8.0};

// -----------------------------------------------------------------------------
// Functions
// 전역 함수
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{
}

// initialization
// 초기화 하는 함수
bool Setup()
{
	int i;
	
	D3DXMatrixIdentity(&g_mWorld);
	D3DXMatrixIdentity(&g_mView);
	D3DXMatrixIdentity(&g_mProj);
		
	// 초록색 바닥을 생성
	if (false == g_legoPlane.create(Device, -1, -1, 9, 0.03f, 6, d3d::GREEN)) return false;
	g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);
	
	// 벽을 생성
	if (false == g_legowall[0].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, 3.06f);
	if (false == g_legowall[1].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, -3.06f);
	if (false == g_legowall[2].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::DARKRED)) return false;
	g_legowall[2].setPosition(4.56f, 0.12f, 0.0f);
	if (false == g_legowall[3].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::DARKRED)) return false;
	g_legowall[3].setPosition(-4.56f, 0.12f, 0.0f);

	// 4개의 공을 생성함
	for (i=0;i<4;i++) {
		if (false == g_sphere[i].create(Device, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS , spherePos[i][1]);
		g_sphere[i].setPower(0,0);
	}
	
	// 파란색 공을 생성함
	if (false == g_target_blueball.create(Device, d3d::BLUE)) return false;
	g_target_blueball.setCenter(.0f, (float)M_RADIUS , .0f);
	
	// 광원 설정
	D3DLIGHT9 lit;
	::ZeroMemory(&lit, sizeof(lit));
	lit.Type         = D3DLIGHT_POINT;
	lit.Diffuse      = d3d::WHITE; 
	lit.Specular     = d3d::WHITE * 0.9f;
	lit.Ambient      = d3d::WHITE * 0.9f;
	lit.Position     = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	lit.Range        = 100.0f;
	lit.Attenuation0 = 0.0f;
	lit.Attenuation1 = 0.9f;
	lit.Attenuation2 = 0.0f;
	if (false == g_light.create(Device, lit))
		return false;
	
	// 카메라의 위치와 시야를 설정함
	D3DXVECTOR3 pos(0.0f, 5.0f, -8.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);
	
	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
		(float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
	
	// Set render states.
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	
	g_light.setLight(Device, g_mWorld);
	return true;
}

// 화면에서 전부 소멸시킴
void Cleanup(void)
{
	g_legoPlane.destroy();
	for(int i = 0 ; i < 4; i++) {
		g_legowall[i].destroy();
	}
	destroyAllLegoBlock();
	g_light.destroy();
}


// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"

// timeDelta는 이전 이미지 프레임과 현재 이미지 프레임의 사이의 시간값을 나타냅니다.
// 공이 움직이는 거리는 무조건 "속도 * timeDelta"여야 합니다.

bool Display(float timeDelta)// 한 프레임에 해당되는 화면을 보여줌
{
	int i=0;
	int j = 0;


	if( Device )
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();
		
		// update the position of each ball. during update, check whether each ball hit by walls.
		// 공의 위치를 갱신한다. 갱신하는 중에는 각각의 공이 벽과 충돌 했는지 확인한다.
		for( i = 0; i < 4; i++) {
			g_sphere[i].ballUpdate(timeDelta);
			for(j = 0; j < 4; j++){ g_legowall[i].hitBy(g_sphere[j]); }
		}

		// check whether any two balls hit together and update the direction of balls
		// 각각의 공에 대해, 다른 공과 서로 충돌 했는지 확인하고, 공의 방향을 갱신한다.
		for(i = 0 ;i < 4; i++){
			for(j = 0 ; j < 4; j++) {
				if(i >= j) {continue;}
				g_sphere[i].hitBy(g_sphere[j]);
			}
		}

		// draw plane, walls, and spheres
		// 초록색 판을 그리고, 벽을 그리고, 공들을 그린다.
		g_legoPlane.draw(Device, g_mWorld);
		for (i=0;i<4;i++) 	{
			g_legowall[i].draw(Device, g_mWorld);
			g_sphere[i].draw(Device, g_mWorld);
		}
		g_target_blueball.draw(Device, g_mWorld);
		g_light.draw(Device);
		
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture( 0, NULL );
	}
	return true;
}

// 마우스 또는 키보드 입력 이벤트 처리 함수
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool wire = false;
	static bool isReset = true;
	static int old_x = 0;
	static int old_y = 0;
	static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;
	
	switch( msg ) {
	case WM_DESTROY:
		{
			::PostQuitMessage(0);
			break;
		}
	case WM_KEYDOWN://키를 누르고 있는 동안
		{
			switch (wParam) {
			case VK_ESCAPE: // ESC 키의 경우 화면을 종료한다
				::DestroyWindow(hwnd);
				break;
			case VK_RETURN: //
				if (NULL != Device) {
					wire = !wire;
					Device->SetRenderState(D3DRS_FILLMODE,
						(wire ? D3DFILL_WIREFRAME : D3DFILL_SOLID));
				}
				break;
			case VK_SPACE:	// 스페이스 바의 경우 파란 공과 하얀 공의 위치를 받아서
							// 그 거리와 방향만큼 하얀 공의 속도를 조정한다.
					D3DXVECTOR3 targetpos = g_target_blueball.getCenter();
					D3DXVECTOR3	whitepos = g_sphere[3].getCenter();
					double theta = acos(sqrt(pow(targetpos.x - whitepos.x, 2)) / sqrt(pow(targetpos.x - whitepos.x, 2) +
						pow(targetpos.z - whitepos.z, 2)));		// 기본 1 사분면
					if(targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x >= 0) { theta = -theta; }	//4 사분면
					if(targetpos.z - whitepos.z >= 0 && targetpos.x - whitepos.x <= 0) { theta = PI - theta; } //2 사분면
					if(targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x <= 0){ theta = PI + theta; } // 3 사분면
					double distance = sqrt(pow(targetpos.x - whitepos.x, 2) + pow(targetpos.z - whitepos.z, 2));
					g_sphere[3].setPower(distance * cos(theta) , distance * sin(theta));
				break;
			}
			break;
		}
		
	case WM_MOUSEMOVE:// 마우스가 움직일 때,
		{
			int new_x = LOWORD(lParam);
			int new_y = HIWORD(lParam);
			float dx;
			float dy;
			
			if (LOWORD(wParam) & MK_LBUTTON) {// 마우스 왼쪽 버튼을 누를 때는, 당구판을 회전시킨다.
				
				if (isReset) {
					isReset = false;
				} else {
					D3DXVECTOR3 vDist;
					D3DXVECTOR3 vTrans;
					D3DXMATRIX mTrans;
					D3DXMATRIX mX;
					D3DXMATRIX mY;
					
					switch (move) {
					case WORLD_MOVE:
						dx = (old_x - new_x) * 0.01f;
						dy = (old_y - new_y) * 0.01f;
						D3DXMatrixRotationY(&mX, dx);
						D3DXMatrixRotationX(&mY, dy);
						g_mWorld = g_mWorld * mX * mY;
						
						break;
					}
				}
				
				old_x = new_x;
				old_y = new_y;

			} else {
				isReset = true;
				
				if (LOWORD(wParam) & MK_RBUTTON) {// 마우스 오른쪽 버튼을 누를 때는, 파란 공의 위치를 옮긴다.
					dx = (old_x - new_x);// * 0.01f;
					dy = (old_y - new_y);// * 0.01f;
		
					D3DXVECTOR3 coord3d=g_target_blueball.getCenter();
					g_target_blueball.setCenter(coord3d.x+dx*(-0.007f),coord3d.y,coord3d.z+dy*0.007f );
				}
				old_x = new_x;
				old_y = new_y;
				
				move = WORLD_MOVE;
			}
			break;
		}
	}
	
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

// 윈도우 창을 생성하고 화면 출력을 함
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	srand(static_cast<unsigned int>(time(NULL)));
	
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
	
	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}
	
	d3d::EnterMsgLoop( Display );
	
	Cleanup();
	
	Device->Release();
	
	return 0;
}