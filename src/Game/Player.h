#pragma once
#include "Unit.h"
#include "Pickup.h"
#include "..\Board\TileManager.h"

class Player {
private:
	int playerRound = 0;
	Unit* selectedUnit = nullptr;
	int score = 0;
	Vector2 startingPos = Vector2(-1, -1);

public:
	std::vector<Unit> PlayerUnits;

	inline void setStartingPos(const Vector2 _position) { startingPos = _position; }
	inline Unit* getSelectedUnit() { return selectedUnit; }
	inline void setSelectedUnit(Unit* _unit) { selectedUnit = _unit; }
	inline void incrementScore() { score++; }
	inline int getScore() const { return score; }

	inline void setPlayerRound(const int _PlayerRound) { playerRound = _PlayerRound; }
	inline void incrementPlayerRound() { playerRound++; }
	inline int getPlayerRound() const { return playerRound; }

	void initUnits(TileManager& _tileManager);
	void resetPlayer(TileManager& _tileManager);

	Player();
	Player(int _PlayerID);
	~Player();
};