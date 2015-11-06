#ifndef _PLAYER_
#define _PLAYER_

enum class TargetBall 
{ 
	NONE, STRIPE, PLAIN
};
// Ÿ�� ���� �Ӽ��� ����
// (NONE�� �ƹ��� ���� ������ �ʾƼ� �������� ���� ���)


class Player
{
private:
	/* Static func or member. */
	static TargetBall FirstPlayerTargetBall;	// 1�� �÷��̾��� Ÿ�� ��

	/* Instance func or member. */
	unsigned int playerID;						// �� �÷��̾��� ID
	unsigned int numTakenBall;					// ȹ���� ���� ����
	TargetBall playerTargetBallType;			//
	bool isAppropriatePlayer();

public:
	/* Static func or member. */

	/* Instance func or member. */
	Player(unsigned int playerID);
	void addTakenBall(unsigned int numBall);	// ���� ȹ���� ���� ������ ������Ŵ
	unsigned int getNumTakenBall() const;		// ȹ���� ���� ������ ��ȯ��
};

#endif