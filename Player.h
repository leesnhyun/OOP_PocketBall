#ifndef _PLAYER_
#define _PLAYER_

enum class BallType
{
	HAND,
	EIGHT,
	STRIPE,
	SOLID,
	NONE
};

// Ÿ�� ���� �Ӽ��� ����
// (NONE�� �ƹ��� ���� ������ �ʾƼ� �������� ���� ���)


class Player
{
private:
	unsigned int playerID; // �� �÷��̾��� ID
	unsigned int numTakenBall; // ȹ���� ���� ����

	BallType playerBallType; // �÷��̾ ��ǥ�� �ϴ� ���� ����

public:
	Player(unsigned int playerID);
	void setTakenBall(unsigned int numBall); // ���� ȹ���� ���� ������ ������Ŵ
	unsigned int getNumTakenBall() const; // ȹ���� ���� ������ ��ȯ��
	unsigned int getPlayerId() const; // �÷��̾��� ID�� ������.

	BallType getBallType() const;
	void setBallType(BallType ballType);
};

#endif

