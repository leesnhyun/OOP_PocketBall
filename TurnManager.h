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
	int nowTurnPlayerIndex;															// ���� ���� �����ϴ� �÷��̾��� ��ġ
	int totalTurnCount;																// ����� ���� ��
	bool turnProcessSignal;															// �� ó���� �����ص� ������ �������� ����
	bool turnChangeSignal;															// ���� �ٲ�������� ���� ����
	void resetTurn();																// ���� ���� ��Ȳ�� �ʱ�ȭ��Ų��.
	void finishTurn();																// ���� ����Ǿ��� �� ó���� ��.
public:
	TurnManager() = delete;															// �⺻ �����ڸ� ����.
	TurnManager(const initializer_list<unsigned int>& playerIdList);				// �ʱ� ����.
	TurnManager(const TurnManager& toCopy);											// ���� ����.
	~TurnManager();																	// ���� �Ҵ����� ���� �Ҹ���.
	TurnManager& operator = (const TurnManager& toAssign) = delete;					// �Ŵ����� ��ŭ ���� �����ڴ� ���� �����ּ���.
	void processTriggerOn();														// �� ó���� �ñ׳��� Ȱ��ȭ.
	void processTriggerOff();														// �� ó���� ���̻� ���� �ʵ��� �ñ׳��� ��Ȱ��ȭ.
	unsigned int getNowTurnIndex() const;											// ���� ���� �����ϴ� �÷��̾��� ID
	unsigned int getCurrentTurnNumber() const;										// ���� �� �� ���� ����Ǿ����� ������.
	bool processTurn(const initializer_list<CSphere>& fieldBalls);					// �� ó���� ����.
	bool isTurnFinished(const initializer_list<CSphere>& fieldBalls);				// ���� ����Ǿ������� ���� ���θ� ������.
	bool isProcessing() const;														// �� ó���� �ϰ��ִ����� ���� ���θ� ������.
	bool isTurnChanged() const;														// ���� �ٲ���� ���� ���� ���θ� ������.
};
#endif