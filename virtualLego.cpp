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

#include <ctime>
#include <array>
#include <cstdlib>
#include <cstdio>

#include "d3dUtility.h"
#include "d3dfont.h"

#include "CLight.h"
#include "CFloor.h"
#include "CWall.h"
#include "CHole.h"
#include "CBorder.h"
#include "Player.h"

#include "FoulManager.h"
#include "TurnManager.h"

#include "CSphere.h"
#include "CHandSphere.h"
#include "CStripeSphere.h"
#include "CSolidSphere.h"
#include "CEightSphere.h"
#include "CTargetSphere.h"

#include "CTopWall.h"
#include "CRightWall.h"
#include "CBottomWall.h"
#include "CLeftWall.h"
#include "Status.h"

using std::array;

IDirect3DDevice9* Device = nullptr;

// 창의 크기
const int Width  = 1024;
const int Height = 768;
const float BALL_SET_RATIO = 1.82f;

// 16개의 공의 위치를 초기화 함.
const float spherePos[16][2] = { 
	
	//white ball
	{ -2.7f, 0 }, 
	
	{ +1.5f, 0 }, 
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO)), CSphere::COMMON_RADIUS + 0.01f }, 
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO)), -(CSphere::COMMON_RADIUS + 0.01f) }, 
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 4), -(CSphere::COMMON_RADIUS * 4 + 0.04f) }, 
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 2), -(CSphere::COMMON_RADIUS * 2 + 0.02f) }, 
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 2), (CSphere::COMMON_RADIUS * 2 + 0.02f) },
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 3), -(CSphere::COMMON_RADIUS + 0.01f) }, 
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 3), (CSphere::COMMON_RADIUS + 0.01f) },
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 3), -(CSphere::COMMON_RADIUS * 3 + 0.03f) }, 
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 3), (CSphere::COMMON_RADIUS * 3 + 0.03f) },
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 4), -(CSphere::COMMON_RADIUS * 2 + 0.02f) }, 
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 4), (CSphere::COMMON_RADIUS * 2 + 0.02f) }, 
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 4), 0 }, 
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 4), 
	(CSphere::COMMON_RADIUS * 4 + 0.04f) },
	//balck ball
	{ (1.5f + (CSphere::COMMON_RADIUS * BALL_SET_RATIO) * 2), 0 } };

// 6개의 구멍의 위치를 초기화 함.
// const float holePos[6][2] = { {-4.23f,-2.73f}, {0,-2.73f}, {4.23f,-2.73f}, {-4.23f,2.73f}, {0,2.73f}, {4.23f,2.73f}};
const float holePos[6][2] = {
								{-4.45f,-2.95f}, {0.05f,-3.05f}, {4.5f,-2.95f}, 
								{-4.45f,2.95f}, {0.05f,3.05f}, {4.5f,2.95f}
							};

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
CFloor g_legoPlane(9, 0.03f, 6, d3d::TABLE_PANE);

array<CWall*, 6> g_legowall = 
{
	new CTopWall(4.0f, 0.3f, 0.15f, d3d::TABLE_WALL), 
	new CTopWall(4.0f, 0.3f, 0.15f, d3d::TABLE_WALL), 
	new CRightWall(0.15f, 0.3f, 5.40f, d3d::TABLE_WALL),
	new CBottomWall(4.0f, 0.3f, 0.15f, d3d::TABLE_WALL),
	new CBottomWall(4.0f, 0.3f, 0.15f, d3d::TABLE_WALL),
	new CLeftWall(0.15f, 0.3f, 5.40f, d3d::TABLE_WALL)
};

array<CSphere, 16> g_sphere = 
{ 
	CHandSphere("0"), CSolidSphere("1"), CSolidSphere("2"), CSolidSphere("3"), 
	CSolidSphere("4"), CSolidSphere("5"), CSolidSphere("6"), CSolidSphere("7"), 
	CEightSphere("8"), CStripeSphere("s9"), CStripeSphere("s10"), CStripeSphere("s11"),
	CStripeSphere("s12"), CStripeSphere("S13"), CStripeSphere("s14"), CStripeSphere("s15") 
};

CTargetSphere g_target_blueball("guide");

CLight	g_light;
CHole	g_hole[6];

CBorder g_border(d3d::TABLE_BORDER);

double g_camera_pos[3] = {0.0, 5.0, -8.0};

Player players[2] = { Player(1), Player(2) };
vector<Player> playerVec = {players[0], players[1]};
Status status(playerVec);
TurnManager turnManager(status.getPlayerIdList());
FoulManager foulManager;

