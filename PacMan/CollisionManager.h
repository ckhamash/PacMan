#pragma once
#include "SFML\Graphics.hpp"

class Game;

class CollisionManager {
private:
	Game* pGame;
public:
	CollisionManager();
	CollisionManager(Game* game);
	~CollisionManager();

	void checkGhosts();
	void checkPellets();

	void update();
};

