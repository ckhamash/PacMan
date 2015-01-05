#pragma once
#include "SFML\Graphics.hpp"
#include "Ghost.h"

class Game;

enum class EnemyState { Chase, Scatter, Frightened };

class EnemyManager {
private:
	Game *pGame;

	EnemyState enemyState = EnemyState::Scatter;

	sf::Texture blinkyTexture, pinkyTexture, inkyTexture, clydeTexture, frightenedTexture;
	std::array<Ghost, 4> ghosts;
	int ghostValue;
	int ghostMultiplier = 0;
	float ghostSpeed, maxGhostSpeed;
	float ghostFrightenTime, ghostChaseTime, ghostScatterTime, maxStateTime;
	std::array<float, 4> ghostImprisonTime;
	float ghostImprisonClock = 0, stateClock = 0, ghostFrightenClock = 0;

public:
	EnemyManager();
	EnemyManager(Game *game);
	~EnemyManager();

	sf::Vector2f getGhostLocation(int ghostIndex);

	Direction getClosestDirection(Intersection& current, Intersection& target, Direction direction);
	Direction getClosestDirection(Intersection& current, sf::Vector2f target, Direction direction);
	Direction getRandomDirection(Intersection& current, Direction direction);
	Direction oppositeDirection(Direction direction);
	Direction getNextDirection(int ghostIndex);

	bool isGhostFrightened(int ghostIndex);
	bool isGhostDead(int ghostIndex);

	void spawnGhosts(std::array<Intersection, 4> ghostIntersections, std::array<sf::Vector2f, 4> ghostPositions);
	void moveGhost(int ghostIndex, float secondsSinceLastFrame);
	void killGhost(int ghostIndex);
	void frightenGhosts();
	void reverseGhosts();
	void updateFrighten(float secondsSinceLastFrame);
	void updateImprison(float secondsSinceLastFrame);
	void updateState(float secondsSinceLastFrame);
	void checkState();
	void restart();

	void update(float secondsSinceLastFrame);
	void draw();
};