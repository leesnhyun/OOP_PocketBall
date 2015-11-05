#ifndef _PLAYER_
#define _PLAYER_

enum targetBall{NONE, STRIPE, PLAIN};	// 타겟 볼의 속성을 정의
										// (NONE은 아무도 공을 맞추지 않아서 정해지지 않은 경우)

class Player
{
	unsigned int numTakenBall;	// 획득한 공의 개수

public:

	static bool isFirstPlayerTurn;	// 1번 플레이어의 차례인지 확인
	static targetBall FirstPlayerTargetBall;	// 1번 플레이어의 타겟 볼

	Player();

	void addTakenBall(unsigned int numBall); // 지금 획득한 공의 개수를 누적시킴

	unsigned getNumTakenBall() const;	// 획득한 공의 개수를 반환함
};

#endif