#ifndef _FOULMANAGER_
#define _FOULMANAGER_

class FoulManager
{
	// 규칙을 위반한 경우 true 반환
	// 파울
	void isNoHitHandBall(); // 아무 공도 맞추지 못한 때,
	void isHandBallInHole(); // 흰 공을 구멍에 넣었을 때,
	void isFirstHitNotMyBall();// 자신의 공을 먼저 맞추지 않은 때

	//패배
	bool isEightBallBadToIn(); // 자신의 공을 다 넣지 않고, 8번 공 넣음
	bool isEightBallWithFoul(); // 8번 공을 넣으면서 파울

public:
	FoulManager();

	void reset();

	void checkFoul();// 파울이면 프리볼을 선언
	bool isLose();//패배이면 패배라고 선언
};

#endif

