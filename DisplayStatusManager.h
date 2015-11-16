#ifndef _DISPLAY_STATUS_MANGER_
#define _DISPLAY_STATUS_MANGER_

#include <vector>
#include <string>

#include "Player.h"
#include "d3dUtility.h"
#include "d3dfont.h"

using std::vector;
using std::string;

class DisplayStatusManager // ���� ���� ��Ȳ�� ȭ�鿡 ������ִ� ��ü
{
private:

	int windowWidth; // ȭ���� �ʺ�
	int windowHeight; // ȭ���� ����

	string playerName[2]; // �÷��̾��� �̸�
	string playerBallCount[2]; // �÷��̾ ���� ���� ����
	string playerBallType[2]; // �÷��̾��� ��ǥ �� ����

	string turnStatus; // ������ ����

	Player* players; // �÷��̾�
	CD3DFont* FontObjects[7]; // ��Ʈ ��ü

public:
	DisplayStatusManager(const int windowWidth, const int windowHeight, Player* players);
	~DisplayStatusManager();

	bool create(const char* fontFamily, const int fontSize, IDirect3DDevice9* pDevice);
	void destory();

	bool update(); // ȭ�鿡 ������

private:
	string _getBallType(const BallType b);
	string _getTurnStatus();
};

#endif

