#pragma once
#include "GamePiece.h"

#ifndef UNIT_H
#define UNIT_H
class TileManager;

class Unit : public GamePiece {
private:
	int controllingPlayerID = -1;
	bool isSelected = false;
	UNIT_TYPE unitType = pawn;

public:
	inline UNIT_TYPE getUnitType()const { return unitType; }
	inline void selectUnit() { isSelected = true; }
	inline void deSelectUnit() { isSelected = false; }
	inline bool isUnitSelected() const { return isSelected; }

	Unit();
	Unit(const int _playerID);
	Unit(const UNIT_TYPE _type, const int _playerID);
	Unit(const UNIT_TYPE _type, const Vector2 _position, const int _playerID, TileManager& _tileManager);
	~Unit();

	inline int getControllingPlayerID() const { return controllingPlayerID; }

	void spawnGamePiece(const Vector2& _position, TileManager& _tileManager);
	bool checkMovement(const Vector2& _targetPosition, TileManager& _tileManager) const;
	void moveUnit(const Vector2& _moveTarget, TileManager& _tileManager);

	bool pawnMovement(const Vector2& _targetPosition, TileManager& _tileManager)	const;
	bool kingMovement(const Vector2& _targetPosition, TileManager& _tileManager)	const;
	bool rookMovement(const Vector2& _targetPosition, TileManager& _tileManager)	const;
	bool rogueMovement(const Vector2& _targetPosition, TileManager& _tileManager)	const;
	bool bishopMovement(const Vector2& _targetPosition, TileManager& _tileManager)	const;

};
#endif