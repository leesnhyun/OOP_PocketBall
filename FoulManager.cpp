#include "FoulManager.h"
#include "TurnManager.h"

extern TurnManager turnManager;
extern CSphere	g_sphere[16];
extern Player players[2];

FoulManager::FoulManager()
{
	reset();
}

void FoulManager::reset()
{
	foul = false;
	lose = false;
}

bool FoulManager::isNoHitHandBall()
{
	return false;
}

bool FoulManager::isHandBallInHole()
{
	if (g_sphere[0].getDeadDate() == turnManager.getCurrentTurnNumber())
	{
		foul = true;
		return true;
	}
	return false;
}

bool FoulManager::isFirstHitNotMyBall()
{
	return false;
}

bool FoulManager::isEightBallBadToIn()
{
	if (g_sphere[15].getDeadDate() == turnManager.getCurrentTurnNumber()&&
		players[turnManager.getNowTurnIndex()].getNumTakenBall() != 7)
	{
		lose = true;
		return true;
	}
	return false;
}

bool FoulManager::isEightBallWithFoul()
{
	if (g_sphere[15].getDeadDate() == turnManager.getCurrentTurnNumber() &&
		foul)
	{
		lose = true;
		return true;
	}
	return false;
}

bool FoulManager::isFoul() const
{
	return this->foul;
}

bool FoulManager::isLose() const
{
	return this->lose;
}