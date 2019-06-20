#include "GamePiece.h"

#ifndef PICKUP_H
#define PICKUP_H
class TileManager;
class Player;

class Pickup : public GamePiece {
public:
	Pickup();
	~Pickup();

	int doPickUp(Player& _player, TileManager& _tileManager);
	void spawnGamePiece(const Vector2& _position, TileManager& _tileManager);
};
#endif