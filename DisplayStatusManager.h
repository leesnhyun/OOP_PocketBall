#ifndef _DISPLAY_STATUS_MANGER_
#define _DISPLAY_STATUS_MANGER_

class DisplayStatusManager
{
	// ��Ģ�� ������ ��� true ��ȯ
	// �Ŀ�
	void isNoHitHandBall();	// �ƹ� ���� ������ ���� ��,
	void isHandBallInHole();	// �� ���� ���ۿ� �־��� ��,
	void isFirstHitNotMyBall();// �ڽ��� ���� ���� ������ ���� ��

	//�й�
	bool isEightBallBadToIn();	// �ڽ��� ���� �� ���� �ʰ�, 8�� �� ����
	bool isEightBallWithFoul();	// 8�� ���� �����鼭 �Ŀ�

public:

	DisplayStatusManager();

	bool update();

};

#endif