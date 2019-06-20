#include "Board/TileManager.h"
#include "UI/UILoader.h"

#ifndef RENDERER_H
#define RENDERER_H
#define WINDOWWIDTH 640
#define WINDOWHEIGHT TILESIZE*11
class Game;
class Unit;
class Renderer {
private:
	static const int BorderSize = TILESIZE / 2;
	SDL_Renderer* rendererRef;
	SDL_Window* windowRef;	
	UILoader* UILoaderRef;

	//
	// Textures
	SDL_Texture* PawnImg;
	SDL_Texture* RogueImg;
	SDL_Texture* RookImg;
	SDL_Texture* KingImg;
	SDL_Texture* BishopImg;
	SDL_Texture* CoinImg;
	SDL_Texture* TargetImg;
public:

	Renderer();
	Renderer(SDL_Renderer* _rendererRef, SDL_Window* _windowRef, UILoader* _UILoaderRef);
	~Renderer();



	void renderPresent();
	void drawTiles(const TileManager& _tileManager)const;
	void drawBorder(const Color _color)const;
	void drawSingleTile(const Color _color, const Vector2 _position)const;
	void drawBoard(const TileManager& _tileManager)const;
	void drawUnits(const Game& _gameRef)const;
	void drawPickup(const Game& _gameRef) const;

	void drawSingleUnit(const Unit& _unitToDraw) const;
	void drawUnitMovement(const Unit& _Unit, TileManager& _tileManager) const;
	bool drawMovementUITile(Color _color, const int _xPos, const int _yPos, const Unit& _Unit, TileManager& _tileManager) const;

	void drawPawnMovementTiles	(const Unit& _unit, TileManager& _tileManager)const;
	void drawKingMovementTiles	(const Unit& _unit, TileManager& _tileManager)const;
	void drawRogueMovementTiles	(const Unit& _unit, TileManager& _tileManager)const;
	void drawRookMovementTiles	(const Unit& _unit, TileManager& _tileManager)const;
	void drawBishopMovementTiles(const Unit& _unit, TileManager& _tileManager)const;

	void drawEverything(const Game& _gameRef, TileManager& _tileManager)const;

	void createTextures();

	void drawUIArea()const;
	void drawPlayerReadyUI(const Game& _gameRef) const;
	void drawUIElement(SDL_Texture* _textureToRender, int _xPos, int _yPos, int _width, int _height) const;
	void drawUIElement(const std::string& input, int _xPos, int _yPos, int _width, int _height) const;
	void drawTargetUI(int _xPos, int _yPos, int _width, int _height) const;
	void drawScore(const Game& _gameRef) const;
	void drawTurnIndicator(const Game& _gameRef)const;
	void drawWinScreen(const Game& _gameRef)const;

}; 
#endif