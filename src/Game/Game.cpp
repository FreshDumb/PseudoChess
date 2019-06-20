#include "Game.h"


Game::Game()
{
}

Game::~Game()
{
}

// Initialize the Game
void Game::initGame(SDL_Renderer* _rendererRef, SDL_Window* _windowRef) {
	Round = 0;

	// Create Players
	Players = new Player[MAXPLAYERS];
	tileManager.initializeTiles();
	for (int i = 0; i < MAXPLAYERS; i++) {
		Players[i] = Player(i);
	}

	// Feed seed for srand
	srand(time(NULL));

	// Create InputManager and UILoader
	InputManagerRef = new InputManager();
	UILoader* tempUILoader = new UILoader();

	// Load font and textures
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	tempUILoader->loadFont("dat/Sans.ttf");
	tempUILoader->loadTexturesToSurface();

	// Create Renderer
	rendererRef = new Renderer(_rendererRef, _windowRef, tempUILoader);
	// Create SDL_Textures from SDL_Surfaces
	rendererRef->createTextures();

	// Randomize starting Player
	currentActivePlayerID = rand() % MAXPLAYERS;
	
	// Fill PlayerUnits Array with dummy Units with a random UNIT_TYPE
	for (int j = 0; j < MAXPLAYERS; j++) {
		for (int i = 0; i < UNITSPERPLAYER; i++) {
			Players[j].PlayerUnits.push_back(Unit(UNIT_TYPE(rand() % 5), j));
		}
	}

	// Create Pickup and place outside of board
	gamePickup = new Pickup();
	gamePickup->setPosition(-1, -1);
}

// Lets player choose a starting position for their units
void Game::doZeroRound() {
	Vector2 temp = InputManagerRef->getMouseAxis();

	temp = tileManager.getClosestTile(temp.xPos, temp.yPos);
	rendererRef->drawTargetUI((TILESIZE / 2) + temp.xPos * TILESIZE + 5, (TILESIZE / 2) + temp.yPos * TILESIZE + 5, 20, 20);

	// On LeftClick try to initialize the Units at the given temp position
	// fails when out of bounds or when the corners of the board since there is not enough space
	if (InputManagerRef->getMouseButtonDown()) {
		getCurrentPlayer().setStartingPos(temp);
		getCurrentPlayer().initUnits(tileManager);
		getCurrentPlayer().incrementPlayerRound();
		currentActivePlayerID = (currentActivePlayerID + 1) % MAXPLAYERS;

		// switches current player
		std::cout << "Player ID:" << currentActivePlayerID << "\n";
		playerConfirmed = false;
	}
}

void Game::waitForPlayerConfirm() {
	// Wait for player to press enter
	if (InputManagerRef->enter == true) {
		std::cout << "Player Confirmed" << "\n";
		playerConfirmed = true;

		// needs to be set to false after confirmation due to complication with the SDL eventsystem
		InputManagerRef->enter = false;
	}
	else {
		playerConfirmed = false;
	}
}

bool Game::isNewRound() const {
	// When both players have performed a turn 
	// thus both having a PlayerRound differing from round returns true
	bool temp = false;
	for (int i = 0; i < MAXPLAYERS; i++) {
		if (Players[i].getPlayerRound() != Round)
			temp = true;
		else
			return false;
	}
	return temp;
}

void Game::spawnPickup() {
	Vector2 newPos;
	std::vector<Vector2> possiblePositions;
	for (int x = 0; x <= BOARDSIZE - 1; x++) {
		for (int y = 0; y <= BOARDSIZE - 1; y++) {
			if (tileManager.isFieldOccupied(x, y) == false) {
				possiblePositions.push_back(Vector2(x, y));
			}
		}
	}
	if (possiblePositions.empty()) {
		std::cout << "Can't spawn Item";
		abort();
	}
	else {
		int resultIndex = rand() % possiblePositions.size();
		newPos = possiblePositions.at(resultIndex);
	}

	gamePickup->spawnGamePiece(newPos, tileManager);
	hasPickup = true;
}

