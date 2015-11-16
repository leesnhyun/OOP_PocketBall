#ifndef _DISPLAY_STATUS_MANGER_
#define _DISPLAY_STATUS_MANGER_

#include <vector>
#include <string>

#include "Player.h"
#include "d3dUtility.h"
#include "d3dfont.h"

using std::vector;
using std::string;

class DisplayStatusManager // 현재 게임 상황을 화면에 출력해주는 객체
{
private:

	int windowWidth; // 화면의 너비
	int windowHeight; // 화면의 높이

	string playerName[2]; // 플레이어의 이름
	string playerBallCount[2]; // 플레이어가 얻은 공의 개수
	string playerBallType[2]; // 플레이어의 목표 공 종류

	string turnStatus; // 차례의 상태

	Player* players; // 플레이어
	CD3DFont* FontObjects[7]; // 폰트 객체

public:
	DisplayStatusManager(const int windowWidth, const int windowHeight, Player* players);
	~DisplayStatusManager();

	bool create(const char* fontFamily, const int fontSize, IDirect3DDevice9* pDevice);
	void destory();

	bool update(); // 화면에 갱신함

private:
	string _getBallType(const BallType b);
	string _getTurnStatus();
};

#endif

