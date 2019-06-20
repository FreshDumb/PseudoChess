#include "Renderer.h"
#include "Game/Game.h"
#include <sstream>

Renderer::Renderer() {
	rendererRef = NULL;
	windowRef = NULL;
}

Renderer::Renderer(SDL_Renderer* _rendererRef, SDL_Window* _windowRef, UILoader* _UILoaderRef) {
	rendererRef = _rendererRef;
	windowRef = _windowRef;
	UILoaderRef = _UILoaderRef;
	SDL_SetRenderDrawBlendMode(rendererRef, SDL_BLENDMODE_BLEND);
}

Renderer::~Renderer()
{

}

void Renderer::drawTiles(const TileManager& _tileManager) const {
	bool colored = true;
	for (int y = 0; y < BOARDSIZE; y++) {
		for (int x = 0; x < BOARDSIZE; x++) {
			if (colored) {
				drawSingleTile(Color(255 - 50, 221 - 50, 238 - 50, 255), Vector2(x, y));
			}
			else {
				drawSingleTile(Color(100, 100, 255, 255), Vector2(x, y));
			}
			colored = !colored;
		}
		colored = !colored;
	}

}

void Renderer::drawBorder(Color _color)const {
	SDL_SetRenderDrawColor(rendererRef, _color.r, _color.g, _color.b, _color.a);
	SDL_Rect Border;
	Border.x = 0;
	Border.y = 0;
	Border.w = BorderSize * 2 + BOARDSIZE * TILESIZE;
	Border.h = BorderSize * 2 + BOARDSIZE * TILESIZE;
	SDL_RenderFillRect(rendererRef, &Border);
}

void Renderer::drawSingleTile(const Color _color, const Vector2 _position)const {
	SDL_SetRenderDrawColor(rendererRef, _color.r, _color.g, _color.b, _color.a);
	SDL_Rect tempRect;
	tempRect.x = BorderSize + _position.xPos * TILESIZE;
	tempRect.y = BorderSize + _position.yPos * TILESIZE;
	tempRect.w = TILESIZE;
	tempRect.h = TILESIZE;
	SDL_RenderFillRect(rendererRef, &tempRect);
}

bool Renderer::drawMovementUITile(Color _color, const int _xPos, const int _yPos, const Unit& _Unit, TileManager& _tileManager) const {
	bool returnValue = false;
	//
	// Check for Enemies
	if (_tileManager.getTile(_xPos, _yPos)->getUnitOnTile() != nullptr &&
		_tileManager.getTile(_xPos, _yPos)->getUnitOnTile()->getControllingPlayerID() != _Unit.getControllingPlayerID()) {
		SDL_SetRenderDrawColor(rendererRef, _color.r + 100, _color.g - 20, _color.b - 20, _color.a + 50);
		returnValue = true;
	}
	else {
		SDL_SetRenderDrawColor(rendererRef, _color.r, _color.g, _color.b, _color.a);
	}
	SDL_Rect tempRect;
	tempRect.x = BorderSize + _xPos * TILESIZE;
	tempRect.y = BorderSize + _yPos * TILESIZE;
	tempRect.w = TILESIZE;
	tempRect.h = TILESIZE;
	SDL_RenderFillRect(rendererRef, &tempRect);
	return returnValue;
}

void Renderer::drawBoard(const TileManager& _tileManager) const {
	drawBorder(Color(150, 10, 10, 255));
	drawTiles(_tileManager);
}

void Renderer::drawUnits(const Game& _gameRef)const {
	for (int p = 0; p < MAXPLAYERS; p++) {
		for (int u = 0; u < UNITSPERPLAYER; u++) {
			Unit* tempUnitPointer = &_gameRef.getPlayers()[p].PlayerUnits.at(u);
			if (tempUnitPointer->isOnBoard())
				drawSingleUnit(*tempUnitPointer);
		}
	}
}

