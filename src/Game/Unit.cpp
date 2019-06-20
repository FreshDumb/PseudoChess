#include "Unit.h"
#include "..\Board\TileManager.h"

Unit::Unit() :
	GamePiece()
{
}

Unit::Unit(const int _playerID) {
	setPosition(-1, -1);
	controllingPlayerID = _playerID;
	setOccupiesField(true);
}

Unit::Unit(const UNIT_TYPE _type, const int _playerID) {
	controllingPlayerID = _playerID;
	setOccupiesField(true);
	setPosition(-1,-1);
	unitType = _type;
}

Unit::Unit(const UNIT_TYPE _type, const Vector2 _position, const int _playerID, TileManager& _tileManager) {
	controllingPlayerID = _playerID;
	setOccupiesField(true);
	setPosition(_position);
	Vector2 tempVector = getPosition();
	_tileManager.getTile(tempVector.xPos,tempVector.yPos)->setUnitOnTile(this);
	unitType = _type;
}

Unit::~Unit() {

}

void Unit::spawnGamePiece(const Vector2& _position, TileManager& _tileManager) {
	if (_tileManager.isFieldOccupied(_position) == false) {
		setPosition(_position);
		_tileManager.getTile(getXPos(), getYPos())->setUnitOnTile(this);
	}
	else {
		Vector2 tempVector;
		if (_tileManager.findSpot(_position, 0, tempVector) == false) {
			abort();
		}
		setPosition(tempVector);
		_tileManager.getTile(getXPos(), getYPos())->setUnitOnTile(this);
	}
}

bool Unit::pawnMovement(const Vector2& _targetPosition, TileManager& _tileManager)		const {
	Vector2 PositionDelta = _targetPosition - getPosition();
	if (PositionDelta == Vector2(+ 1, 0) ||
		PositionDelta == Vector2(0, + 1) ||
		PositionDelta == Vector2(- 1, 0) ||
		PositionDelta == Vector2(0, - 1))
		return true;

	return false;
}
bool Unit::kingMovement(const Vector2& _targetPosition, TileManager& _tileManager)		const {
	Vector2 PositionDelta = _targetPosition - getPosition();
	if (PositionDelta == Vector2(+ 1, 0) ||
		PositionDelta == Vector2(0, + 1) ||
		PositionDelta == Vector2(- 1, 0) ||
		PositionDelta == Vector2(0, - 1) ||
		PositionDelta == Vector2(+ 1,+ 1) ||
		PositionDelta == Vector2(- 1, + 1) ||
		PositionDelta == Vector2(- 1, - 1) ||
		PositionDelta == Vector2(+ 1, - 1))
		return true;

	return false;
}
bool Unit::rookMovement(const Vector2& _targetPosition, TileManager& _tileManager)		const {
	Vector2 currentPosTemp = getPosition();
	Vector2 tempVector = (_targetPosition - currentPosTemp).normalize();
	Vector2 starting = currentPosTemp + tempVector;
	while (starting != _targetPosition && _tileManager.isInBounds(starting.xPos, starting.yPos)) {
		if (_tileManager.isFieldOccupied(starting.xPos, starting.yPos))
			return false;
		starting = starting + tempVector;
	}
	if (_targetPosition.yPos == getYPos())
		return true;
	if (_targetPosition.xPos == getXPos())
		return true;
	return false;
}
bool Unit::rogueMovement(const Vector2& _targetPosition, TileManager& _tileManager)	const {
	Vector2 PositionDelta = _targetPosition - getPosition();
	if (PositionDelta == Vector2(+ 2, + 1) ||
		PositionDelta == Vector2(- 2, + 1) ||
		PositionDelta == Vector2(+ 2, - 1) ||
		PositionDelta == Vector2(- 2, - 1) ||

		PositionDelta == Vector2(+ 1, + 2) ||
		PositionDelta == Vector2(+ 1, - 2) ||
		PositionDelta == Vector2(- 1, + 2) ||
		PositionDelta == Vector2(- 1, - 2))
		return true;

	return false;
}
bool Unit::bishopMovement(const Vector2& _targetPosition, TileManager& _tileManager)	const {
	Vector2 currentPosTemp = getPosition();
	Vector2 tempVector = (_targetPosition - currentPosTemp).normalize();
	Vector2 starting = currentPosTemp + tempVector;
	while (starting != _targetPosition && _tileManager.isInBounds(starting.xPos, starting.yPos)) {
		if (_tileManager.isFieldOccupied(starting.xPos, starting.yPos))
			return false;
		starting = starting + tempVector;

	}
	tempVector = _targetPosition - currentPosTemp;
	if (abs(tempVector.xPos) == abs(tempVector.yPos))
		return true;

	return false;
}

bool Unit::checkMovement(const Vector2& _targetPosition, TileManager& _tileManager) const {
	switch (unitType) {
	case pawn:
		return pawnMovement(_targetPosition, _tileManager);
	case king:
		return kingMovement(_targetPosition, _tileManager);
	case rook:
		return rookMovement(_targetPosition, _tileManager);
	case rogue:
		return rogueMovement(_targetPosition, _tileManager);
	case bishop:
		return bishopMovement(_targetPosition, _tileManager);
	}
	return false;
}

void Unit::moveUnit(const Vector2& _moveTarget, TileManager& _tileManager) {
	_tileManager.getTile(getXPos(), getYPos())->setUnitOnTile(nullptr);
	setPosition(_moveTarget);
	_tileManager.getTile(getXPos(), getYPos())->setUnitOnTile(this);
}