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
#include "CSphere.h"
#include "CLight.h"
#include "CWall.h"
#include "CHole.h"
#include "CBorder.h"
#include "TurnManager.h"
#include "Player.h"

#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>

IDirect3DDevice9* Device = NULL;

// 창의 크기
const int Width  = 1024;
const int Height = 768;

// 16개의 공의 위치를 초기화 함.
const float spherePos[16][2] = { { -2.7f, 0 }, { +1.5, 0 }, { 1.88f, 0.211f }, { 1.88f, -0.211f }, { 3.02f, -0.844f }, 
{ 2.26f, -0.422f }, { 2.26f, 0.422f }, { 2.64f, -0.211f }, { 2.64f, 0.211f }, { 2.64f, -0.633 }, { 2.64f, 0.633f }, 
{ 3.02f, -0.422f }, { 3.02f, 0.422f }, { 3.02f, 0 }, { 3.02f, 0.844f }, { 2.26, 0} };

// 6개의 구멍의 위치를 초기화 함.
// const float holePos[6][2] = { {-4.23f,-2.73f}, {0,-2.73f}, {4.23f,-2.73f}, {-4.23f,2.73f}, {0,2.73f}, {4.23f,2.73f}};
const float holePos[6][2] = {
								{-4.52f,-3.00f}, {0,-3.23f}, {4.73f,-3.23f}, 
								{-4.52f,3.00f}, {0,3.23f}, {4.73f,3.23f}
							};

// 4개의 공의 색상을 초기화 함.
const D3DXCOLOR sphereColor[16] = { d3d::WHITE, d3d::RED, d3d::YELLOW, d3d::RED, d3d::YELLOW, d3d::YELLOW, d3d::RED, 
d3d::YELLOW, d3d::RED, d3d::RED, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::RED, d3d::RED, d3d::BLACK };

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;


// -----------------------------------------------------------------------------
// Global variables
// 전역 변수
// -----------------------------------------------------------------------------
CWall	g_legoPlane;
CWall	g_legowall[4];
CSphere	g_sphere[16];
CSphere	g_target_blueball;
CLight	g_light;
CHole	g_hole[6];