void Renderer::drawSingleUnit(const Unit& _unitToDraw)const {
	SDL_Rect tempRect = SDL_Rect();
	tempRect.x = BorderSize + _unitToDraw.getXPos() * TILESIZE;
	tempRect.y = BorderSize + _unitToDraw.getYPos() * TILESIZE;
	tempRect.w = TILESIZE;
	tempRect.h = TILESIZE;

	SDL_Texture* temp = PawnImg;
	switch (_unitToDraw.getUnitType()) {
	case pawn:
		temp = PawnImg;
		break;
	case rogue:
		temp = RogueImg;
		break;
	case rook:
		temp = RookImg;
		break;
	case king:
		temp = KingImg;
		break;
	case bishop:
		temp = BishopImg;
		break;
	default:
		temp = PawnImg;
	}

	int y;
	if (_unitToDraw.isUnitSelected() == false) {
		//
		// Currently only supports white and black pieces
		int tempPlayerID = _unitToDraw.getControllingPlayerID();
		y = SDL_SetTextureColorMod(temp, (tempPlayerID + 1) % 2 * 255, (tempPlayerID + 1) % 2 * 255, (tempPlayerID + 1) % 2 * 255);
	}
	else {
		y = SDL_SetTextureColorMod(temp, 128, 128, 128);
	}

	int x = SDL_RenderCopy(rendererRef,
		temp,
		NULL,
		&tempRect);
}

void Renderer::drawPawnMovementTiles(const Unit& _unit, TileManager& _tileManager)const {
	Color tempColor = Color(50, 30, 30, 150);
	Vector2 tempPosition = _unit.getPosition();
	int tempPlayerID = _unit.getControllingPlayerID();
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos + 1, tempPosition.yPos, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos + 1, tempPosition.yPos, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos, tempPosition.yPos + 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos, tempPosition.yPos + 1, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos, tempPosition.yPos - 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos, tempPosition.yPos - 1, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos - 1, tempPosition.yPos, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos - 1, tempPosition.yPos, _unit, _tileManager);
}
void Renderer::drawKingMovementTiles(const Unit& _unit, TileManager& _tileManager)const {
	Color tempColor = Color(50, 30, 30, 150);
	Vector2 tempPosition = _unit.getPosition();
	int tempPlayerID = _unit.getControllingPlayerID();
	//
	// Left,Right,Top,Down
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos + 1, tempPosition.yPos, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos + 1, tempPosition.yPos, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos, tempPosition.yPos + 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos, tempPosition.yPos + 1, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos, tempPosition.yPos - 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos, tempPosition.yPos - 1, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos - 1, tempPosition.yPos, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos - 1, tempPosition.yPos, _unit, _tileManager);
	//
	//	Diagonals
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos + 1, tempPosition.yPos + 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos + 1, tempPosition.yPos + 1, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos - 1, tempPosition.yPos + 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos - 1, tempPosition.yPos + 1, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos + 1, tempPosition.yPos - 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos + 1, tempPosition.yPos - 1, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos - 1, tempPosition.yPos - 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos - 1, tempPosition.yPos - 1, _unit, _tileManager);
}
void Renderer::drawRogueMovementTiles(const Unit& _unit, TileManager& _tileManager)const {
	Color tempColor = Color(50, 30, 30, 150);
	Vector2 tempPosition = _unit.getPosition();
	int tempPlayerID = _unit.getControllingPlayerID();

	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos + 2, tempPosition.yPos + 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos + 2, tempPosition.yPos + 1, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos + 2, tempPosition.yPos - 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos + 2, tempPosition.yPos - 1, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos - 2, tempPosition.yPos + 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos - 2, tempPosition.yPos + 1, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos - 2, tempPosition.yPos - 1, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos - 2, tempPosition.yPos - 1, _unit, _tileManager);

	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos + 1, tempPosition.yPos + 2, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos + 1, tempPosition.yPos + 2, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos - 1, tempPosition.yPos + 2, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos - 1, tempPosition.yPos + 2, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos + 1, tempPosition.yPos - 2, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos + 1, tempPosition.yPos - 2, _unit, _tileManager);
	if (_tileManager.isInBoundsAndNotEnemy(tempPosition.xPos - 1, tempPosition.yPos - 2, tempPlayerID))
		drawMovementUITile(tempColor, tempPosition.xPos - 1, tempPosition.yPos - 2, _unit, _tileManager);

}
void Renderer::drawRookMovementTiles(const Unit& _unit, TileManager& _tileManager)const {
	Color tempColor = Color(50, 30, 30, 150);
	Vector2 tempPosition = _unit.getPosition();
	int tempPlayerID = _unit.getControllingPlayerID();

	Vector2 direction = Vector2(-1, 0);
	Vector2 currentPoint = tempPosition + direction;
	while (_tileManager.isInBoundsAndNotEnemy(currentPoint.xPos, currentPoint.yPos, tempPlayerID)) {
		if (drawMovementUITile(tempColor, currentPoint.xPos, currentPoint.yPos, _unit, _tileManager))
			break;
		currentPoint = currentPoint + direction;
	}
	direction = -direction;
	currentPoint = tempPosition + direction;
	while (_tileManager.isInBoundsAndNotEnemy(currentPoint.xPos, currentPoint.yPos, tempPlayerID)) {
		if (drawMovementUITile(tempColor, currentPoint.xPos, currentPoint.yPos, _unit, _tileManager))
			break;
		currentPoint = currentPoint + direction;
	}
	direction.xPos = 0;
	direction.yPos = 1;
	currentPoint = tempPosition + direction;
	while (_tileManager.isInBoundsAndNotEnemy(currentPoint.xPos, currentPoint.yPos, tempPlayerID)) {
		if (drawMovementUITile(tempColor, currentPoint.xPos, currentPoint.yPos, _unit, _tileManager))
			break;
		currentPoint = currentPoint + direction;
	}
	direction = -direction;
	currentPoint = tempPosition + direction;
	while (_tileManager.isInBoundsAndNotEnemy(currentPoint.xPos, currentPoint.yPos, tempPlayerID)) {
		if (drawMovementUITile(tempColor, currentPoint.xPos, currentPoint.yPos, _unit, _tileManager))
			break;
		currentPoint = currentPoint + direction;
	}
}
void Renderer::drawBishopMovementTiles(const Unit& _unit, TileManager& _tileManager)const {
	Color tempColor = Color(50, 30, 30, 150);
	Vector2 tempPosition = _unit.getPosition();
	int tempPlayerID = _unit.getControllingPlayerID();

	Vector2 direction = Vector2(-1, -1);
	Vector2 currentPoint = tempPosition + direction;
	while (_tileManager.isInBoundsAndNotEnemy(currentPoint.xPos, currentPoint.yPos, tempPlayerID)) {
		if (drawMovementUITile(tempColor, currentPoint.xPos, currentPoint.yPos, _unit, _tileManager))
			break;
		currentPoint = currentPoint + direction;
	}
	direction = -direction;
	currentPoint = tempPosition + direction;
	while (_tileManager.isInBoundsAndNotEnemy(currentPoint.xPos, currentPoint.yPos, tempPlayerID)) {
		if (drawMovementUITile(tempColor, currentPoint.xPos, currentPoint.yPos, _unit, _tileManager))
			break;
		currentPoint = currentPoint + direction;
	}
	direction.xPos *= -1;
	currentPoint = tempPosition + direction;
	while (_tileManager.isInBoundsAndNotEnemy(currentPoint.xPos, currentPoint.yPos, tempPlayerID)) {
		if (drawMovementUITile(tempColor, currentPoint.xPos, currentPoint.yPos, _unit, _tileManager))
			break;
		currentPoint = currentPoint + direction;
	}
	direction = -direction;
	currentPoint = tempPosition + direction;
	while (_tileManager.isInBoundsAndNotEnemy(currentPoint.xPos, currentPoint.yPos, tempPlayerID)) {
		if (drawMovementUITile(tempColor, currentPoint.xPos, currentPoint.yPos, _unit, _tileManager))
			break;
		currentPoint = currentPoint + direction;
	}
}

