#pragma once
#include "SFML\Graphics.hpp"
#include <array>

enum class Direction { Up, Right, Down, Left };

class Intersection {
private:
	sf::Vector2f location;
	std::array<Intersection*, 4> pNeighbors;

	bool hasPellets = true;
public:
	Intersection();
	Intersection(sf::Vector2f location);
	~Intersection();

	void setNeighbor(Intersection &intersection, Direction direction);
	Intersection& getNeighbor(Direction direction);
	sf::Vector2f getLocation();

	bool spawnsPellets();
	void noPellets();
};