CBorder g_frame;

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
	
	// 프레임생성
	if (false == g_frame.create(Device, -1, -1, 9, d3d::YELLOW)) return false;
	g_frame.setPosition(0.115f, -0.44f, 0.00f);

	// 초록색 바닥을 생성
	if (false == g_legoPlane.create(Device, -1, -1, 9, 0.03f, 6, false, d3d::TABLE_PANE)) return false;
	g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);
	
	//// 벽을 생성
	// 가로벽 (9*0.3f*0.15) , (0, 0.12, 3.06)
	if (false == g_legowall[0].create(Device, -1, -1, 4.5, 0.3f, 0.15f, false, d3d::TABLE_WALL)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, 3.06f);
	if (false == g_legowall[1].create(Device, -1, -1, 9, 0.3f, 0.15f, false, d3d::TABLE_WALL)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, -3.06f);

	// 세로벽 (0.15f*0.3f*6.24f) , (4.56, 0.12, 0)
	if (false == g_legowall[2].create(Device, -1, -1, 0.15f, 0.3f, 5.55f, true, d3d::TABLE_WALL)) return false;
	g_legowall[2].setPosition(4.56f, 0.12f, 0.0f);
	if (false == g_legowall[3].create(Device, -1, -1, 0.15f, 0.3f, 5.55f, true, d3d::TABLE_WALL)) return false;
	g_legowall[3].setPosition(-4.56f, 0.12f, 0.0f);
	////

	// 16개의 공을 생성함
	for (i=0;i<16;i++) {
		if (false == g_sphere[i].create(Device, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS , spherePos[i][1]);
		g_sphere[i].setPower(0,0);
	}

	// 6개의 구멍을 생성함
	for (i = 0; i<6; i++) {
		if (false == g_hole[i].create(Device, d3d::BLACK)) return false;
		g_hole[i].setCenter(holePos[i][0], 0, holePos[i][1]);
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
	lit.Position     = D3DXVECTOR3(0.0f, 3.0f, -2.0f);
	lit.Range        = 150.0f;
	lit.Attenuation0 = 0.0f;
	lit.Attenuation1 = 0.9f;
	lit.Attenuation2 = 0.0f;
	if (false == g_light.create(Device, lit))
		return false;
	
	// 카메라의 위치와 시야를 설정함
	D3DXVECTOR3 pos(0.0f, 10.0f, 0.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 0.0f, 2.0f);
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

Player players[2] = { Player(1), Player(2) };
TurnManager turnManager({ players[0].getPlayerId(), players[1].getPlayerId() });

bool Display(float timeDelta)// 한 프레임에 해당되는 화면을 보여줌
{
	int i = 0;
	int j = 0;


	if( Device )
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();
		
		// update the position of each ball. during update, check whether each ball hit by walls.
		// 공의 위치를 갱신한다. 갱신하는 중에는 각각의 공이 벽과 충돌 했는지 확인한다.
		for( i = 0; i < 16; i++) {
			g_sphere[i].ballUpdate(timeDelta);
			for(j = 0; j < 4; j++){ g_legowall[j].hitBy(g_sphere[i]); }
		}

		// check whether any two balls hit together and update the direction of balls
		// 각각의 공에 대해, 다른 공과 서로 충돌 했는지 확인하고, 공의 방향을 갱신한다.
		for(i = 0 ;i < 16; i++){
			for(j = 0 ; j < 16; j++) {
				if(i >= j) {continue;}
				g_sphere[i].hitBy(g_sphere[j]);
			}
		}

		// 각각의 구멍에 대해, 공과 서로 충돌 했는지 확인하고, 공을 넣는다.
		for (i = 0; i < 6; i++){
			for (j = 0; j < 16; j++) {
				g_hole[i].hitBy(g_sphere[j]);
			}
		}

		// draw plane, walls, and spheres
		// 초록색 판을 그리고, 벽을 그리고, 공들을 그린다.
		g_legoPlane.draw(Device, g_mWorld);
		for (i = 0; i < 4; i++)
			g_legowall[i].draw(Device, g_mWorld);
		for (i = 0; i < 16; i++)
			g_sphere[i].draw(Device, g_mWorld);
		
		for (i = 0; i < 6; i++)
			g_hole[i].draw(Device, g_mWorld);

		g_target_blueball.draw(Device, g_mWorld);
		//g_light.draw(Device);

		// 프레임을 그린다
		g_frame.draw(Device, g_mWorld);
		
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture( 0, NULL );
	}

	if (turnManager.processTurn({ g_sphere[0], g_sphere[1], g_sphere[2], g_sphere[3] }))
	{
		MessageBox(0, "플레이어 바뀜 ", 0, 0);
	}

	return true;
}

// 마우스 또는 키보드 입력 이벤트 처리 함수
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool wire = false;
	//static bool isReset = true;
	static bool cameraTopView = true;
	static int old_x = 0;
	static int old_z = 0;
	static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;

	if (msg == WM_DESTROY){
		::PostQuitMessage(0);
	}
	else if (msg == WM_KEYDOWN){//키를 누르고 있는 동안
		if (wParam == VK_ESCAPE){// ESC 키의 경우 화면을 종료한다
			::DestroyWindow(hwnd);
		}
		else if (wParam == VK_RETURN){ //
			if (NULL != Device) {
				wire = !wire;
				Device->SetRenderState(D3DRS_FILLMODE,
					(wire ? D3DFILL_WIREFRAME : D3DFILL_SOLID));
			}
		}
		else if (wParam == VK_TAB)//카메라 시점 변경
		{
			if (cameraTopView)
			{
				D3DXVECTOR3 pos(0.0f, 5.0f, -8.0f);
				D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
				D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
				Device->SetTransform(D3DTS_VIEW, &g_mView);
			}
			else
			{
				D3DXVECTOR3 pos(0.0f, 10.0f, 0.0f);
				D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 up(0.0f, 0.0f, 2.0f);
				D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
				Device->SetTransform(D3DTS_VIEW, &g_mView);
			}

			cameraTopView = !cameraTopView;
		}
		else if (wParam == VK_LEFT){
			D3DXMATRIX mX;

			switch (move) {
			case WORLD_MOVE:
				float dx = 3 * 0.01f;
				D3DXMatrixRotationY(&mX, dx);
				g_mWorld = g_mWorld * mX;

				break;
			}
		}
		else if (wParam == VK_RIGHT){
			D3DXMATRIX mX;

			switch (move) {
			case WORLD_MOVE:
				float dx = -3 * 0.01f;
				D3DXMatrixRotationY(&mX, dx);
				g_mWorld = g_mWorld * mX;

				break;
			}
		}
		else if (wParam == VK_UP){
			D3DXMATRIX mX;

			switch (move) {
			case WORLD_MOVE:
				float dy = 3 * 0.01f;
				D3DXMatrixRotationZ(&mX, dy);
				g_mWorld = g_mWorld * mX;

				break;
			}
		}
		else if (wParam == VK_DOWN){
			D3DXMATRIX mX;

			switch (move) {
			case WORLD_MOVE:
				float dy = -3 * 0.01f;
				D3DXMatrixRotationZ(&mX, dy);
				g_mWorld = g_mWorld * mX;

				break;
			}
		}
		else if (wParam == VK_SPACE && !turnManager.isProcessing()){	// 스페이스 바의 경우 파란 공과 하얀 공의 위치를 받아서
			// 그 거리와 방향만큼 하얀 공의 속도를 조정한다.
			D3DXVECTOR3 targetpos = g_target_blueball.getCenter();
			D3DXVECTOR3	whitepos = g_sphere[0].getCenter();
			double theta = acos(sqrt(pow(targetpos.x - whitepos.x, 2)) / sqrt(pow(targetpos.x - whitepos.x, 2) +
				pow(targetpos.z - whitepos.z, 2)));		// 기본 1 사분면
			if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x >= 0) { theta = -theta; }	//4 사분면
			if (targetpos.z - whitepos.z >= 0 && targetpos.x - whitepos.x <= 0) { theta = PI - theta; } //2 사분면
			if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x <= 0){ theta = PI + theta; } // 3 사분면
			double distance = sqrt(pow(targetpos.x - whitepos.x, 2) + pow(targetpos.z - whitepos.z, 2));
			g_sphere[0].setPower(distance * cos(theta), distance * sin(theta));
			turnManager.processTriggerOn();
		}
	}else if(msg == WM_MOUSEMOVE){// 마우스가 움직일 때,
		int new_x = LOWORD(lParam);
		int new_z = HIWORD(lParam);
		double dx;
		double dz;
			
		if (LOWORD(wParam) & MK_RBUTTON) {// 마우스 오른쪽 버튼을 누를 때는, 파란 공의 위치를 옮긴다.
			dx = (old_x - new_x);// * 0.01f;
			dz = (old_z - new_z);// * 0.01f;
		
			D3DXVECTOR3 coord3d=g_target_blueball.getCenter();
			g_target_blueball.setCenter(coord3d.x+dx*(-0.007f),coord3d.y,coord3d.z+dz*0.007f );
		}
		old_x = new_x;
		old_z = new_z;
		
		if (g_target_blueball.getCenter().x > 4.56f) 
		{
			g_target_blueball.setCenter(4.56f, g_target_blueball.getCenter().y, g_target_blueball.getCenter().z);
			old_x = 4.56f;
		}
		if (g_target_blueball.getCenter().x < -4.56f)
		{
			g_target_blueball.setCenter(-4.56f, g_target_blueball.getCenter().y, g_target_blueball.getCenter().z);
			old_x = -4.56f;
		}
		if (g_target_blueball.getCenter().z > 3.06f)
		{
			g_target_blueball.setCenter(g_target_blueball.getCenter().x, g_target_blueball.getCenter().y, 3.06f);
			old_z = 3.06f;
		}
		if (g_target_blueball.getCenter().z < -3.06f)
		{
			g_target_blueball.setCenter(g_target_blueball.getCenter().x, g_target_blueball.getCenter().y, -3.06f);
			old_z = -3.06f;
		}

		move = WORLD_MOVE;
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
	
	if(!d3d::InitD3D(hinstance, Width, Height, true, D3DDEVTYPE_HAL, &Device))
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