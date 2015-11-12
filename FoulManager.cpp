#include "FoulManager.h"
#include "TurnManager.h"
#include "Status.h"
#include "CHandSphere.h"
#include <cstdlib>
extern Status status;
extern array<CSphere*, 16> g_sphere;
extern Player players[2];

FoulManager::FoulManager()
{
	reset();
}

void FoulManager::reset()
{
	status.setFoulStatus(false);
}

void FoulManager::isNoHitHandBall()
{
	CHandSphere& handSphere = dynamic_cast<CHandSphere&>(*g_sphere[0]);
	if (handSphere.getFirstHitBallType() == BallType::NONE && status.getTurnProgressStatus() &&
		(abs(handSphere.getVelocity_X()) < 0.01 && abs(handSphere.getVelocity_Z()) < 0.01))
	{
		status.setFoulStatus(true);
	}
}

void FoulManager::isHandBallInHole()
{
	if (g_sphere[0]->getDisableTurn() == status.getCurrentTurnCount())
	{
		g_sphere[0]->enable();
		g_sphere[0]->setPower(0, 0);
		g_sphere[0]->setPosition(0, g_sphere[0]->getPosition().y, 0);
		int i;
		while (true) {
			for (i = 1; i < 16; i++)
			{
				if (g_sphere[0]->hasIntersected(*g_sphere[i]))
				{
					g_sphere[0]->setPosition(rand() % 5, g_sphere[0]->getPosition().y, rand() % 5);
					break;
				}
			}
			if (i == 16)
				break;
		}
		status.setFoulStatus(true);
	}
}

void FoulManager::isFirstHitNotMyBall()
{
	BallType nowBallType = dynamic_cast<CHandSphere&>(*g_sphere[0]).getFirstHitBallType();

	// 1. 플레이어들의 목표 공 종류가 결정됨
	// 2. 현재 공이 최소 하나의 공은 맞힌 상태
	// 3. 현재 플레이어 목표 종류와 맞힌 공이 일치하지 않음.
	if (status.getTurnPlayer()->getBallType() != BallType::NONE && nowBallType != BallType::NONE
		&& status.getTurnPlayer()->getBallType() != nowBallType)
	{
		status.setFoulStatus(true);
	}
}

bool FoulManager::isEightBallBadToIn()
{
	if (g_sphere[8]->getDisableTurn() == status.getCurrentTurnCount() &&
		status.getTurnPlayer()->getNumTakenBall() != 7)
	{
		return true;
	}
	return false;
}

bool FoulManager::isEightBallWithFoul()
{
	if (g_sphere[8]->getDisableTurn() == status.getCurrentTurnCount() &&
		status.getFoulStatus())
	{
		return true;
	}
	return false;
}

void FoulManager::checkFoul()
{
	isNoHitHandBall();
	isFirstHitNotMyBall();
	isHandBallInHole();
}

bool FoulManager::isLose()
{
	return (isEightBallBadToIn() || isEightBallWithFoul());
}