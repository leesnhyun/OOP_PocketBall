#include "DisplayStatusManager.h"
#include "d3dfont.h"
#include "Status.h"

DisplayStatusManager::DisplayStatusManager(const int windowWidth, const int windowHeight, Player* players){

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->players = players;

}

DisplayStatusManager::~DisplayStatusManager(){
	
	for (unsigned int i = 0; i < 2; i++){
		delete this->FontObjects[i];
	}

	delete[] this->FontObjects;

}

bool DisplayStatusManager::create(const char* fontFamily, const int fontSize, IDirect3DDevice9* pDevice){

	for (unsigned int i = 0; i < 2; i++){
		this->FontObjects[i] = new CD3DFont(fontFamily, fontSize, 0);
		this->FontObjects[i]->InitDeviceObjects(pDevice);
		this->FontObjects[i]->RestoreDeviceObjects();
	}

	this->playerName[0] = "player " + std::to_string(this->players[0].getPlayerId());
	this->playerName[1] = "player " + std::to_string(this->players[1].getPlayerId());

	return true;
}

void DisplayStatusManager::destory(){

	for (unsigned int i = 0; i < 2; i++){
		this->FontObjects[i]->InvalidateDeviceObjects();
		this->FontObjects[i]->DeleteDeviceObjects();
		d3d::Delete<CD3DFont*>(this->FontObjects[i]);
	}
	
}

void DisplayStatusManager::display(){

	this->FontObjects[0]->DrawText(200, 20, 0xff000000, this->playerName[0].c_str());
	this->FontObjects[1]->DrawText(windowWidth-200, 20, 0xff000000, this->playerName[1].c_str());

}

bool DisplayStatusManager::update(){
	
	return false;
}
