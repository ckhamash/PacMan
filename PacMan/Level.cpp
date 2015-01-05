#include "Level.h"

Level::Level() { }

Level::Level(std::vector<Intersection> intersections) 
	: intersections(intersections) { }

Level::~Level() { }

std::vector<Intersection>& Level::getIntersections() { return intersections; }

void Level::setStartIntersection(int index) { startIntersection = intersections[index]; }

Intersection& Level::getStartIntersection() { return startIntersection; }

void Level::setStartPosition(sf::Vector2f position) { startPosition = position; }

sf::Vector2f Level::getStartPosition() { return startPosition; }

void Level::setGhostPositions(std::array<sf::Vector2f, 4> positions) { ghostPositions = positions; }

std::array<sf::Vector2f, 4> Level::getGhostPositions() { return ghostPositions; }

void Level::setGhostIntersections(std::array<Intersection, 4> intersections) { ghostIntersections = intersections; }

std::array<Intersection, 4>& Level::getGhostIntersections() { return ghostIntersections; }

void Level::setGhostCorners(std::array<sf::Vector2f, 4> positions) { ghostCorners = positions; }

std::array<sf::Vector2f, 4> Level::getGhostCorners() { return ghostCorners; }

void Level::setEnergizerLocations(std::vector<sf::Vector2f> locations) { energizerLocations = locations; }

std::vector<sf::Vector2f> Level::getEnergizerLocations() { return energizerLocations; }

void Level::initializeIntersection(int index, std::array<int, 4> neighborIndex, bool hasPellets) {
	if (neighborIndex[0] > -1 && neighborIndex[0] != index)
		intersections[index].setNeighbor(intersections[neighborIndex[0]], Direction::Up);
	if (neighborIndex[1] > -1 && neighborIndex[1] != index)
		intersections[index].setNeighbor(intersections[neighborIndex[1]], Direction::Right);
	if (neighborIndex[2] > -1 && neighborIndex[2] != index)
		intersections[index].setNeighbor(intersections[neighborIndex[2]], Direction::Down);
	if (neighborIndex[3] > -1 && neighborIndex[3] != index)
		intersections[index].setNeighbor(intersections[neighborIndex[3]], Direction::Left);
	if (!hasPellets)
		intersections[index].noPellets();
}

void Level::initializeGhostIntersection(int index, std::array<int, 4> neighborIndex) {
	if (neighborIndex[0] > -1 && neighborIndex[0] != index)
		ghostIntersections[index].setNeighbor(ghostIntersections[neighborIndex[0]], Direction::Up);
	if (neighborIndex[1] > -1 && neighborIndex[1] != index)
		ghostIntersections[index].setNeighbor(ghostIntersections[neighborIndex[1]], Direction::Right);
	if (neighborIndex[2] > -1 && neighborIndex[2] != index)
		ghostIntersections[index].setNeighbor(ghostIntersections[neighborIndex[2]], Direction::Down);
	if (neighborIndex[3] > -1 && neighborIndex[3] != index)
		ghostIntersections[index].setNeighbor(ghostIntersections[neighborIndex[3]], Direction::Left);
}

void Level::initializeGhostExit(int index, std::array<int, 4> neighborIndex) {
	if (neighborIndex[0] > -1)
		ghostIntersections[index].setNeighbor(intersections[neighborIndex[0]], Direction::Up);
	if (neighborIndex[1] > -1)
		ghostIntersections[index].setNeighbor(intersections[neighborIndex[1]], Direction::Right);
	if (neighborIndex[2] > -1)
		ghostIntersections[index].setNeighbor(intersections[neighborIndex[2]], Direction::Down);
	if (neighborIndex[3] > -1)
		ghostIntersections[index].setNeighbor(intersections[neighborIndex[3]], Direction::Left);
}