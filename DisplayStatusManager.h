#ifndef _DISPLAY_STATUS_MANGER_
#define _DISPLAY_STATUS_MANGER_

#include <vector>
#include <string>

#include "Player.h"
#include "d3dUtility.h"
#include "d3dfont.h"

using std::vector;
using std::string;

class DisplayStatusManager {

private:

	int windowWidth;
	int windowHeight;

	string playerName[2];
	string playerBallCount[2];
	string playerBallType[2];
	
	string turnStatus;

	Player* players;
	CD3DFont* FontObjects[7];

public:
	DisplayStatusManager(const int windowWidth, const int windowHeight, Player* players);
	~DisplayStatusManager();

	bool create(const char* fontFamily, const int fontSize, IDirect3DDevice9* pDevice);
	void destory();

	bool update();

private:
	string _getBallType(const BallType b);
	string _getTurnStatus();

};

#endif