#ifndef _PLAYER_
#define _PLAYER_

enum class TargetBall 
{ 
	NONE, STRIPE, PLAIN
};
// 타겟 볼의 속성을 정의
// (NONE은 아무도 공을 맞추지 않아서 정해지지 않은 경우)


class Player
{
private:
	/* Static func or member. */
	static TargetBall FirstPlayerTargetBall;	// 1번 플레이어의 타겟 볼

	/* Instance func or member. */
	unsigned int playerID;						// 이 플레이어의 ID
	unsigned int numTakenBall;					// 획득한 공의 개수
	TargetBall playerTargetBallType;			//
	bool isAppropriatePlayer();

public:
	/* Static func or member. */

	/* Instance func or member. */
	Player(unsigned int playerID);
	void addTakenBall(unsigned int numBall);	// 지금 획득한 공의 개수를 누적시킴
	unsigned int getNumTakenBall() const;		// 획득한 공의 개수를 반환함
};

#endif