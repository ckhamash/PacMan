#pragma once
#include "SFML\Graphics.hpp"
#include "Intersection.h"

class Ghost {
private:
	sf::Sprite sprite;
	Intersection targetIntersection;
	bool dead = false;
	bool frightened = false;
	Direction currentDirection = Direction::Up;
public:
	Ghost();

	Ghost(sf::Texture &texture);

	~Ghost();

	Direction& getDirection();
	sf::Sprite& getSprite();
	Intersection& getTarget();
	bool isDead();
	bool isFrightened();

	void frighten();
	void kill();
	void revive();
	void spawn(Intersection &startIntersection, sf::Vector2f startPosition);
};