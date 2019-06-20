#pragma once
#include<vector>
#include "../Board/Vector2.h"
#include "../Defines.h"

#ifndef GAMEPIECE_H
#define GAMEPIECE_H
class TileManager;

class GamePiece {
private:
	Vector2 position;
	bool occupiesField = false;
public:
	GamePiece();
	GamePiece(Vector2 _position);
	~GamePiece();

	inline void setPosition(int _xPosition, int _yPosition) { position = Vector2(_xPosition, _yPosition); }
	inline void setPosition(Vector2 _position) { position = _position; }

	inline bool doesOccupieField() const { return occupiesField; }
	inline void setOccupiesField(bool _value) { occupiesField = _value; }

	inline Vector2 getPosition() const { return position; }
	inline int getXPos() const { return position.xPos; }
	inline int getYPos() const { return position.yPos; }

	virtual void spawnGamePiece(const Vector2& _position, TileManager& _tileManager) = 0;
	bool isOnBoard();
};
#endif