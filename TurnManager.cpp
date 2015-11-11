#include "TurnManager.h"
#include "Status.h"

extern Status status;

TurnManager::TurnManager(const vector<int>& playerIdList)
{
	this->playerIdList = playerIdList;
	this->nowTurnPlayerIndex = 0;
}


bool TurnManager::isTurnFinished(const array<CSphere, 16>& fieldBalls)
{
	if (status.getTurnProgressStatus())
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
	status.nextTurnCount();
	status.setTurnChangeStatus(false);
	status.setTurnPlayer(this->playerIdList.at(this->nowTurnPlayerIndex));
	this->processTriggerOff();
}

void TurnManager::finishTurn()
{
	status.nextTurnCount();
	status.setTurnChangeStatus(true);
	this->nowTurnPlayerIndex = (this->nowTurnPlayerIndex + 1) % this->playerIdList.size();
	status.setTurnPlayer(this->playerIdList.at(this->nowTurnPlayerIndex));
	this->processTriggerOff();
}

void TurnManager::processTriggerOff()
{
	status.setTurnProgressStatus(false);
}

void TurnManager::processTriggerOn()
{
	status.setTurnProgressStatus(true);
	status.setTurnChangeStatus(false);
}

bool TurnManager::processTurn(const array<CSphere, 16>& fieldBalls)
{
	if (!this->isTurnFinished(fieldBalls))
	{
		return false;
	}

	// TODO : When lose.
	if (status.getFoulStatus())
	{
		this->finishTurn();
	}
	else
	{

	}
	// TODO : Process
	// TODO : How to reset turn?
	return true;
}