#ifndef _STATUS_
#define _STATUS_
#include <exception>
#include <vector>
#include "Player.h"
#define noexcept throw ()
#define BLANK_BLOCK {}

using std::vector;
using std::exception;

/* Custom Exception Class. */
class PlayerNotFoundException : public exception
{
private:
	char * exceptionMessageString;
public:
	PlayerNotFoundException(const char * exceptionMessage)
	{
		strcpy(this->exceptionMessageString, exceptionMessage);
	}

	virtual const char * what() const noexcept
	{
		return exceptionMessageString;

	}
};

class Status
{

private:
	bool foulStatus;
	bool turnProgressStatus;
	bool turnChangeStatus;
	vector<Player *> playerList;
	Player* pNowPlayer;
	int currentTurnCount;
	int winnerPlayer;

public:
	Status(vector<Player *> playerList);
	Player* getTurnPlayer() noexcept;
	Player* getNotTurnPlayer() noexcept;
	vector<int> getPlayerIdList() const noexcept;
	bool getFoulStatus() const noexcept;
	bool getTurnProgressStatus() const noexcept;
	bool getTurnChangeStatus() const noexcept;
	bool getGameEndStatus() const noexcept;
	int getCurrentTurnCount() const noexcept;
	int getWinnerPlayer() const;
	void setWinnerPlayer(int winner) noexcept;
	void setFoulStatus(bool toSet) noexcept;
	void setTurnProgressStatus(bool toSet) noexcept;
	void setTurnChangeStatus(bool toSet) noexcept;
	void setTurnPlayer(int playerID) throw (PlayerNotFoundException);
	void nextTurnCount() noexcept;

};

#endif