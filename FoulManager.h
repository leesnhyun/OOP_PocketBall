#ifndef _FOULMANAGER_
#define _FOULMANAGER_

class FoulManager
{
	// ��Ģ�� ������ ��� true ��ȯ
	// �Ŀ�
	void isNoHitHandBall(); // �ƹ� ���� ������ ���� ��,
	void isHandBallInHole(); // �� ���� ���ۿ� �־��� ��,
	void isFirstHitNotMyBall();// �ڽ��� ���� ���� ������ ���� ��

	//�й�
	bool isEightBallBadToIn(); // �ڽ��� ���� �� ���� �ʰ�, 8�� �� ����
	bool isEightBallWithFoul(); // 8�� ���� �����鼭 �Ŀ�

public:
	FoulManager();

	void reset();

	void checkFoul();// �Ŀ��̸� �������� ����
	bool isLose();//�й��̸� �й��� ����
};

#endif

