#ifndef _FOULMANAGER_
#define _FOULMANAGER_

class FoulManager
{

public:
	// ��Ģ�� ������ ��� true ��ȯ
	// �Ŀ�
	bool isNoCrashHandBall();	// �ƹ� ���� ������ ���� ��,
	bool isHandBallInHole();	// �� ���� ���ۿ� �־��� ��,
	bool isFirstHitNotMyBall();// �ڽ��� ���� ���� ������ ���� ��

	//�й�
	bool isEightBallBadToIn();	// �ڽ��� ���� �� ���� �ʰ�, 8�� �� ����
	bool isEightBallWithFoul();	// 8�� ���� �����鼭 �Ŀ�

	bool foul();// �Ŀ��̸� �������� ����

	bool lose();//�й��̸� �й��� ����
};

#endif