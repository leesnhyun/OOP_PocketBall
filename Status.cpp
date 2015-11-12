#include "Status.h"
#include <algorithm>

using std::find_if;
using std::exception;

Status::Status(vector<Player*> playerList)
{
	this->playerList = vector<Player*>(playerList.begin(), playerList.end());
	this->currentTurnCount = 0;
	this->foulStatus = false;
	this->gameEndStatus = false;
	this->turnProgressStatus = false;
	this->turnChangeStatus = false;
	this->pNowPlayer = playerList.at(0);
}

Player* Status::getTurnPlayer() noexcept
{
	return this->pNowPlayer;
}

Player* Status::getNotTurnPlayer() noexcept
{
	Player* nowPlayer = this->pNowPlayer;
	vector<Player*>::iterator foundIndex = find_if(this->playerList.begin(), this->playerList.end(), [nowPlayer](Player* pPlayer) {
		return (pPlayer->getPlayerId() != nowPlayer->getPlayerId());
	});

	return *foundIndex;
}

vector<int> Status::getPlayerIdList() const noexcept
{
	vector<int> idList;

	for (Player* player : this->playerList)
	{
		idList.push_back(player->getPlayerId());
	}

	return idList;
}

void Status::setTurnPlayer(int playerID)
{
	vector<Player*>::iterator foundIndex = find_if(this->playerList.begin(), this->playerList.end(), [playerID](Player* pPlayer) {
		return (pPlayer->getPlayerId() == playerID);
	});

	if (foundIndex == this->playerList.end())
	{
		throw PlayerNotFoundException("플레이어 ID에 맞는 플레이어를 찾을 수 없습니다.");
	}

	this->pNowPlayer = *foundIndex;
}

bool Status::getFoulStatus() const noexcept
{
	return this->foulStatus;
}

bool Status::getTurnProgressStatus() const noexcept
{
	return this->turnProgressStatus;
}

bool Status::getTurnChangeStatus() const noexcept
{
	return this->turnChangeStatus;
}

bool Status::getGameEndStatus() const noexcept
{
	return this->gameEndStatus;
}

int Status::getCurrentTurnCount() const noexcept
{
	return this->currentTurnCount;
}

void Status::setFoulStatus(bool toSet) noexcept
{
	this->foulStatus = toSet;
}

void Status::setTurnProgressStatus(bool toSet) noexcept
{
	this->turnProgressStatus = toSet;
}

void Status::setTurnChangeStatus(bool toSet) noexcept
{
	this->turnChangeStatus = toSet;
}

void Status::setGameEndStatus(bool toSet) noexcept
{
	this->gameEndStatus = toSet;
}

void Status::nextTurnCount() noexcept
{
	this->currentTurnCount++;
}