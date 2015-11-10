#ifndef _PLAYER_
#define _PLAYER_

enum class BallType
{ 
	HAND, EIGHT, STRIPE, SOLID, NONE
};
// 타겟 볼의 속성을 정의
// (NONE은 아무도 공을 맞추지 않아서 정해지지 않은 경우)


class Player
{

private:
	/* Instance func or member. */
	unsigned int playerID;						// 이 플레이어의 ID
	unsigned int numTakenBall;					// 획득한 공의 개수

public:
	/* Instance func or member. */
	Player(unsigned int playerID);
	BallType playerBallType;					// 플레이어가 목표로 하는 공의 종류
	void addTakenBall(unsigned int numBall);	// 지금 획득한 공의 개수를 누적시킴
	unsigned int getNumTakenBall() const;		// 획득한 공의 개수를 반환함
	unsigned int getPlayerId() const;			// 플레이어의 ID를 가져옴.

};

#endif