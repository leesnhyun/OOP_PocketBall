#include "FoulManager.h"
#include "TurnManager.h"
#include "Status.h"
#include "CHandSphere.h"

extern Status status;
extern array<CSphere, 16> g_sphere;
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
	//TODO : add firsthitball
	if (dynamic_cast<CHandSphere&>(g_sphere[0]).getFirstHitBallType() == BallType::NONE) status.setFoulStatus(true);
}

void FoulManager::isHandBallInHole()
{
	if (g_sphere[0].getDisableTurn() == status.getCurrentTurnCount())
	{
		status.setFoulStatus(true);
	}
}

void FoulManager::isFirstHitNotMyBall()
{
	//TODO : add firsthitball balltype
	if (status.getTurnPlayer().getBallType() != dynamic_cast<CHandSphere&>(g_sphere[0]).getFirstHitBallType()) status.setFoulStatus(true);
}

bool FoulManager::isEightBallBadToIn()
{
	if (g_sphere[8].getDisableTurn() == status.getCurrentTurnCount() &&
		status.getTurnPlayer().getNumTakenBall() != 7)
	{
		return true;
	}
	return false;
}

bool FoulManager::isEightBallWithFoul()
{
	if (g_sphere[8].getDisableTurn() == status.getCurrentTurnCount() &&
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