#ifndef _DISPLAY_STATUS_MANGER_
#define _DISPLAY_STATUS_MANGER_

class DisplayStatusManager
{
	// 규칙을 위반한 경우 true 반환
	// 파울
	void isNoHitHandBall();	// 아무 공도 맞추지 못한 때,
	void isHandBallInHole();	// 흰 공을 구멍에 넣었을 때,
	void isFirstHitNotMyBall();// 자신의 공을 먼저 맞추지 않은 때

	//패배
	bool isEightBallBadToIn();	// 자신의 공을 다 넣지 않고, 8번 공 넣음
	bool isEightBallWithFoul();	// 8번 공을 넣으면서 파울

public:

	DisplayStatusManager();

	bool update();

};

#endif