#ifndef _FOULMANAGER_
#define _FOULMANAGER_

class FoulManager
{

public:
	// �Ŀ�
	bool isNoCrashHandBall();	// �ƹ� ���� ������ ���� ��,
	bool isHandBallInHall();	// �� ���� ���ۿ� �־��� ��,
	bool isFirstHitIsMyBall();// �ڽ��� ���� ���� ������ ���� ��

	//�й�
	bool isEightBallGoodToIn();	// �ڽ��� ���� �� ���� �ʰ�, 8�� �� ����
	bool isEightBallWithFoul();	// 8�� ���� �����鼭 �Ŀ�

	bool foul();// �Ŀ��̸� �������� ����

	bool lose();//�й��̸� �й��� ����
};

#endif