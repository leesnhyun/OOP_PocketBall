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
	// TODO : Where position..

	if (g_sphere[0]->getDisableTurn() == status.getCurrentTurnCount())
	{
		g_sphere[0]->enable();
		g_sphere[0]->setPower(0, 0);
		g_sphere[0]->setPosition(g_sphere[0]->getPreCenter_x(), 0, g_sphere[0]->getPreCenter_z());

		status.setFoulStatus(true);
	}
}

void FoulManager::isFirstHitNotMyBall()
{
	BallType nowBallType = dynamic_cast<CHandSphere&>(*g_sphere[0]).getFirstHitBallType();

	// 1. �÷��̾���� ��ǥ �� ������ ������
	// 2. ���� ���� �ּ� �ϳ��� ���� ���� ����
	// 3. ���� �÷��̾� ��ǥ ������ ���� ���� ��ġ���� ����.
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