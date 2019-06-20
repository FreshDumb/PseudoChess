#include "Player.h"

void Player::initUnits(TileManager& _tileManager) {
	for (Unit& u : PlayerUnits) {
		_tileManager.placeGamePiece(u, startingPos.xPos, startingPos.yPos);
	}
}

Player::Player() {

}

Player::Player(int _PlayerID) {

}


Player::~Player() {

}

void Player::resetPlayer(TileManager& _tileManager) {
	score = 0;
	for (Unit& u : PlayerUnits) {
		_tileManager.getTile(u.getXPos(), u.getYPos())->setUnitOnTile(nullptr);
	}
	PlayerUnits.clear();
	playerRound = 0;
	startingPos = Vector2(-1, -1);
}
