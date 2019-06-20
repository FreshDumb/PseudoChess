#include "TileManager.h"
#include <random>
#include <iostream>

//
// Initializes the tile array Tiles with Tiles. Sets the xIndex and yIndex of the Tile to the appropriate values
void TileManager::initializeTiles() {
	for (int y = 0; y < BOARDSIZE; y++) {
		for (int x = 0; x < BOARDSIZE; x++) {
			Tiles[x][y] = Tile(x, y);
		}
	}
}

TileManager::TileManager()
{

}

TileManager::~TileManager()
{

}

// Gets the closest Tile on board from a given Pixelposition
Vector2 TileManager::getClosestTile(int _xPixelPos, int _yPixelPos) const {
	// Subtracts BOARDSIZE
	// Then devides by Tilesize to get the required position
	int tempX = (_xPixelPos - (BOARDSIZE)) / TILESIZE;
	int tempY = (_yPixelPos - (BOARDSIZE)) / TILESIZE;
	//
	// Checks if the position is out of bounds
	if (tempX > BOARDSIZE - 1)
		tempX = BOARDSIZE - 1;
	if (tempX < 0)
		tempX = 0;
	if (tempY > BOARDSIZE - 1)
		tempY = BOARDSIZE - 1;
	if (tempY < 0)
		tempY = 0;
	return Vector2(tempX, tempY);
}

float TileManager::distance(const Vector2& _posA, const Vector2& _posB) const {
	Vector2 temp = _posA - _posB;
	return (sqrt((float)(temp.xPos * temp.xPos) + (float)(temp.yPos * temp.yPos)));
}

// Returns a valid Tile around the _originPos on the board randomly, as a Vector2.
bool TileManager::findSpot(const Vector2& _originPos, const int _range, Vector2& _posReference) const {
	std::vector<Vector2> result;

	int xLowBound = _originPos.xPos - _range;
	if (xLowBound < 0)
		xLowBound = 0;
	int xUpperBound = _originPos.xPos + _range;
	if (xUpperBound > BOARDSIZE - 1)
		xUpperBound = BOARDSIZE - 1;
	int yLowBound = _originPos.yPos - _range;
	if (yLowBound < 0)
		yLowBound = 0;
	int yUpperBound = _originPos.yPos + _range;
	if (yUpperBound > BOARDSIZE - 1)
		yUpperBound = BOARDSIZE - 1;

	for (int x = xLowBound; x <= xUpperBound; x++) {
		for (int y = yLowBound; y <= yUpperBound; y++) {
			float tempDistance = distance(Tiles[x][y].getIndex(), _originPos);
			if (tempDistance <= _range + 0.5f && isFieldOccupied(Tiles[x][y].getIndex()) == false)
				result.push_back(Tiles[x][y].getIndex());
		}
	}
	if (result.empty()) {
		if (xLowBound == 0 && xUpperBound == BOARDSIZE - 1 && yLowBound == 0 && yUpperBound == BOARDSIZE - 1) {
			return false;
		}
		findSpot(_originPos, _range + 1, _posReference);
	}
	else {
		int resultIndex = rand() % result.size();
		_posReference = result.at(resultIndex);
		return true;
	}
}



// Checks if the given Vector2 (or individual X and Y positions) is in range of the Tiles Array 
// and returns true if that’s the case. Else it returns false
bool TileManager::isInBounds(const int _xPos, const int _yPos) const {
	if (_xPos > BOARDSIZE - 1)
		return false;
	if (_xPos < 0)
		return false;
	if (_yPos > BOARDSIZE - 1)
		return false;
	if (_yPos < 0)
		return false;

	return true;
}
bool TileManager::isInBounds(const Vector2& _position) const {
	if (_position.xPos > BOARDSIZE - 1)
		return false;
	if (_position.xPos < 0)
		return false;
	if (_position.yPos > BOARDSIZE - 1)
		return false;
	if (_position.yPos < 0)
		return false;

	return true;
}
bool TileManager::isInBoundsAndNotEnemy(const int _xPos, const int _yPos, const int _playerID) const {
	if (_xPos > BOARDSIZE - 1)
		return false;
	if (_xPos < 0)
		return false;
	if (_yPos > BOARDSIZE - 1)
		return false;
	if (_yPos < 0)
		return false;
	// If Occupied and controlled by Enemy
	// return false
	if (isFieldOccupied(_xPos, _yPos) && Tiles[_xPos][_yPos].getUnitOnTile()->getControllingPlayerID() == _playerID)
		return false;

	return true;
}

bool TileManager::isFieldOccupied(const Vector2 _position) const {
	if (isInBounds(_position) == false) {
		return false;
	}
	if (Tiles[_position.xPos][_position.yPos].getUnitOnTile() == nullptr) {
		return false;
	}
	return Tiles[_position.xPos][_position.yPos].getUnitOnTile()->doesOccupieField();
}

// Returns false when the Tile is at _xPos and _yPos is empty
// Returns true when there is a Unit on the Tile that occupies a field
bool TileManager::isFieldOccupied(const int _xPos, const int _yPos) const {
	if (isInBounds(_xPos, _yPos) == false) {
		return false;
	}
	if (Tiles[_xPos][_yPos].getUnitOnTile() == nullptr) {
		return false;
	}
	return Tiles[_xPos][_yPos].getUnitOnTile()->doesOccupieField();
}

void TileManager::placeGamePiece(GamePiece& _gamePiece, int _xPos, int _yPos) {
	_gamePiece.spawnGamePiece(Vector2(_xPos, _yPos), *this);
}

Tile* TileManager::getTile(const int _xPos, const int _yPos) {
	if (_xPos >= 0 &&
		_yPos >= 0 &&
		_xPos < BOARDSIZE &&
		_yPos < BOARDSIZE) {
		return &Tiles[_xPos][_yPos];
	}
	else {
		return nullptr;
	}
}