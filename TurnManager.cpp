#include "TurnManager.h"
#include "Status.h"

extern Status gameStatus;

TurnManager::TurnManager(const vector<int>& playerIdList)
{
	this->playerIdList = playerIdList;
	this->nowTurnPlayerIndex = 0;
}


bool TurnManager::isTurnFinished(const array<CSphere, 16>& fieldBalls)
{
	if (!gameStatus.getTurnProgressStatus())
	{
		return false;
	}

	for (unsigned int i = 0; i < fieldBalls.size(); i++)
	{
		CSphere ball = fieldBalls.at(i);

		if (abs(ball.getVelocity_X()) > 0.01 || abs(ball.getVelocity_Z()) > 0.01)
		{
			return false;
		}
	}

	return true;
}

void TurnManager::resetTurn()
{
	gameStatus.nextTurnCount();
	gameStatus.setTurnChangeStatus(false);
	gameStatus.setTurnPlayer(this->playerIdList.at(this->nowTurnPlayerIndex));
	this->processTriggerOff();
}

void TurnManager::finishTurn()
{
	gameStatus.nextTurnCount();
	gameStatus.setTurnChangeStatus(true);
	this->nowTurnPlayerIndex = (this->nowTurnPlayerIndex + 1) % this->playerIdList.size();
	gameStatus.setTurnPlayer(this->playerIdList.at(this->nowTurnPlayerIndex));
	this->processTriggerOff();
}

bool TurnManager::processTurn(const array<CSphere, 16>& fieldBalls)
{
	if (!this->isTurnFinished(fieldBalls))
	{
		return false;
	}

	// TODO : When lose.
	if (gameStatus.getFoulStatus())
	{
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