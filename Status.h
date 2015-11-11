#ifndef _STATUS_
#define _STATUS_
#include <exception>
#include <vector>
#include "Player.h"
#define noexcept throw ()
#define BLANK_BLOCK {}

using std::vector;

class Status
{
private:
	bool foulStatus;
	bool turnProgressStatus;
	bool turnChangeStatus;
	bool gameEndStatus;
	vector<Player> playerList;
	Player nowPlayer;
	int currentTurnCount;
public:
	Status(vector<Player> playerList);
	Player& getTurnPlayer() noexcept;
	bool getFoulStatus() const noexcept;
	bool getTurnProgressStatus() const noexcept;
	bool getTurnChangeStatus() const noexcept;
	bool getGameEndStatus() const noexcept;
	int getCurrentTurnCount() const noexcept;
	void setFoulStatus(bool toSet) noexcept;
	void setTurnProgressStatus(bool toSet) noexcept;
	void setTurnChangeStatus(bool toSet) noexcept;
	void setGameEndStatus(bool toSet) noexcept;
	void Status::setTurnPlayer(int playerID) throw (PlayerNotFoundException);
	void nextTurnCount() noexcept;
};

/* Custom Exception Class. */
class PlayerNotFoundException : public exception
{
private:
	const char * exceptionMessageString;
public:
	PlayerNotFoundException(const char * exceptionMessage);
	virtual const char * what() const noexcept
	{
		return exceptionMessageString;

	}
};

#endif