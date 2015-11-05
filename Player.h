#ifndef _PLAYER_
#define _PLAYER_

enum targetBall{NONE, STRIPE, PLAIN};	// Ÿ�� ���� �Ӽ��� ����
										// (NONE�� �ƹ��� ���� ������ �ʾƼ� �������� ���� ���)

class Player
{
	unsigned int numTakenBall;	// ȹ���� ���� ����

public:

	static bool isFirstPlayerTurn;	// 1�� �÷��̾��� �������� Ȯ��
	static targetBall FirstPlayerTargetBall;	// 1�� �÷��̾��� Ÿ�� ��

	Player();

	void addTakenBall(unsigned int numBall); // ���� ȹ���� ���� ������ ������Ŵ

	unsigned getNumTakenBall() const;	// ȹ���� ���� ������ ��ȯ��
};

#endif