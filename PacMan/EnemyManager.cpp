#include "EnemyManager.h"
#include "Game.h"

EnemyManager::EnemyManager() { }

EnemyManager::EnemyManager(Game *game) 
	: pGame(game)
	, ghostValue(200)
	, ghostSpeed(60 * pGame->getScale())
	, maxGhostSpeed(66 * pGame->getScale())
	, ghostFrightenTime(10)
	, ghostChaseTime(20)
	, ghostScatterTime(5)
	, maxStateTime(80) { 
	ghostImprisonTime = { { 0, 2, 4, 8} };
	blinkyTexture.loadFromFile("Images/Blinky.png");
	pinkyTexture.loadFromFile("Images/Pinky.png");
	inkyTexture.loadFromFile("Images/Inky.png");
	clydeTexture.loadFromFile("Images/Clyde.png");
	frightenedTexture.loadFromFile("Images/Frightened.png");
	ghosts[0] = Ghost(blinkyTexture);
	ghosts[1] = Ghost(pinkyTexture);
	ghosts[2] = Ghost(inkyTexture);
	ghosts[3] = Ghost(clydeTexture);

	for (int i = 0; i < ghosts.size(); i++) {
		ghosts[i].getSprite().setScale(pGame->getScale(), pGame->getScale());
		ghosts[i].getSprite().setOrigin(ghosts[i].getSprite().getLocalBounds().width / 2, ghosts[i].getSprite().getLocalBounds().height / 2);
	}
}

EnemyManager::~EnemyManager() { }

sf::Vector2f EnemyManager::getGhostLocation(int ghostIndex) { return ghosts[ghostIndex].getSprite().getPosition(); }

Direction EnemyManager::getClosestDirection(Intersection& current, Intersection& target, Direction direction) { 
	return getClosestDirection(current, target.getLocation(), direction);
}

Direction EnemyManager::getClosestDirection(Intersection& current, sf::Vector2f target, Direction direction) {
	Direction closest = Direction::Up;
	float shortest = -1;
	for (int i = 0; i < 4; i++) {
		if ((Direction)i != oppositeDirection(direction)) {
			Intersection* neighbor = &current.getNeighbor((Direction)i);
			if (&current != neighbor) {
				sf::Vector2f diff = neighbor->getLocation() - target;
				float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
				if (shortest < 0) {
					shortest = distance;
					closest = (Direction)i;
				}
				else if (distance < shortest) {
					shortest = distance;
					closest = (Direction)i;
				}
			}
		}
	}

	return closest;
}

Direction EnemyManager::getRandomDirection(Intersection& current, Direction direction) {
	std::vector<Direction> possibleDirections;
	for (int i = 0; i < 4; i++)
	if (&current != &current.getNeighbor((Direction)i) && (Direction)i != oppositeDirection(direction))
		possibleDirections.push_back((Direction)i);

	// randomly choose a direction
	int index = pGame->getRandom() * possibleDirections.size();
	return possibleDirections[index];
}

Direction EnemyManager::oppositeDirection(Direction direction) {
	int i = (int)direction;
	i += 2;
	if (i > 3)
		i -= 4;
	return (Direction)i;
}

Direction EnemyManager::getNextDirection(int ghostIndex) {
	switch (enemyState) {
	case EnemyState::Chase:
		return getClosestDirection(ghosts[ghostIndex].getTarget(), pGame->getPlayerManager().getTarget(), ghosts[ghostIndex].getDirection());
		break;
	case EnemyState::Scatter:
		return getClosestDirection(ghosts[ghostIndex].getTarget(), pGame->getLevelManager().getGhostCorners()[ghostIndex], ghosts[ghostIndex].getDirection());
		break;
	case EnemyState::Frightened:
		return getRandomDirection(ghosts[ghostIndex].getTarget(), ghosts[ghostIndex].getDirection());
		break;
	}
}

bool EnemyManager::isGhostFrightened(int ghostIndex) { return ghosts[ghostIndex].isFrightened(); }

bool EnemyManager::isGhostDead(int ghostIndex) { return ghosts[ghostIndex].isDead(); }

void EnemyManager::spawnGhosts(std::array<Intersection, 4> ghostIntersections, std::array<sf::Vector2f, 4> ghostPositions) {
	for (int i = 0; i < ghosts.size(); i++)
		ghosts[i].spawn(ghostIntersections[i], ghostPositions[i]);
	ghostMultiplier = 0;
	ghostImprisonClock = 0;
}

