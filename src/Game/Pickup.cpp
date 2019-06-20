#include "Pickup.h"
#include "..\Board\TileManager.h"
#include "Player.h"

Pickup::Pickup() {
	setPosition(-1, -1);
}

Pickup::~Pickup() {

}

void Pickup::spawnGamePiece(const Vector2& _position, TileManager& _tileManager) {
	setPosition( _position);
	_tileManager.getTile(_position.xPos, _position.yPos)->setPickupOnTile(this);
}

int Pickup::doPickUp(Player& _player, TileManager& _tileManager) {
	_tileManager.getTile(getXPos(), getYPos())->setPickupOnTile(nullptr);
	_player.incrementScore();
	setPosition(-1, -1);
	return _player.getScore();
}