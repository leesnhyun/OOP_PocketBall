#include "TurnManager.h"
#include "FoulManager.h"
#include <cstdlib>
#include <vector>
using std::vector;

TurnManager::TurnManager(const initializer_list<unsigned int>& playerIdList)
{
	vector<int> idListVector(playerIdList.begin(), playerIdList.end());
	this->playerIdList = static_cast<unsigned int *> (calloc(playerIdList.size(), sizeof(unsigned int)));
	this->playerNumber = playerIdList.size();
	this->nowTurnPlayerIndex = 0;
	this->totalTurnCount = 0;
	this->freeballSignal = false;

	for (unsigned int i = 0; i < this->playerNumber; i++)
	{
		(this->playerIdList)[i] = idListVector.at(i);
	}
}

TurnManager::TurnManager(const TurnManager& toCopy)
{
	this->playerIdList = static_cast<unsigned int *>(calloc(toCopy.playerNumber, sizeof(unsigned int)));
	this->nowTurnPlayerIndex = toCopy.nowTurnPlayerIndex;
	this->playerNumber = toCopy.playerNumber;
	this->totalTurnCount = toCopy.totalTurnCount;
	this->freeballSignal = toCopy.freeballSignal;

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

		if (abs(targetBall.getVelocity_X()) > CSphere::STOP_SPEED || abs(targetBall.getVelocity_Z()) > CSphere::STOP_SPEED)
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
	this->freeballSignal = false;
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

bool TurnManager::isFreeBall() const
{
	return this->freeballSignal;
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
		this->freeballSignal = true;
		this->finishTurn();
	}
	else
	{
		if (!foulManager.isLose())
		{
			this->finishTurn();
		}
	}
	// TODO : Process
	// TODO : How to reset turn?
	return true;
}