#include "Tile.h"

Tile::Tile()
{
	Tile(0, 0);
}

Tile::Tile(int _xIndex, int _yIndex) : xIndex(_xIndex), yIndex(_yIndex) {

}

Tile::~Tile()
{
}