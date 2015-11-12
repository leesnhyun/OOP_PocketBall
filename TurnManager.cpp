#include "TurnManager.h"
#include "Status.h"
#include "CHandSphere.h"
#include <array>
using std::array;

extern Status status;
extern array<CSphere*, 16> g_sphere;
TurnManager::TurnManager(const vector<int>& playerIdList)
{
	this->playerIdList = playerIdList;
	this->nowTurnPlayerIndex = 0;
}


bool TurnManager::isTurnFinished(const array<CSphere*, 16>& fieldBalls)
{
	if (!status.getTurnProgressStatus())
	{
		return false;
	}

	for (unsigned int i = 0; i < fieldBalls.size(); i++)
	{
		CSphere ball = *(fieldBalls.at(i));

		if (abs(ball.getVelocity_X()) > 0.01 || abs(ball.getVelocity_Z()) > 0.01)
		{
			return false;
		}
	}

	return true;
}

void TurnManager::resetTurn()
{
	status.setTurnChangeStatus(false);
	status.setTurnPlayer(this->playerIdList.at(this->nowTurnPlayerIndex));
	this->processTriggerOff();
}

void TurnManager::finishTurn()
{
	status.setTurnChangeStatus(true);
	this->nowTurnPlayerIndex = (this->nowTurnPlayerIndex + 1) % this->playerIdList.size();
	status.setTurnPlayer(this->playerIdList.at(this->nowTurnPlayerIndex));
	this->processTriggerOff();
}

void TurnManager::processTriggerOff()
{
	status.nextTurnCount();
	status.setTurnProgressStatus(false);
	dynamic_cast<CHandSphere&>(*g_sphere[0]).setFirstHitBallType(BallType::NONE);
}

void TurnManager::processTriggerOn()
{
	dynamic_cast<CHandSphere&>(*g_sphere[0]).setFirstHitBallType(BallType::NONE);
	status.setFoulStatus(false);
	status.setTurnProgressStatus(true);
	status.setTurnChangeStatus(false);
}

bool TurnManager::processTurn(const array<CSphere*, 16>& fieldBalls)
{
	if (!this->isTurnFinished(fieldBalls) || status.getGameEndStatus())
	{
		return false;
	}

	// TODO : When lose.
	if (status.getFoulStatus() || status.getTurnPlayer()->getBallType() == BallType::NONE)
	{
		this->finishTurn();
	}
	else
	{
		bool isPutMyBall = false;
		int myBallCount = 0;

		for (int i = 0; i < 16; i++)
		{
			if (g_sphere[i]->getBallType() == status.getTurnPlayer()->getBallType() && g_sphere[i]->isDisabled())
			{
				myBallCount++;
			}

			if (g_sphere[i]->getDisableTurn() == status.getCurrentTurnCount()
				&& g_sphere[i]->getBallType() == status.getTurnPlayer()->getBallType())
			{
				isPutMyBall = true;
			}
		}

		if (myBallCount == 7 && g_sphere[8]->isDisabled())
		{
			status.setWinnerPlayer(this->playerIdList[this->nowTurnPlayerIndex]);
		}

		if (isPutMyBall)
		{
			this->resetTurn();
		}
		else
		{
			this->finishTurn();
		}
	}

	return true;
}