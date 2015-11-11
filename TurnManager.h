#ifndef _TURN_MANAGER_
#define _TURN_MANAGER_
#include "CSphere.h"
#include <array>
#include <vector>

using std::vector;
using std::array;

class TurnManager
{
private:
	vector<int> playerIdList;														// �Ŵ����� ������ �÷��̾���� ID
	int nowTurnPlayerIndex;															// ���� ���� �����ϴ� �÷��̾��� ��ġ
	void resetTurn();																// ���� ���� ��Ȳ�� �ʱ�ȭ��Ų��.
	void finishTurn();																// ���� ����Ǿ��� �� ó���� ��.
public:
	TurnManager() = delete;															// �⺻ �����ڸ� ����.
	TurnManager(const vector<int>& playerIdList);									// �ʱ� ����.
	bool processTurn(const array<CSphere, 16>& fieldBalls);							// �� ó���� ����.
	bool isTurnFinished(const array<CSphere, 16>& fieldBalls);						// ���� ����Ǿ������� ���� ���θ� ������.
	void processTriggerOff();
	void processTriggerOn();
};
#endif