#include "CollisionManager.h"
#include "Game.h"

CollisionManager::CollisionManager() { }

CollisionManager::CollisionManager(Game* game) 
	: pGame(game) { }

CollisionManager::~CollisionManager() { }

void CollisionManager::checkGhosts() {
	for (int i = 0; i < 4; i++) {
		sf::Vector2f diff = pGame->getEnemyManager().getGhostLocation(i) - pGame->getPlayerManager().getLocation();
		float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
		if (!pGame->getEnemyManager().isGhostDead(i) && distance < pGame->getPlayerManager().getRadius()) {
			if (pGame->getEnemyManager().isGhostFrightened(i)) {
				pGame->getEnemyManager().killGhost(i);
			}
			else {
				pGame->getPlayerManager().kill();
			}
		}
	}
}

void CollisionManager::checkPellets() {
	for (int i = 0; i < pGame->getLevelManager().getPelletCount(); i++) {
		sf::Vector2f diff = pGame->getLevelManager().getPelletLocation(i) - pGame->getPlayerManager().getLocation();
		float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
		if (distance < pGame->getPlayerManager().getRadius()) { 
			pGame->getLevelManager().eatPellet(i);
		}
	}
}

void CollisionManager::update() {
	checkGhosts();
	checkPellets();
}