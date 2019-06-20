#include "Vector2.h"

#ifndef TILEMANAGER_H
#define TILEMANAGER_H
#define TILESIZE 30
#include "Tile.h"

class TileManager
{
private:
	Tile Tiles[BOARDSIZE][BOARDSIZE];
public:
	void initializeTiles();

	TileManager();
	~TileManager();

	Tile* getTile(const int _xPos,const int _yPos);

	void placeGamePiece(GamePiece& _gamePiece, int _xPos, int _yPos);
	Vector2 getClosestTile(int _xPixelPos, int _yPixelPos) const;
	float distance(const Vector2& _posA, const Vector2& _posB) const;
	bool findSpot(const Vector2& _originPos, const int _range, Vector2& reference) const;
	bool isInBounds(const Vector2& _position) const;
	bool isInBounds(const int _xPos, const int _yPos) const;
	bool isInBoundsAndNotEnemy(const int _xPos, const int _yPos, const int _playerID) const;
	bool isFieldOccupied(const Vector2 _position) const;
	bool isFieldOccupied(const int _xPos,const int _yPos) const;
};
#endif
