#include "Pellet.h"

Pellet::Pellet() { }

Pellet::Pellet(sf::Vector2f location, int value, bool energizer) 
	: location(location)
	, value(value)
	, energizer(energizer) { }

Pellet::~Pellet() { }

sf::Vector2f Pellet::getLocation() { return location; }

int Pellet::getValue() { return value; }

bool Pellet::isEnergizer() { return energizer; }