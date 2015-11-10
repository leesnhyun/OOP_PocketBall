#ifndef _PLAYER_
#define _PLAYER_

enum class BallType
{ 
	HAND, EIGHT, STRIPE, SOLID, NONE
};
// Ÿ�� ���� �Ӽ��� ����
// (NONE�� �ƹ��� ���� ������ �ʾƼ� �������� ���� ���)


class Player
{

private:
	/* Instance func or member. */
	unsigned int playerID;						// �� �÷��̾��� ID
	unsigned int numTakenBall;					// ȹ���� ���� ����

public:
	/* Instance func or member. */
	Player(unsigned int playerID);
	BallType playerBallType;					// �÷��̾ ��ǥ�� �ϴ� ���� ����
	void addTakenBall(unsigned int numBall);	// ���� ȹ���� ���� ������ ������Ŵ
	unsigned int getNumTakenBall() const;		// ȹ���� ���� ������ ��ȯ��
	unsigned int getPlayerId() const;			// �÷��̾��� ID�� ������.

};

#endif