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
	vector<int> playerIdList;														// 매니저가 관리할 플레이어들의 ID
	int nowTurnPlayerIndex;															// 현재 턴을 진행하는 플레이어의 위치
	void resetTurn();																// 현재 턴의 상황을 초기화시킨다.
	void finishTurn();																// 턴이 종료되었을 때 처리를 함.
public:
	TurnManager() = delete;															// 기본 생성자를 삭제.
	TurnManager(const vector<int>& playerIdList);									// 초기 생성.
	bool processTurn(const array<CSphere, 16>& fieldBalls);							// 턴 처리를 시작.
	bool isTurnFinished(const array<CSphere, 16>& fieldBalls);						// 턴이 종료되었는지에 대한 여부를 가져옴.
	void processTriggerOff();
	void processTriggerOn();
};
#endif