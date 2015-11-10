#ifndef _FOULMANAGER_
#define _FOULMANAGER_

class FoulManager
{
	// ��Ģ�� ������ ��� true ��ȯ
	// �Ŀ�

	bool foul;
	bool lose;
	int firstHitBall;

	void isNoHitHandBall();	// �ƹ� ���� ������ ���� ��,
	void isHandBallInHole();	// �� ���� ���ۿ� �־��� ��,
	void isFirstHitNotMyBall();// �ڽ��� ���� ���� ������ ���� ��

	//�й�
	void isEightBallBadToIn();	// �ڽ��� ���� �� ���� �ʰ�, 8�� �� ����
	void isEightBallWithFoul();	// 8�� ���� �����鼭 �Ŀ�

public:
	FoulManager();

	void reset();

	void setFirstHitBall(int ball_idx);

	bool isFoul();// �Ŀ��̸� �������� ����
	bool isLose();//�й��̸� �й��� ����
};

#endif