#pragma once
#include "Vector2.h"
#include "../Game/Unit.h"
#include "../Game/Pickup.h"
#include "../Defines.h"

#ifndef TILE_H
#define TILE_H
class Tile
{
private:
	int xIndex, yIndex;
	Unit* unitOnTile = nullptr;
	Pickup* pickupOnTile = nullptr;
public:
	inline Unit* getUnitOnTile() const { return unitOnTile; }
	inline void setUnitOnTile(Unit* _unit) { unitOnTile = _unit; }
	inline Pickup* getPickupOnTile() const { return pickupOnTile; }
	inline void setPickupOnTile(Pickup* _pickup) { pickupOnTile = _pickup; }

	inline int getXIndex() const { return xIndex; }
	inline int getYIndex() const { return yIndex; }
	inline Vector2 getIndex() const { return Vector2(xIndex, yIndex); }

	Tile();
	Tile(int _xIndex, int _yIndex);

	~Tile();
};
#endif

