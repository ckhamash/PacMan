#include "Intersection.h"

Intersection::Intersection() { 
	for (int i = 0; i < pNeighbors.size(); i++)
		pNeighbors[i] = nullptr;
}

Intersection::Intersection(sf::Vector2f location) 
	: location(location) {
	for (int i = 0; i < pNeighbors.size(); i++)
		pNeighbors[i] = nullptr;
}

Intersection::~Intersection() { }

void Intersection::setNeighbor(Intersection &intersection, Direction direction) {
	pNeighbors[(int)direction] = &intersection;
}

Intersection& Intersection::getNeighbor(Direction direction) {
	if (pNeighbors[(int)direction] == nullptr)
		return *this;
	else
		return *pNeighbors[(int)direction];
}

sf::Vector2f Intersection::getLocation() {
	return location;
}

bool Intersection::spawnsPellets() { return hasPellets; }
void Intersection::noPellets() { hasPellets = false; }