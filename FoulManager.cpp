//#include "FoulManager.h"
//#include "TurnManager.h"
//
//extern TurnManager turnManager;
//extern CSphere	g_sphere[16];
//extern Player players[2];
//
//FoulManager::FoulManager()
//{
//	reset();
//}
//
//void FoulManager::reset()
//{
//	foul = false;
//	lose = false;
//	firstHitBall = -1;
//}
//
//void FoulManager::setFirstHitBall(int ball_idx)
//{
//	if (firstHitBall == -1) firstHitBall = ball_idx;
//}
//
//void FoulManager::isNoHitHandBall()
//{
//	if (firstHitBall == -1) foul = true;
//}
//
//void FoulManager::isHandBallInHole()
//{
//	if (g_sphere[0].getDeadDate() == turnManager.getCurrentTurnNumber())
//	{
//		foul = true;
//	}
//}
//
//void FoulManager::isFirstHitNotMyBall()
//{
//	//TODO : add balltype
//	if (players[turnManager.getNowTurnIndex()].playerBallType != g_sphere[firstHitBall].getBallType()) foul = true;
//}
//
//void FoulManager::isEightBallBadToIn()
//{
//	if (g_sphere[15].getDeadDate() == turnManager.getCurrentTurnNumber()&&
//		players[turnManager.getNowTurnIndex()].getNumTakenBall() != 7)
//	{
//		lose = true;
//	}
//}
//
//void FoulManager::isEightBallWithFoul()
//{
//	if (g_sphere[15].getDeadDate() == turnManager.getCurrentTurnNumber() &&
//		foul)
//	{
//		lose = true;
//	}
//}
//
//bool FoulManager::isFoul()
//{
//	void isNoHitHandBall();
//	void isFirstHitNotMyBall();
//	void isHandBallInHole();
//
//	return this->foul;
//}
//
//bool FoulManager::isLose()
//{
//	void isEightBallBadToIn();
//	void isEightBallWithFoul();
//	return this->lose;
//}