void Game::doTurn() {
	Vector2 temp = InputManagerRef->getMouseAxis();
	temp = tileManager.getClosestTile(temp.xPos, temp.yPos);

	rendererRef->drawTargetUI((TILESIZE / 2) + temp.xPos * TILESIZE + 5, (TILESIZE / 2) + temp.yPos * TILESIZE + 5, 20, 20);

	// When the Player has currently selected a Unit
	// draw the required Movement Helper Overlay
	if (getCurrentPlayer().getSelectedUnit() != nullptr) {
		rendererRef->drawUnitMovement(*getCurrentPlayer().getSelectedUnit(), tileManager);

		// When the LeftButton was clicked check if the clicked Tile is occupied
		if (InputManagerRef->getMouseButtonDown() &&
			!tileManager.isFieldOccupied(temp.xPos, temp.yPos)) {
			// Check if the clicked Tile is a possible destination for the Selected Unit
			if (getCurrentPlayer().getSelectedUnit()->checkMovement(temp, tileManager)) {
				// Check if the target Tile contains the Pickup
				if (temp == gamePickup->getPosition()) {
					// When the player reaches the SCORETOWIN by picking up the Pickup
					// end the game
					if (gamePickup->doPickUp(getCurrentPlayer(), tileManager) >= SCORETOWIN) {
						isGameOver = true;
						InputManagerRef->enter = false;
					}
					hasPickup = false;
				}
				// Move the Unit
				getCurrentPlayer().getSelectedUnit()->moveUnit(temp, tileManager);
				// Deselect the Unit after moving it
				getCurrentPlayer().getSelectedUnit()->deSelectUnit();
				getCurrentPlayer().setSelectedUnit(nullptr);
				playerConfirmed = false;

				// If the game isn't over the turn ends and players are switched
				if (isGameOver == false) {
					getCurrentPlayer().incrementPlayerRound();
					currentActivePlayerID = (currentActivePlayerID + 1) % MAXPLAYERS;
				}
			}
		}
	}

	// Checks whether a Tile with a Unit on it was clicked
	if (InputManagerRef->getMouseButtonDown() &&
		tileManager.isFieldOccupied(temp.xPos, temp.yPos))	{
		Unit* tempUnit = tileManager.getTile(temp.xPos, temp.yPos)->getUnitOnTile();
		// Checks whether the Unit is of the Player whose turn it currently is
		if (tempUnit->getControllingPlayerID() == currentActivePlayerID) {
			// Deselects the old Unit when Player already had selected a Unit
			if (getCurrentPlayer().getSelectedUnit() != nullptr) {
				getCurrentPlayer().getSelectedUnit()->deSelectUnit();
			}

			// Selects the Unit
			tempUnit->selectUnit();
			getCurrentPlayer().setSelectedUnit(tempUnit);
		}
	}


}

void Game::update() {
	InputManagerRef->pollEvents();
	rendererRef->drawEverything(*this, tileManager);

	if (isNewRound())
		Round++;
	if (playerConfirmed == true) {
		if (isGameOver) {
			restartGame();
		}
		if (Round == 0) {
			doZeroRound();
		}
		else if (Round > 0) {
			// Spawns a Pickup when none on the board
			if (!hasPickup) {
				spawnPickup();
			}
			doTurn();
		}

	}
	else {
		waitForPlayerConfirm();
	}

	// Call the SDL_Renderer to refresh the Rendering
	rendererRef->renderPresent();
}

void Game::restartGame() {
	Round = 0;
	for (int i = 0; i < MAXPLAYERS; i++) {
		Players[i].resetPlayer(tileManager);
	}
	currentActivePlayerID = rand() % MAXPLAYERS;
	for (int j = 0; j < MAXPLAYERS; j++) {
		for (int i = 0; i < UNITSPERPLAYER; i++) {
			Players[j].PlayerUnits.push_back(Unit(UNIT_TYPE(rand() % 5), j));
		}
	}

	playerConfirmed = false;
	InputManagerRef->enter = false;
	isGameOver = false;
}