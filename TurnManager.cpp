#include "TurnManager.h"
#include <cstdlib>
#include <vector>
using std::vector;

TurnManager::TurnManager(initializer_list<int> playerIdList)
{
	vector<int> idListVector(playerIdList.begin(), playerIdList.end());
	this->playerIdList = (int *) calloc(playerIdList.size(), sizeof(int));
	this->playerNumber = playerIdList.size();

	for (int i = 0; i < this->playerNumber; i++)
	{
		(this->playerIdList)[i] = idListVector.at(i);
	}
}

TurnManager::TurnManager(const TurnManager& toCopy)
{
	this->playerIdList = (int *) calloc(toCopy.playerNumber, sizeof(int));
	for (int i = 0; i < toCopy.playerNumber; i++)
	{
		(this->playerIdList)[i] = (toCopy.playerIdList)[i];
	}
}

TurnManager::~TurnManager()
{
	delete[] (this->playerIdList);
}

bool TurnManager::isTurnFinished(initializer_list<CSphere> fieldBalls)
{
	vector<CSphere> ballVector(fieldBalls.begin(), fieldBalls.end());

	if (!this->turnProcessSignal)
	{
		return false;
	}

	for (int i = 0; i < fieldBalls.size(); i++)
	{
		CSphere targetBall = ballVector.at(i);

		if (targetBall.getVelocity_X != 0 || targetBall.getVelocity_Z != 0)
		{
			return false;
		}
	}

	return true;
}

void TurnManager::finishTurn()
{
	this->turnChangeSignal = true;
	this->processTriggerOff();
}

void TurnManager::processTriggerOn()
{
	this->turnProcessSignal = true;
}

void TurnManager::processTriggerOff()
{
	this->turnProcessSignal = false;
}

void TurnManager::processTurn()
{
	// if not finished, just end func.
	// TODO : Process

	this->finishTurn();
}