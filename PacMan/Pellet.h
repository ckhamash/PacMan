#pragma once
#include "SFML\Graphics.hpp"
class Pellet {
private:
	sf::Vector2f location;
	int value;
	bool energizer = false;
public:
	Pellet();
	Pellet(sf::Vector2f, int value = 10, bool energizer = false);
	~Pellet();

	sf::Vector2f getLocation();
	int getValue();
	bool isEnergizer();
};

