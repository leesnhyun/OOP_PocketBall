#ifndef _TURN_MANAGER_
#define _TURN_MANAGER_
#include <initializer_list>
#include "CSphere.h"

using std::initializer_list;

class TurnManager
{
private:
	unsigned int * playerIdList;													// �Ŵ����� ������ �÷��̾���� ID
	unsigned int playerNumber;														// �Ŵ����� ������ �÷��̾� ��
	unsigned int nowTurnPlayerIndex;												// ���� ���� �����ϴ� �÷��̾��� ��ġ
	bool turnProcessSignal;															// �� ó���� �����ص� ������ �������� ����
	bool turnChangeSignal;															// ���� �ٲ�������� ���� ����
public:
	TurnManager(const initializer_list<unsigned int>& playerIdList);				// �ʱ� ����.
	TurnManager(const TurnManager& toCopy);											// ���� ����.
	~TurnManager();																	// ���� �Ҵ����� ���� �Ҹ���.
	TurnManager& operator = (const TurnManager& toAssign) = delete;					// �Ŵ����� ��ŭ ���� �����ڴ� ���� �����ּ���.
	void processTriggerOn();														// �� ó���� �ñ׳��� Ȱ��ȭ.
	void processTriggerOff();														// �� ó���� ���̻� ���� �ʵ��� �ñ׳��� ��Ȱ��ȭ.
	unsigned int getNowTurnID() const;												// ���� ���� �����ϴ� �÷��̾��� ID
	void resetTurn();																// ���� ���� ��Ȳ�� �ʱ�ȭ��Ų��.
	bool processTurn(const initializer_list<CSphere>& fieldBalls);					// �� ó���� ����.
	bool isTurnFinished(const initializer_list<CSphere>& fieldBalls);				// ���� ����Ǿ������� ���� ���θ� ������.
	void finishTurn();																// ���� ����Ǿ��� �� ó���� ��.
};
#endif