CD3DFont* Font = 0;
DWORD FrameCnt = 0;
float TimeElapsed = 0;
float FPS = 0;
char FPSString[9];

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

	// 글자생성
	Font = new CD3DFont("Tahoma", 16, 0);
	Font->InitDeviceObjects(Device);
	Font->RestoreDeviceObjects();

	// 프레임생성
	if (false == g_border.create(Device)) return false;
	g_border.setPosition(0.115f, -0.44f, 0.00f);

	// 6개의 구멍을 생성함
	for (i = 0; i<6; i++) {
		if (false == g_hole[i].create(Device)) return false;
		g_hole[i].setPosition(holePos[i][0], -0.23f, holePos[i][1]);
	}
	// 초록색 바닥을 생성
	if (false == g_legoPlane.create(Device)) return false;
	g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);
	
	//// 벽을 생성 ////
	//     [0]     [1]
	//  [5]            [2]
	//     [4]     [3]

	// 가로벽 (9*0.3f*0.15) , (0, 0.12, 3.06)
	if (false == g_legowall[0]->create(Device)) return false;
	g_legowall[0]->setPosition(-2.2f, 0.12f, 3.06f);
	if (false == g_legowall[1]->create(Device)) return false;
	g_legowall[1]->setPosition(2.3f, 0.12f, 3.06f);

	if (false == g_legowall[3]->create(Device)) return false;
	g_legowall[3]->setPosition(-2.2f, 0.12f, -3.06f);
	if (false == g_legowall[4]->create(Device)) return false;
	g_legowall[4]->setPosition(2.3f, 0.12f, -3.06f);

	// 세로벽 (0.15f*0.3f*6.24f) , (4.56, 0.12, 0)
	if (false == g_legowall[2]->create(Device)) return false;
	g_legowall[2]->setPosition(4.56f, 0.12f, 0.0f);
	if (false == g_legowall[5]->create(Device)) return false;
	g_legowall[5]->setPosition(-4.56f, 0.12f, 0.0f);

	// 16개의 공을 생성함
	for (i=0;i<16;i++) {
		if (false == g_sphere[i].create(Device)) return false;
		g_sphere[i].setPosition(spherePos[i][0], static_cast<float>(CSphere::COMMON_RADIUS) , spherePos[i][1]);
		g_sphere[i].setPower(0,0);
	}

	// 파란색 공을 생성함
	if (false == g_target_blueball.create(Device)) return false;
	g_target_blueball.setPosition(.0f, static_cast<float>(CSphere::COMMON_RADIUS) , .0f);

	if (false == g_light.create(Device))
		return false;
	
	// 카메라의 위치와 시야를 설정함
	D3DXVECTOR3 pos(0.0f, 10.0f, 0.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 0.0f, 2.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);
	
	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
		static_cast<float>(Width) / static_cast<float>(Height), 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
	
	// Set render states.
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	
	g_light.draw(Device, g_mWorld);
	return true;
}

// 화면에서 전부 소멸시킴
void Cleanup(void)
{
	g_legoPlane.destroy();
	for(int i = 0 ; i < 4; i++) {
		g_legowall[i]->destroy();
	}
	destroyAllLegoBlock();
	g_light.destroy();

	if (Font) {
		Font->InvalidateDeviceObjects();
		Font->DeleteDeviceObjects();
		d3d::Delete<CD3DFont*>(Font);
	}
}


// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"

// timeDelta는 이전 이미지 프레임과 현재 이미지 프레임의 사이의 시간값을 나타냅니다.
// 공이 움직이는 거리는 무조건 "속도 * timeDelta"여야 합니다.

