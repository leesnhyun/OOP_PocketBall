#include "DisplayStatusManager.h"
#include "d3dfont.h"
#include "Status.h"
#include "Player.h"

extern Status status;


DisplayStatusManager::DisplayStatusManager(const int windowWidth, const int windowHeight, Player* players)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->players = players;
}

DisplayStatusManager::~DisplayStatusManager()
{
	for (unsigned int i = 0; i < 7; i++)
	{
		delete this->FontObjects[i];
	}

	delete[] this->FontObjects;
}

bool DisplayStatusManager::create(const char* fontFamily, const int fontSize, IDirect3DDevice9* pDevice)
{
	for (unsigned int i = 0; i < 7; i++)
	{
		this->FontObjects[i] = new CD3DFont(fontFamily, fontSize, 0);
		this->FontObjects[i]->InitDeviceObjects(pDevice);
		this->FontObjects[i]->RestoreDeviceObjects();
	}

	this->playerName[0] = "player " + std::to_string(this->players[0].getPlayerId());
	this->playerName[1] = "player " + std::to_string(this->players[1].getPlayerId());

	return true;
}

void DisplayStatusManager::destory()
{
	for (unsigned int i = 0; i < 2; i++)
	{
		this->FontObjects[i]->InvalidateDeviceObjects();
		this->FontObjects[i]->DeleteDeviceObjects();
		d3d::Delete<CD3DFont*>(this->FontObjects[i]);
	}
}

string DisplayStatusManager::_getBallType(const BallType b)
{
	string result;

	switch (b)
	{
	case BallType::NONE: result = "";
		break;
	case BallType::SOLID: result = "SOLID";
		break;
	case BallType::STRIPE: result = "STRIPE";
		break;
	case BallType::EIGHT: result = "BALCK";
		break;
	}

	return result;
}

string DisplayStatusManager::_getTurnStatus()
{
	string result = "";

	if (status.getTurnProgressStatus() == true)
	{
		result = "Processing...";
	}
	else if (status.getFoulStatus() == true)
	{
		result = "FREE BALL";
	}

	return result;
}

bool DisplayStatusManager::update()
{
	this->playerBallCount[0] = "Ball Count : " + std::to_string(this->players[0].getNumTakenBall());
	this->playerBallCount[1] = "Ball Count : " + std::to_string(this->players[1].getNumTakenBall());

	this->playerBallType[0] = _getBallType(this->players[0].getBallType());
	this->playerBallType[1] = _getBallType(this->players[1].getBallType());


	switch (status.getTurnPlayer()->getPlayerId())
	{
	case 1:
		this->FontObjects[0]->DrawText(60, 20, 0xffFF00000, this->playerName[0].c_str());
		this->FontObjects[1]->DrawText(windowWidth - 160, 20, 0xff000000, this->playerName[1].c_str());
		break;

	case 2:
		this->FontObjects[0]->DrawText(60, 20, 0xff000000, this->playerName[0].c_str());
		this->FontObjects[1]->DrawText(windowWidth - 160, 20, 0xffFF0000, this->playerName[1].c_str());
		break;
	}

	this->FontObjects[2]->DrawText(60, 50, 0xff0000000, this->playerBallCount[0].c_str());
	this->FontObjects[3]->DrawText(windowWidth - 160, 50, 0xff0000000, this->playerBallCount[1].c_str());

	this->FontObjects[4]->DrawText(60, 80, 0xff0000000, this->playerBallType[0].c_str());
	this->FontObjects[5]->DrawText(windowWidth - 160, 80, 0xff0000000, this->playerBallType[1].c_str());

	this->FontObjects[6]->DrawText(windowWidth / 2, 10, 0xff0000000, _getTurnStatus().c_str());

	return false;
}

