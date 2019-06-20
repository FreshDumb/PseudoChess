#include "GamePiece.h"

GamePiece::GamePiece(Vector2 _position) {
	position = _position;
}

GamePiece::GamePiece() {
	position = Vector2(-1,-1);
}

GamePiece::~GamePiece() {

}

bool GamePiece::isOnBoard() {
	if (position.xPos == -1 && position.yPos == -1)
		return false;
	else
		return true;
}