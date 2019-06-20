#include "Player.h"
#include <time.h>
#include "Unit.h"
#include "Pickup.h"
#include "../InputManager.h"
#include "../UI/UILoader.h"
#include "../Renderer.h"
#include "../Defines.h"

#ifndef GAME_H
#define GAME_H
class Game
{
private:
	int Round = 0;
	Player* Players;
	bool isGameOver = false;
	bool playerConfirmed = false;
	int currentActivePlayerID;
	Renderer* rendererRef;
	bool hasPickup = false;
	TileManager tileManager;
	Pickup* gamePickup;
	InputManager* InputManagerRef;
public:
	Game();
	~Game();

	inline bool isTheGameOver() const { return isGameOver; }
	inline int getActivePlayerID()const { return currentActivePlayerID; }
	inline bool hasPlayerConfirmed()const { return playerConfirmed; }
	inline Player* getPlayers() const { return Players; }
	inline Vector2 getPickupPos() const { return gamePickup->getPosition(); }
	inline bool gameHasPickup() const { return hasPickup; }
	inline bool gameQuit() { return InputManagerRef->hasQuit(); }
	inline Player& getCurrentPlayer() const { return Players[currentActivePlayerID]; }

	void initGame(SDL_Renderer* _rendererRef, SDL_Window* _windowRef);
	void restartGame();
	void doZeroRound();
	void update();
	void waitForPlayerConfirm();
	void doTurn();

	void spawnPickup();

	bool isNewRound() const;
};
#endif