void Renderer::drawUnitMovement(const Unit& _Unit, TileManager& _tileManager)const {
	switch (_Unit.getUnitType()) {
	case pawn:
		drawPawnMovementTiles(_Unit, _tileManager);
		break;
	case king:
		drawKingMovementTiles(_Unit, _tileManager);
		break;
	case rook:
		drawRookMovementTiles(_Unit, _tileManager);
		break;
	case rogue:
		drawRogueMovementTiles(_Unit, _tileManager);
		break;
	case bishop:
		drawBishopMovementTiles(_Unit, _tileManager);
		break;
	}

}

void Renderer::drawEverything(const Game& _gameRef, TileManager& _tileManager) const {
	//
	// Clear the Screen
	SDL_SetRenderDrawColor(rendererRef, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(rendererRef);

	drawBoard(_tileManager);
	drawUIArea();
	drawUnits(_gameRef);
	drawPickup(_gameRef);
	if (_gameRef.hasPlayerConfirmed() == false) {
		if (_gameRef.isTheGameOver() == true)
			drawWinScreen(_gameRef);
		else
			drawPlayerReadyUI(_gameRef);
	}
	if (_gameRef.hasPlayerConfirmed() == true)
		drawTurnIndicator(_gameRef);
	drawScore(_gameRef);
}

void Renderer::drawPickup(const Game& _gameRef) const {
	if (_gameRef.gameHasPickup() == true) {
		SDL_Rect tempRect = SDL_Rect();
		tempRect.x = BorderSize + _gameRef.getPickupPos().xPos * TILESIZE;
		tempRect.y = BorderSize + _gameRef.getPickupPos().yPos * TILESIZE;
		tempRect.w = TILESIZE;
		tempRect.h = TILESIZE;

		SDL_Texture* temp = CoinImg;
		int x = SDL_RenderCopy(rendererRef,
			temp,
			NULL,
			&tempRect);
	}
}

void Renderer::createTextures() {
	PawnImg = SDL_CreateTextureFromSurface(rendererRef, UILoaderRef->UnitSurf[0]);
	RogueImg = SDL_CreateTextureFromSurface(rendererRef, UILoaderRef->UnitSurf[1]);
	RookImg = SDL_CreateTextureFromSurface(rendererRef, UILoaderRef->UnitSurf[2]);
	KingImg = SDL_CreateTextureFromSurface(rendererRef, UILoaderRef->UnitSurf[3]);
	BishopImg = SDL_CreateTextureFromSurface(rendererRef, UILoaderRef->UnitSurf[4]);
	TargetImg = SDL_CreateTextureFromSurface(rendererRef, UILoaderRef->TargetSurf);
	CoinImg = SDL_CreateTextureFromSurface(rendererRef, UILoaderRef->PickupSurf[0]);
	SDL_FreeSurface(UILoaderRef->UnitSurf[0]);
	SDL_FreeSurface(UILoaderRef->UnitSurf[1]);
	SDL_FreeSurface(UILoaderRef->UnitSurf[2]);
	SDL_FreeSurface(UILoaderRef->UnitSurf[3]);
	SDL_FreeSurface(UILoaderRef->UnitSurf[4]);
	SDL_FreeSurface(UILoaderRef->TargetSurf);
	SDL_FreeSurface(UILoaderRef->PickupSurf[0]);
}

void Renderer::drawUIArea() const {
	SDL_SetRenderDrawColor(rendererRef, 255, 255, 255, 255);
	SDL_Rect tempRect;
	tempRect.x = TILESIZE * 11 + BorderSize;
	tempRect.y = 0 + BorderSize;
	tempRect.w = WINDOWWIDTH - TILESIZE * 12;
	tempRect.h = TILESIZE * 10;
	SDL_RenderFillRect(rendererRef, &tempRect);
	//
	// Draw Line to divide Players
	SDL_SetRenderDrawColor(rendererRef, 0, 0, 0, 255);
	tempRect.x = WINDOWWIDTH - ((WINDOWWIDTH - TILESIZE * 11) / 2) - 2;
	tempRect.y = 0;
	tempRect.w = 4;
	tempRect.h = TILESIZE * 11;
	SDL_RenderFillRect(rendererRef, &tempRect);

	//
	// Draw Line Below Player Names
	SDL_SetRenderDrawColor(rendererRef, 0, 0, 0, 255);
	tempRect.x = TILESIZE * 11 + BorderSize;
	tempRect.y = 0 + BorderSize + TILESIZE;
	tempRect.w = WINDOWWIDTH - TILESIZE * 12;
	tempRect.h = 4;
	SDL_RenderFillRect(rendererRef, &tempRect);

	// 
	// Draw Player Names
	drawUIElement("White", TILESIZE * 11 + BorderSize + 10, 0 + BorderSize, TILESIZE * 4, TILESIZE);
	drawUIElement("Black", WINDOWWIDTH - ((WINDOWWIDTH - TILESIZE * 11) / 2) + 12, 0 + BorderSize, TILESIZE * 4, TILESIZE);

}

void Renderer::drawUIElement(SDL_Texture* _textureToRender, int _xPos, int _yPos, int _width, int _height) const {
	SDL_Rect tempRect = SDL_Rect();
	tempRect.x = _xPos;
	tempRect.y = _yPos;
	tempRect.w = _width;
	tempRect.h = _height;

	int x = SDL_RenderCopy(rendererRef,
		_textureToRender,
		NULL,
		&tempRect);
}
void Renderer::drawUIElement(const std::string& input, int _xPos, int _yPos, int _width, int _height) const {
	SDL_Rect tempRect = SDL_Rect();
	tempRect.x = _xPos;
	tempRect.y = _yPos;
	tempRect.w = _width;
	tempRect.h = _height;

	SDL_Surface* tempSurf = UILoaderRef->getTextSurface(input.c_str());
	SDL_Texture* Message = SDL_CreateTextureFromSurface(rendererRef, tempSurf);

	int x = SDL_RenderCopy(rendererRef,
		Message,
		NULL,
		&tempRect);
	SDL_FreeSurface(tempSurf);
	SDL_DestroyTexture(Message);
}

void Renderer::drawPlayerReadyUI(const Game& _gameRef) const {
	std::string message;

	int midPos = TILESIZE * 11 / 2;

	SDL_SetRenderDrawColor(rendererRef, 0, 0, 0, 255);
	SDL_Rect UIBox;
	int tempBoxSizeX = TILESIZE * 7 + 10;
	int tempBoxSizeY = TILESIZE * 2 + 10;
	UIBox.x = midPos - tempBoxSizeX / 2;
	UIBox.y = midPos - tempBoxSizeY / 2;
	UIBox.w = tempBoxSizeX;
	UIBox.h = tempBoxSizeY;
	SDL_RenderFillRect(rendererRef, &UIBox);

	SDL_SetRenderDrawColor(rendererRef, 255, 255, 255, 255);
	tempBoxSizeX = TILESIZE * 7;
	tempBoxSizeY = TILESIZE * 2;
	UIBox.x = midPos - tempBoxSizeX / 2;
	UIBox.y = midPos - tempBoxSizeY / 2;
	UIBox.w = tempBoxSizeX;
	UIBox.h = tempBoxSizeY;
	SDL_RenderFillRect(rendererRef, &UIBox);

	if (_gameRef.getActivePlayerID() == 0)
		message = "White ready?";
	else
		message = "Black ready?";

	drawUIElement(message, midPos - tempBoxSizeX / 2 + 7, midPos - tempBoxSizeY / 2 + 7, tempBoxSizeX - 15, tempBoxSizeY - 15);
	SDL_SetRenderDrawColor(rendererRef, 0, 0, 0, 255);
	tempBoxSizeX = TILESIZE * 7 + 10;
	tempBoxSizeY = TILESIZE * 2 + 10;
	UIBox.x = midPos - tempBoxSizeX / 2 + 10 * TILESIZE + 15;
	UIBox.y = midPos - tempBoxSizeY / 2;
	UIBox.w = tempBoxSizeX;
	UIBox.h = tempBoxSizeY;
	SDL_RenderFillRect(rendererRef, &UIBox);

	SDL_SetRenderDrawColor(rendererRef, 255, 255, 255, 255);
	tempBoxSizeX = TILESIZE * 7;
	tempBoxSizeY = TILESIZE * 2;
	UIBox.x = midPos - tempBoxSizeX / 2 + 10 * TILESIZE + 15;
	UIBox.y = midPos - tempBoxSizeY / 2;
	UIBox.w = tempBoxSizeX;
	UIBox.h = tempBoxSizeY;
	SDL_RenderFillRect(rendererRef, &UIBox);
	drawUIElement("press Enter", midPos - tempBoxSizeX / 2 + 7 + 10 * TILESIZE + 15, midPos - tempBoxSizeY / 2 + 7, tempBoxSizeX - 15, tempBoxSizeY - 15);
}

void Renderer::drawScore(const Game& _gameRef) const {
	std::stringstream ss;
	ss << "Score:  " << _gameRef.getPlayers()[0].getScore();
	std::string name = ss.str();
	drawUIElement(name, TILESIZE * 11 + BorderSize + 10, 0 + BorderSize + TILESIZE, TILESIZE * 4, TILESIZE);
	ss.str("");
	ss << "Score:  " << _gameRef.getPlayers()[1].getScore();
	name = ss.str();
	drawUIElement(name, WINDOWWIDTH - ((WINDOWWIDTH - TILESIZE * 11) / 2) + 12, 0 + BorderSize + TILESIZE, TILESIZE * 4, TILESIZE);


	for (int i = 0; i < _gameRef.getPlayers()[0].getScore(); i++) {
		drawUIElement(CoinImg, TILESIZE * 11 + BorderSize + 10 + i * 5, 0 + BorderSize + TILESIZE * 2, TILESIZE, TILESIZE);
	}
	for (int j = 0; j < _gameRef.getPlayers()[1].getScore(); j++) {
		drawUIElement(CoinImg, WINDOWWIDTH - ((WINDOWWIDTH - TILESIZE * 11) / 2) + 12 + j * 5, 0 + BorderSize + TILESIZE * 2, TILESIZE, TILESIZE);
	}
}

void Renderer::drawTurnIndicator(const Game& _gameRef) const {
	if (_gameRef.getActivePlayerID() == 0) {
		drawUIElement("YOUR TURN!", TILESIZE * 11 + BorderSize + 10, 0 + BorderSize + TILESIZE * 9, TILESIZE * 4, TILESIZE);
	}
	else {
		drawUIElement("YOUR TURN!", WINDOWWIDTH - ((WINDOWWIDTH - TILESIZE * 11) / 2) + 12, 0 + BorderSize + TILESIZE * 9, TILESIZE * 4, TILESIZE);
	}
}

void Renderer::drawWinScreen(const Game& _gameRef)const {
	std::string message;

	int midPos = TILESIZE * 11 / 2;

	SDL_SetRenderDrawColor(rendererRef, 0, 0, 0, 255);
	SDL_Rect UIBox;
	int tempBoxSizeX = TILESIZE * 7 + 10;
	int tempBoxSizeY = TILESIZE * 2 + 10;
	UIBox.x = midPos - tempBoxSizeX / 2;
	UIBox.y = midPos - tempBoxSizeY / 2;
	UIBox.w = tempBoxSizeX;
	UIBox.h = tempBoxSizeY;
	SDL_RenderFillRect(rendererRef, &UIBox);

	SDL_SetRenderDrawColor(rendererRef, 255, 255, 255, 255);
	tempBoxSizeX = TILESIZE * 7;
	tempBoxSizeY = TILESIZE * 2;
	UIBox.x = midPos - tempBoxSizeX / 2;
	UIBox.y = midPos - tempBoxSizeY / 2;
	UIBox.w = tempBoxSizeX;
	UIBox.h = tempBoxSizeY;
	SDL_RenderFillRect(rendererRef, &UIBox);

	if (_gameRef.getActivePlayerID() == 0)
		message = "WHITE WINS!";
	else
		message = "BLACK WINS!";

	drawUIElement(message, midPos - tempBoxSizeX / 2 + 7, midPos - tempBoxSizeY / 2 + 7, tempBoxSizeX - 15, tempBoxSizeY - 15);


	drawUIElement(message, midPos - tempBoxSizeX / 2 + 7, midPos - tempBoxSizeY / 2 + 7, tempBoxSizeX - 15, tempBoxSizeY - 15);
	SDL_SetRenderDrawColor(rendererRef, 0, 0, 0, 255);
	tempBoxSizeX = TILESIZE * 7 + 10;
	tempBoxSizeY = TILESIZE * 2 + 10;
	UIBox.x = midPos - tempBoxSizeX / 2 + 10 * TILESIZE + 15;
	UIBox.y = midPos - tempBoxSizeY / 2;
	UIBox.w = tempBoxSizeX;
	UIBox.h = tempBoxSizeY;
	SDL_RenderFillRect(rendererRef, &UIBox);

	SDL_SetRenderDrawColor(rendererRef, 255, 255, 255, 255);
	tempBoxSizeX = TILESIZE * 7;
	tempBoxSizeY = TILESIZE * 2;
	UIBox.x = midPos - tempBoxSizeX / 2 + 10 * TILESIZE + 15;
	UIBox.y = midPos - tempBoxSizeY / 2;
	UIBox.w = tempBoxSizeX;
	UIBox.h = tempBoxSizeY;
	SDL_RenderFillRect(rendererRef, &UIBox);
	drawUIElement("Enter to restart", midPos - tempBoxSizeX / 2 + 7 + 10 * TILESIZE + 15, midPos - tempBoxSizeY / 2 + 7, tempBoxSizeX - 15, tempBoxSizeY - 15);
}

void Renderer::renderPresent() {
	SDL_RenderPresent(rendererRef);
}

void Renderer::drawTargetUI(int _xPos, int _yPos, int _width, int _height) const {
	drawUIElement(TargetImg, _xPos, _yPos,  _width, _height);
}
