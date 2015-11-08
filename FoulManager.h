#ifndef _FOULMANAGER_
#define _FOULMANAGER_

class FoulManager
{
	bool foul;
	bool lose;
public:
	// ��Ģ�� ������ ��� true ��ȯ
	// �Ŀ�
	bool isNoHitHandBall();	// �ƹ� ���� ������ ���� ��,
	bool isHandBallInHole();	// �� ���� ���ۿ� �־��� ��,
	bool isFirstHitNotMyBall();// �ڽ��� ���� ���� ������ ���� ��

	//�й�
	bool isEightBallBadToIn();	// �ڽ��� ���� �� ���� �ʰ�, 8�� �� ����
	bool isEightBallWithFoul();	// 8�� ���� �����鼭 �Ŀ�

	bool isFoul();// �Ŀ��̸� �������� ����

	bool isLose();//�й��̸� �й��� ����
};

#endif