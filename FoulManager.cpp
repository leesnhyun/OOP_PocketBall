#include "FoulManager.h"
#include "TurnManager.h"
#include "Status.h"

extern Status status;
extern CSphere	g_sphere[16];
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
	if (firstHitBall == -1) status.setFoulStatus(true);
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
	if (status.getTurnPlayer().getBallType() != g_sphere[].getBallType()) status.setFoulStatus(true);
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