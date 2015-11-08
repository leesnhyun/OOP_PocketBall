#include "TurnManager.h"
#include "FoulManager.h"
#include <cstdlib>
#include <vector>
using std::vector;

TurnManager::TurnManager(const initializer_list<unsigned int>& playerIdList)
{
	vector<int> idListVector(playerIdList.begin(), playerIdList.end());
	this->playerIdList = (unsigned int *) calloc(playerIdList.size(), sizeof(unsigned int));
	this->playerNumber = playerIdList.size();
	this->nowTurnPlayerIndex = 0;
	this->totalTurnCount = 0;

	for (unsigned int i = 0; i < this->playerNumber; i++)
	{
		(this->playerIdList)[i] = idListVector.at(i);
	}
}

TurnManager::TurnManager(const TurnManager& toCopy)
{
	this->playerIdList = (unsigned int *) calloc(toCopy.playerNumber, sizeof(unsigned int));
	this->nowTurnPlayerIndex = toCopy.nowTurnPlayerIndex;
	this->playerNumber = toCopy.playerNumber;
	this->totalTurnCount = toCopy.totalTurnCount;

	for (unsigned int i = 0; i < toCopy.playerNumber; i++)
	{
		(this->playerIdList)[i] = (toCopy.playerIdList)[i];
	}
}

TurnManager::~TurnManager()
{
	delete[] (this->playerIdList);
}

bool TurnManager::isTurnFinished(const initializer_list<CSphere>& fieldBalls)
{
	vector<CSphere> ballVector(fieldBalls.begin(), fieldBalls.end());

	if (!this->turnProcessSignal)
	{
		return false;
	}

	for (unsigned int i = 0; i < fieldBalls.size(); i++)
	{
		CSphere targetBall = ballVector.at(i);

		if (targetBall.getVelocity_X() != 0 || targetBall.getVelocity_Z() != 0)
		{
			return false;
		}
	}

	return true;
}

void TurnManager::finishTurn()
{
	this->totalTurnCount++;
	this->turnChangeSignal = true;
	this->nowTurnPlayerIndex = (this->nowTurnPlayerIndex + 1) % this->playerNumber;
	this->processTriggerOff();
}

void TurnManager::processTriggerOn()
{
	this->turnChangeSignal = false;
	this->turnProcessSignal = true;
}

void TurnManager::processTriggerOff()
{
	this->turnProcessSignal = false;
}

unsigned int TurnManager::getNowTurnIndex() const
{
	return this->nowTurnPlayerIndex;
}

unsigned int TurnManager::getCurrentTurnNumber() const
{
	return this->totalTurnCount;
}

void TurnManager::resetTurn()
{
	this->totalTurnCount++;
	this->turnChangeSignal = false;
	this->processTriggerOff();
}

bool TurnManager::isProcessing() const
{
	return (this->turnProcessSignal);
}

bool TurnManager::isTurnChanged() const
{
	return (this->turnChangeSignal);
}

bool TurnManager::processTurn(const initializer_list<CSphere>& fieldBalls)
{
	FoulManager foulManager;
	if (!this->isTurnFinished(fieldBalls))
	{
		return false;
	}

	// TODO : When lose.
	if (foulManager.isFoul())
	{
		this->finishTurn();
	}
	else
	{
		this->resetTurn();
	}
	// TODO : Process
	
	return true;
}