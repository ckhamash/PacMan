#include "Ghost.h"

Ghost::Ghost() { }

Ghost::Ghost(sf::Texture &texture) {
	sprite.setTexture(texture);
}

Ghost::~Ghost() { }

Direction& Ghost::getDirection() { return currentDirection; }

sf::Sprite& Ghost::getSprite() { return sprite;}

Intersection& Ghost::getTarget() { return targetIntersection; }

bool Ghost::isDead() { return dead; }
bool Ghost::isFrightened() { return frightened; }
void Ghost::frighten() { frightened = true; }
void Ghost::kill() { dead = true; }
void Ghost::revive() { dead = false; frightened = false; }
void Ghost::spawn(Intersection &startIntersection, sf::Vector2f startPosition) {
	targetIntersection = startIntersection;
	sprite.setPosition(startPosition);
	frightened = false;
	dead = false;
}