bool Display(float timeDelta)// 한 프레임에 해당되는 화면을 보여줌
{
	int i = 0;
	int j = 0;

	if( Device )
	{
		
		// Status Message
		FrameCnt++;
		TimeElapsed += timeDelta;

		if (TimeElapsed >= 1.0f)
		{
			FPS = (float)FrameCnt / TimeElapsed;


			sprintf(FPSString, "%f", FPS);
			FPSString[8] = '\0'; // mark end of string

			TimeElapsed = 0.0f;
			FrameCnt = 0;
		}


		Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();

		if (Font) Font->DrawText(200, 20, 0xff000000, FPSString);
		

		// update the position of each ball. during update, check whether each ball hit by walls.
		// 공의 위치를 갱신한다. 갱신하는 중에는 각각의 공이 벽과 충돌 했는지 확인한다.
		for( i = 0; i < 16; i++) {
			g_sphere[i].ballUpdate(timeDelta);
			for (j = 0; j < 6; j++){ g_legowall[j]->hitBy(g_sphere[i]); }
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
				if (!status.getFoulStatus()) {
					if (g_hole[i].hasIntersected(g_sphere[j]) && status.getTurnPlayer().getBallType() == BallType::NONE) {
						// TODO : Check
						BallType nowBallType = g_sphere[j].getBallType();
						status.getTurnPlayer().setBallType(nowBallType);
						status.getNoTurnPlayer().setBallType((nowBallType == BallType::STRIPE) ? BallType::SOLID : BallType::STRIPE);
					}
					g_hole[i].hitBy(g_sphere[j]);
				}
			}
		}

		// draw plane, walls, and spheres
		// 초록색 판을 그리고, 벽을 그리고, 공들을 그린다.
		g_legoPlane.draw(Device, g_mWorld);
		
		for (i = 0; i < 6; i++)
			g_legowall[i]->draw(Device, g_mWorld);

		for (i = 0; i < 16; i++)
			g_sphere[i].draw(Device, g_mWorld);
		
		for (i = 0; i < 6; i++)
			g_hole[i].draw(Device, g_mWorld);

		g_target_blueball.draw(Device, g_mWorld);
		//g_light.draw(Device);

		// 프레임을 그린다
		g_border.draw(Device, g_mWorld);
		
		Device->EndScene();
		Device->Present(nullptr, nullptr, nullptr, nullptr);
		//Device->SetTexture( 0, NULL );
	}

	if (turnManager.processTurn(g_sphere))
	{
		MessageBox(nullptr, "플레이어 바뀜 ", nullptr, 0);
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
				D3DXMatrixRotationX(&mX, dy);
				g_mWorld = g_mWorld * mX;

				break;
			}
		}
		else if (wParam == VK_DOWN){
			D3DXMATRIX mX;

			switch (move) {
			case WORLD_MOVE:
				float dy = -3 * 0.01f;
				D3DXMatrixRotationX(&mX, dy);
				g_mWorld = g_mWorld * mX;

				break;
			}
		}
		else if (wParam == VK_SPACE && !status.getTurnProgressStatus()){	// 스페이스 바의 경우 파란 공과 하얀 공의 위치를 받아서
			// 그 거리와 방향만큼 하얀 공의 속도를 조정한다.
			D3DXVECTOR3 targetpos = g_target_blueball.getPosition();
			D3DXVECTOR3	whitepos = g_sphere[0].getPosition();
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
		
		if ((LOWORD(wParam) & MK_LBUTTON) && !status.getTurnProgressStatus() && status.getFoulStatus()) {
			// 마우스 왼쪽 버튼을 누를 때는, 흰 공의 위치를 옮긴다.
			dx = (old_x - new_x);// * 0.01f;
			dz = (old_z - new_z);// * 0.01f;

			CHandSphere preMovedWhiteBall("0");
			bool canMove = true;
			D3DXVECTOR3 coord3d = g_sphere[0].getPosition();
			preMovedWhiteBall.setPosition(coord3d.x + dx*(-0.007f), coord3d.y, coord3d.z + dz*0.007f);

			for (int i = 0; i < 6; i++)
			{
				if (g_legowall[i]->hasIntersected(preMovedWhiteBall))
				{
					canMove = false;
				}
			}

			for (int i = 0; i < 6; i++)
			{
				if (g_hole[i].hasIntersected(preMovedWhiteBall))
				{
					canMove = false;
				}
			}

			for (int i = 1; i < 16; i++)
			{
				if (g_sphere[i].hasIntersected(preMovedWhiteBall))
				{
					canMove = false;
				}
			}

			if (canMove)
			{
				g_sphere[0].setPosition(coord3d.x + dx*(-0.007f), coord3d.y, coord3d.z + dz*0.007f);
				old_x = new_x;
				old_z = new_z;
			}
		}

		if (LOWORD(wParam) & MK_RBUTTON) {// 마우스 오른쪽 버튼을 누를 때는, 파란 공의 위치를 옮긴다.
			dx = (old_x - new_x);// * 0.01f;
			dz = (old_z - new_z);// * 0.01f;

			CHandSphere preMovedWhiteBall("0");
			bool canMove = true;
			D3DXVECTOR3 coord3d = g_target_blueball.getPosition();
			preMovedWhiteBall.setPosition(coord3d.x + dx*(-0.007f), coord3d.y, coord3d.z + dz*0.007f);

			for (int i = 0; i < 6; i++)
			{
				if (g_legowall[i]->hasIntersected(preMovedWhiteBall))
				{
					canMove = false;
				}
			}

			for (int i = 0; i < 6; i++)
			{
				if (g_hole[i].hasIntersected(preMovedWhiteBall))
				{
					canMove = false;
				}
			}

			if (canMove)
			{
				g_target_blueball.setPosition(coord3d.x + dx*(-0.007f), coord3d.y, coord3d.z + dz*0.007f);
				old_x = new_x;
				old_z = new_z;
			}
		}
		old_x = new_x;
		old_z = new_z;

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
	srand(static_cast<unsigned int>(time(nullptr)));
	
	if(!d3d::InitD3D(hinstance, Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(nullptr, "InitD3D() - FAILED", nullptr, 0);
		return 0;
	}
	
	if(!Setup())
	{
		::MessageBox(nullptr, "Setup() - FAILED", nullptr, 0);
		return 0;
	}
	
	d3d::EnterMsgLoop( Display );
	
	Cleanup();
	
	Device->Release();
	
	return 0;
}