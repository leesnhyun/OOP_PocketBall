#ifndef _TURN_MANAGER_
#define _TURN_MANAGER_
#include <initializer_list>
#include "CSphere.h"

using std::initializer_list;

class TurnManager
{
private:
	int * playerIdList;																// �Ŵ����� ������ �÷��̾���� ID
	int playerNumber;																// �Ŵ����� ������ �÷��̾� ��
	bool turnProcessSignal;															// �� ó���� �����ص� ������ �������� ����
	bool turnChangeSignal;															// ���� �ٲ�������� ���� ����
public:
	TurnManager(initializer_list<int> playerIdList);								// �ʱ� ����.
	TurnManager(const TurnManager& toCopy);											// ���� ����.
	~TurnManager();																	// ���� �Ҵ����� ���� �Ҹ���.
	TurnManager& operator = (const TurnManager& toAssign) = delete;					// �Ŵ����� ��ŭ ���� �����ڴ� ���� �����ּ���.
	void processTriggerOn();														// �� ó���� �ñ׳��� Ȱ��ȭ.
	void processTriggerOff();														// �� ó���� ���̻� ���� �ʵ��� �ñ׳��� ��Ȱ��ȭ.
	void processTurn();																// �� ó���� ����.
	bool isTurnFinished(initializer_list<CSphere> fieldBalls);						// ���� ����Ǿ������� ���� ���θ� ������.
	void finishTurn();																// ���� ����Ǿ��� �� ó���� ��.
};
#endif