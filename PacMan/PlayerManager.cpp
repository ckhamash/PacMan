#include "PlayerManager.h"
#include "Game.h"

PlayerManager::PlayerManager() { }

PlayerManager::PlayerManager(Game *game) 
	: pGame(game) {
	playerTexture.loadFromFile("Images/Player.png");
	player.setTexture(playerTexture);
	player.setScale(pGame->getScale(), pGame->getScale());
	player.setPosition(0, 0);
	player.setOrigin(player.getLocalBounds().width / 2, player.getLocalBounds().height / 2);
	playerSpeed = 64 * pGame->getScale();
	playerRadius = 4 * pGame->getScale();
	respawnTime = 2;
	respawnClock = respawnTime;
}

PlayerManager::~PlayerManager() { }

float PlayerManager::getRadius() { return playerRadius; }

sf::Vector2f PlayerManager::getLocation() { return player.getPosition(); }

Intersection& PlayerManager::getTarget() { return targetIntersection; }

bool PlayerManager::isAlive() { return lives > 0; }

bool PlayerManager::isRespawning() { return respawnClock < respawnTime; }

void PlayerManager::lifeUp() { lives++; }

void PlayerManager::updateDirection() {
	if (pGame->getInputManager().isCommandActive(Command::TurnUp)) 
		inputDirection = Direction::Up;
	else if (pGame->getInputManager().isCommandActive(Command::TurnRight)) 
		inputDirection = Direction::Right;
	else if (pGame->getInputManager().isCommandActive(Command::TurnDown)) 
		inputDirection = Direction::Down;
	else if (pGame->getInputManager().isCommandActive(Command::TurnLeft)) 
		inputDirection = Direction::Left;
}

void PlayerManager::checkReverse() {
	if ((inputDirection == Direction::Up && currentDirection == Direction::Down) ||
		(inputDirection == Direction::Down && currentDirection == Direction::Up) ||
		(inputDirection == Direction::Right && currentDirection == Direction::Left) ||
		(inputDirection == Direction::Left && currentDirection == Direction::Right)) {
		currentDirection = inputDirection;
		targetIntersection = targetIntersection.getNeighbor(currentDirection);
	}
}

void PlayerManager::move(float secondsSinceLastFrame) { 
	sf::Vector2f direction = targetIntersection.getLocation() - player.getPosition();
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	sf::Vector2f moveVector = direction / distance * playerSpeed;

	sf::Vector2f prevPosition = player.getPosition();
	sf::Vector2f currPosition;

	player.move(moveVector * secondsSinceLastFrame);

	prevPosition = prevPosition - targetIntersection.getLocation();
	currPosition = player.getPosition() - targetIntersection.getLocation();

	// check if player has passed the target intersection
	if ((!(std::signbit(prevPosition.x) == std::signbit(currPosition.x)) || currPosition.x == 0) &&
		(!(std::signbit(prevPosition.y) == std::signbit(currPosition.y)) || currPosition.y == 0)) {
		player.setPosition(targetIntersection.getLocation());

		// get player input to determine new intersection
		if (&targetIntersection != &targetIntersection.getNeighbor(inputDirection))
			currentDirection = inputDirection;

		sf::FloatRect windowRect(0, 0, pGame->getWindow().getSize().x, pGame->getWindow().getSize().y);
		if (windowRect.contains(targetIntersection.getLocation()) || windowRect.contains(targetIntersection.getNeighbor(currentDirection).getLocation())) {
			targetIntersection = targetIntersection.getNeighbor(currentDirection);
		}
		else {
			targetIntersection = targetIntersection.getNeighbor(currentDirection);
			player.setPosition(targetIntersection.getLocation());
			targetIntersection = targetIntersection.getNeighbor(currentDirection);
		}
	}
	// reset input
	inputDirection = currentDirection;
	// set player orientation
	/*if (moveVector.y < 0)
		player.setRotation(std::atan(-moveVector.x / moveVector.y) * 180.0f / PI));
	else
		player.setRotation(std::atan(-moveVector.x / moveVector.y) * 180.0f / PI + 180.0f));*/
}

void PlayerManager::spawn(Intersection &startIntersection, sf::Vector2f startPosition) { 
	player.setPosition(startPosition);
	targetIntersection = startIntersection;
	currentDirection = Direction::Left;
}

void PlayerManager::kill() { 
	lives--; 
	respawnClock = 0;
}

void PlayerManager::restart() {
	lives = 3;
	respawnClock = respawnTime;
}

void PlayerManager::update(float secondsSinceLastFrame) {
	if (respawnClock < respawnTime && respawnClock + secondsSinceLastFrame >= respawnTime)
		pGame->getLevelManager().respawn();
	respawnClock += secondsSinceLastFrame;
	if (!isRespawning()) {
		updateDirection();
		checkReverse();
		move(secondsSinceLastFrame);
	}
}

void PlayerManager::draw() {
	if (!isRespawning() || std::cos(respawnClock * PI * 4) > 0) {
		pGame->getWindow().draw(player);
		sf::Sprite life(playerTexture);
		life.setScale(pGame->getScale(), pGame->getScale());
		life.setPosition(life.getGlobalBounds().width, pGame->getWindow().getSize().y - life.getGlobalBounds().height);
		for (int i = 0; i < lives; i++) {
			pGame->getWindow().draw(life);
			life.move(life.getGlobalBounds().width * 1.5, 0);
		}
	}
}