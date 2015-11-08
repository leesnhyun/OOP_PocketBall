#ifndef _TURN_MANAGER_
#define _TURN_MANAGER_
#include <initializer_list>
#include "CSphere.h"

using std::initializer_list;

class TurnManager
{
private:
	unsigned int * playerIdList;													// 매니저가 관리할 플레이어들의 ID
	unsigned int playerNumber;														// 매니저가 관리할 플레이어 수
	int nowTurnPlayerIndex;															// 현재 턴을 진행하는 플레이어의 위치
	int totalTurnCount;																// 진행된 턴의 수
	bool turnProcessSignal;															// 턴 처리를 시작해도 괜찮은 것인지의 여부
	bool turnChangeSignal;															// 턴이 바뀌었는지에 대한 여부
	void resetTurn();																// 현재 턴의 상황을 초기화시킨다.
	void finishTurn();																// 턴이 종료되었을 때 처리를 함.
public:
	TurnManager() = delete;															// 기본 생성자를 삭제.
	TurnManager(const initializer_list<unsigned int>& playerIdList);				// 초기 생성.
	TurnManager(const TurnManager& toCopy);											// 복사 생성.
	~TurnManager();																	// 동적 할당으로 인한 소멸자.
	TurnManager& operator = (const TurnManager& toAssign) = delete;					// 매니저인 만큼 대입 연산자는 쓰지 말아주세요.
	void processTriggerOn();														// 턴 처리의 시그널을 활성화.
	void processTriggerOff();														// 턴 처리를 더이상 하지 않도록 시그널을 비활성화.
	unsigned int getNowTurnIndex() const;											// 현재 턴을 진행하는 플레이어의 ID
	unsigned int getCurrentTurnNumber() const;										// 현재 총 몇 턴이 진행되었는지 가져옴.
	bool processTurn(const initializer_list<CSphere>& fieldBalls);					// 턴 처리를 시작.
	bool isTurnFinished(const initializer_list<CSphere>& fieldBalls);				// 턴이 종료되었는지에 대한 여부를 가져옴.
	bool isProcessing() const;														// 턴 처리를 하고있는지에 대한 여부를 가져옴.
	bool isTurnChanged() const;														// 턴이 바뀌었는 지에 대한 여부를 가져옴.
};
#endif