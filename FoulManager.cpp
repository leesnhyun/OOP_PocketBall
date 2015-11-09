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

void FoulManager::isNoHitHandBall()
{

}

void FoulManager::isHandBallInHole()
{
	if (g_sphere[0].getDeadDate() == turnManager.getCurrentTurnNumber())
	{
		foul = true;
	}
}

void FoulManager::isFirstHitNotMyBall()
{
}

void FoulManager::isEightBallBadToIn()
{
	if (g_sphere[15].getDeadDate() == turnManager.getCurrentTurnNumber()&&
		players[turnManager.getNowTurnIndex()].getNumTakenBall() != 7)
	{
		lose = true;
	}
}

void FoulManager::isEightBallWithFoul()
{
	if (g_sphere[15].getDeadDate() == turnManager.getCurrentTurnNumber() &&
		foul)
	{
		lose = true;
	}
}

bool FoulManager::isFoul()
{
	void isNoHitHandBall();
	void isHandBallInHole();
	void isFirstHitNotMyBall();

	return this->foul;
}

bool FoulManager::isLose()
{
	void isEightBallBadToIn();
	void isEightBallWithFoul();
	return this->lose;
}