void EnemyManager::moveGhost(int ghostIndex, float secondsSinceLastFrame) {
	sf::Vector2f direction = ghosts[ghostIndex].getTarget().getLocation() - ghosts[ghostIndex].getSprite().getPosition();
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	float speed = maxGhostSpeed - ((maxGhostSpeed - ghostSpeed) * std::pow(0.95f, pGame->getLevelManager().getLevelNumber()));
	sf::FloatRect windowRect(0, 0, pGame->getWindow().getSize().x, pGame->getWindow().getSize().y);
	if (ghosts[ghostIndex].isFrightened() || ghosts[ghostIndex].isDead() || !windowRect.contains(ghosts[ghostIndex].getTarget().getLocation()))
		speed /= 2;
	sf::Vector2f moveVector = direction / distance * speed;

	sf::Vector2f prevPosition = ghosts[ghostIndex].getSprite().getPosition();
	sf::Vector2f currPosition;

	ghosts[ghostIndex].getSprite().move(moveVector * secondsSinceLastFrame);

	prevPosition = prevPosition - ghosts[ghostIndex].getTarget().getLocation();
	currPosition = ghosts[ghostIndex].getSprite().getPosition() - ghosts[ghostIndex].getTarget().getLocation();

	// check if ghost has passed the target intersection
	if ((!(std::signbit(prevPosition.x) == std::signbit(currPosition.x)) || currPosition.x == 0) &&
		(!(std::signbit(prevPosition.y) == std::signbit(currPosition.y)) || currPosition.y == 0)) {
		if (ghosts[ghostIndex].isDead()) {
			ghosts[ghostIndex].revive();
		}
		ghosts[ghostIndex].getSprite().setPosition(ghosts[ghostIndex].getTarget().getLocation());

		// determine new intersection for ghost
		Direction inputDirection = getNextDirection(ghostIndex);

		if (&ghosts[ghostIndex].getTarget() != &ghosts[ghostIndex].getTarget().getNeighbor(inputDirection))
			ghosts[ghostIndex].getDirection() = inputDirection;

		// warp intersection
		if (windowRect.contains(ghosts[ghostIndex].getTarget().getLocation()) || windowRect.contains(ghosts[ghostIndex].getTarget().getNeighbor(ghosts[ghostIndex].getDirection()).getLocation())) {
			ghosts[ghostIndex].getTarget() = ghosts[ghostIndex].getTarget().getNeighbor(ghosts[ghostIndex].getDirection());
		}
		else {
			ghosts[ghostIndex].getTarget() = ghosts[ghostIndex].getTarget().getNeighbor(ghosts[ghostIndex].getDirection());
			ghosts[ghostIndex].getSprite().setPosition(ghosts[ghostIndex].getTarget().getLocation());
			ghosts[ghostIndex].getTarget() = ghosts[ghostIndex].getTarget().getNeighbor(ghosts[ghostIndex].getDirection());
		}
	}
}

void EnemyManager::killGhost(int ghostIndex) { 
	ghosts[ghostIndex].kill();
	if (ghostIndex == 0)
		ghosts[ghostIndex].getTarget() = pGame->getLevelManager().getLevel().getGhostIntersections()[ghostIndex + 1];
	else
		ghosts[ghostIndex].getTarget() = pGame->getLevelManager().getLevel().getGhostIntersections()[ghostIndex];
	pGame->addScore(ghostValue * std::pow(2, ghostMultiplier));
	ghostMultiplier++;
}

void EnemyManager::frightenGhosts() {
	ghostMultiplier = 0;
	ghostFrightenClock = 0;
	for (int i = 0; i < ghosts.size(); i++)
		ghosts[i].frighten();
	reverseGhosts();
	enemyState = EnemyState::Frightened;
}

void EnemyManager::reverseGhosts() {
	for (int i = 0; i < ghosts.size(); i++)
		if (!ghosts[i].isDead()) {
			Direction opposite = oppositeDirection(ghosts[i].getDirection());
			ghosts[i].getTarget() = ghosts[i].getTarget().getNeighbor(opposite);
			ghosts[i].getDirection() = opposite;
		}
}

void EnemyManager::updateFrighten(float secondsSinceLastFrame) {
	ghostFrightenClock += secondsSinceLastFrame;
	float time = ghostFrightenTime * std::pow(0.95f, pGame->getLevelManager().getLevelNumber());
	if (ghostFrightenClock > time) {
		checkState();
		for (int i = 0; i < ghosts.size(); i++)
			if (!ghosts[i].isDead())
				ghosts[i].revive();
	}
}

void EnemyManager::updateImprison(float secondsSinceLastFrame) {
	ghostImprisonClock += secondsSinceLastFrame;
}

void EnemyManager::updateState(float secondsSinceLastFrame) {
	if (enemyState != EnemyState::Frightened) {
		stateClock += secondsSinceLastFrame;
		checkState();
	}
}

void EnemyManager::checkState() {
	EnemyState newState = EnemyState::Chase;
	if (stateClock > maxStateTime)
		newState = EnemyState::Chase;
	else {
		float clock = stateClock;
		while (clock > (ghostScatterTime + ghostChaseTime))
			clock -= (ghostScatterTime + ghostChaseTime);
		if (clock < ghostScatterTime)
			newState = EnemyState::Scatter;
		else
			newState = EnemyState::Chase;
	}
	if (enemyState != newState && enemyState != EnemyState::Frightened)
		reverseGhosts();
	enemyState = newState;
}

void EnemyManager::restart() {
	stateClock = 0;
}

void EnemyManager::update(float secondsSinceLastFrame) {
	updateImprison(secondsSinceLastFrame);
	updateFrighten(secondsSinceLastFrame);
	updateState(secondsSinceLastFrame);
	for (int i = 0; i < ghosts.size(); i++)
		if (ghostImprisonTime[i] < ghostImprisonClock) {
			moveGhost(i, secondsSinceLastFrame);
		}
}

void EnemyManager::draw() {
	for (int i = 0; i < ghosts.size(); i++)
		if (ghosts[i].isDead()) {

		}
		else if (ghosts[i].isFrightened()) {
			sf::Sprite fright(ghosts[i].getSprite());
			fright.setTexture(frightenedTexture);
			//sf::Sprite fright(frightenedTexture);
			//fright.setPosition(ghosts[i].getSprite().getPosition());
			pGame->getWindow().draw(fright);
		}
		else {
			pGame->getWindow().draw(ghosts[i].getSprite());
		}
}