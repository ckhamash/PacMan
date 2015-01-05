#pragma once
#include "SFML\Graphics.hpp"
#include "Intersection.h"

class Game;

class PlayerManager {
private:
	Game* pGame;

	sf::Sprite player;
	sf::Texture playerTexture;
	Intersection targetIntersection;
	Direction inputDirection;
	Direction currentDirection = Direction::Left;
	float playerSpeed, playerRadius;
	int lives = 3;

	float respawnTime, respawnClock;
public:
	PlayerManager();
	PlayerManager(Game *game);
	~PlayerManager();

	float getRadius();
	sf::Vector2f getLocation();
	Intersection& getTarget();
	bool isAlive();
	bool isRespawning();

	void lifeUp();
	void updateDirection();
	void checkReverse();
	void move(float secondsSinceLastFrame);
	void spawn(Intersection &startIntersection, sf::Vector2f startPosition);
	void kill();
	void restart();

	void update(float secondsSinceLastFrame);
	void draw();
};

