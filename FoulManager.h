#ifndef _FOULMANAGER_
#define _FOULMANAGER_

class FoulManager
{

public:
	// 파울
	bool isNoCrashHandBall();	// 아무 공도 맞추지 못한 때,
	bool isHandBallInHall();	// 흰 공을 구멍에 넣었을 때,
	bool isFirstHitIsMyBall();// 자신의 공을 먼저 맞추지 않은 때

	//패배
	bool isEightBallGoodToIn();	// 자신의 공을 다 넣지 않고, 8번 공 넣음
	bool isEightBallWithFoul();	// 8번 공을 넣으면서 파울

	bool foul();// 파울이면 프리볼을 선언

	bool lose();//패배이면 패배라고 선언
};

#endif