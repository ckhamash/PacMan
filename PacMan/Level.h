#pragma once
#include "SFML\Graphics.hpp"
#include "Intersection.h"

class Level {
private:
	std::vector<Intersection> intersections;
	sf::Vector2f startPosition;
	Intersection startIntersection;
	std::array<sf::Vector2f, 4> ghostPositions;
	std::array<Intersection, 4> ghostIntersections;
	std::array<sf::Vector2f, 4> ghostCorners;
	std::vector<sf::Vector2f> energizerLocations;

public:
	Level();
	Level(std::vector<Intersection> intersections);
	~Level();

	std::vector<Intersection>& getIntersections();
	void setStartIntersection(int index);
	Intersection& getStartIntersection();
	void setStartPosition(sf::Vector2f position);
	sf::Vector2f getStartPosition();
	void setGhostPositions(std::array<sf::Vector2f, 4> positions);
	std::array<sf::Vector2f, 4> getGhostPositions();
	void setGhostIntersections(std::array<Intersection, 4> positions);
	std::array<Intersection, 4>& getGhostIntersections();
	void setGhostCorners(std::array<sf::Vector2f, 4> positions);
	std::array<sf::Vector2f, 4> getGhostCorners();
	void setEnergizerLocations(std::vector<sf::Vector2f> locations);
	std::vector<sf::Vector2f> getEnergizerLocations();

	void initializeIntersection(int index, std::array<int, 4> neighborIndex, bool hasPellets = true);
	void initializeGhostIntersection(int index, std::array<int, 4> neighborIndex);
	void initializeGhostExit(int index, std::array<int, 4> neighborIndex);
};

