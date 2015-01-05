#pragma once
#include "SFML\Graphics.hpp"
#include "Level.h"
#include "Pellet.h"

class Game;

class LevelManager {
private:
	Game *pGame;

	sf::Texture laneTexture, pelletTexture, energizerTexture;
	std::vector<Level> levels;
	std::vector<Pellet> pellets;
	int currentLevel = 0;
	int tileWidth, intersectionWidth;

	float startTime, startClock = 0;
	float endTime, endClock = 0;
public:
	LevelManager();
	LevelManager(Game *game);
	~LevelManager();

	int getLevelNumber();
	std::array<sf::Vector2f, 4> getGhostCorners();
	int getPelletCount();
	sf::Vector2f getPelletLocation(int pelletIndex);
	void eatPellet(int pelletIndex);

	Level& getLevel();
	
	bool isStarting();
	bool isEnding();

	void restart();
	void respawn();

	void initLevels();
	void loadLevel();
	void levelUp();
	void spawnPellets();
	
	void update(float secondsSinceLastFrame);
	void drawLanes();
	void drawPellets();
	void draw();
};

