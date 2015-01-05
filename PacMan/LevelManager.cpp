#include "LevelManager.h"
#include "Game.h"

LevelManager::LevelManager() { }

LevelManager::LevelManager(Game *game) 
	: pGame(game)
	, tileWidth(8 * pGame->getScale())
	, intersectionWidth(2 * tileWidth)
	, startTime(2.5)
	, endTime(3) {
	endClock = endTime;
	laneTexture.loadFromFile("Images/Lane.png");
	pelletTexture.loadFromFile("Images/Pellet.png");
	energizerTexture.loadFromFile("Images/Energizer.png");
	initLevels();

	loadLevel();
}

LevelManager::~LevelManager() { }

int LevelManager::getLevelNumber() { return currentLevel; }

std::array<sf::Vector2f, 4> LevelManager::getGhostCorners() { return levels[currentLevel % levels.size()].getGhostCorners(); }

int LevelManager::getPelletCount() { return pellets.size(); }

sf::Vector2f LevelManager::getPelletLocation(int pelletIndex) { return pellets[pelletIndex].getLocation(); }

void LevelManager::eatPellet(int pelletIndex) { 
	if (pellets[pelletIndex].isEnergizer())
		pGame->getEnemyManager().frightenGhosts();
	pGame->addScore(pellets[pelletIndex].getValue());
	pellets.erase(pellets.begin() + pelletIndex);
	if (pellets.size() < 1) 
		endClock = 0;
}

Level& LevelManager::getLevel() { return levels[currentLevel % levels.size()]; }

bool LevelManager::isStarting() { return startClock < startTime; }

bool LevelManager::isEnding() { return endClock < endTime; }

void LevelManager::restart() { 
	startClock = 0;
	currentLevel = 0;
	// restart enemies
	// restart player
}

void LevelManager::respawn() {
	startClock = 0;
	pGame->getPlayerManager().spawn(levels[currentLevel % levels.size()].getStartIntersection(), levels[currentLevel % levels.size()].getStartPosition());
	pGame->getEnemyManager().spawnGhosts(levels[currentLevel % levels.size()].getGhostIntersections(), levels[currentLevel % levels.size()].getGhostPositions());
}

void LevelManager::initLevels() {
	std::vector<sf::Vector2f> intersectionTileLocations;
	intersectionTileLocations = {
		/* 0*/{ 0,  0 },                  /* 1*/{ 5,  0 },                  /* 2*/{ 11,  0 }, /* 3*/{ 14,  0 },                   /* 4*/{ 20,  0 },                   /* 5*/{ 25,  0 },
		/* 6*/{ 0,  4 },                  /* 7*/{ 5,  4 }, /* 8*/{ 8,  4 }, /* 9*/{ 11,  4 }, /*10*/{ 14,  4 }, /*11*/{ 17,  4 }, /*12*/{ 20,  4 },                   /*13*/{ 25,  4 },
		/*14*/{ 0,  7 },                  /*15*/{ 5,  7 }, /*16*/{ 8,  7 }, /*17*/{ 11,  7 }, /*18*/{ 14,  7 }, /*19*/{ 17,  7 }, /*20*/{ 20,  7 },                   /*21*/{ 25,  7 }, 
		                                                   /*22*/{ 8, 10 }, /*23*/{ 11, 10 }, /*24*/{ 14, 10 }, /*25*/{ 17, 10 }, 
		                                  /*26*/{ 5, 13 }, /*27*/{ 8, 13 },                                     /*28*/{ 17, 13 }, /*29*/{ 20, 13 }, 
						                                   /*30*/{ 8, 16 },                                     /*31*/{ 17, 16 },
		/*32*/{ 0, 19 },                  /*33*/{ 5, 19 }, /*34*/{ 8, 19 }, /*35*/{ 11, 19 }, /*36*/{ 14, 19 }, /*37*/{ 17, 19 }, /*38*/{ 20, 19 },                   /*39*/{ 25, 19 }, 
		/*40*/{ 0, 22 }, /*41*/{ 2, 22 }, /*42*/{ 5, 22 }, /*43*/{ 8, 22 }, /*44*/{ 11, 22 }, /*45*/{ 14, 22 }, /*46*/{ 17, 22 }, /*47*/{ 20, 22 }, /*48*/{ 23, 22 }, /*49*/{ 25, 22 },
		/*50*/{ 0, 25 }, /*51*/{ 2, 25 }, /*52*/{ 5, 25 }, /*53*/{ 8, 25 }, /*54*/{ 11, 25 }, /*55*/{ 14, 25 }, /*56*/{ 17, 25 }, /*57*/{ 20, 25 }, /*58*/{ 23, 25 }, /*59*/{ 25, 25 },
		/*60*/{ 0, 28 }, 				                                    /*61*/{ 11, 28 }, /*62*/{ 14, 28 }, 				                                      /*63*/{ 25, 28 },

		/*warp intersections*/                                              /*64*/{ -3, 13 }, /*65*/{ 28, 13 },
	};
	sf::Vector2f topLeftPosition = { 1.5f * tileWidth, 4.5f * tileWidth };
	Level level1;
	for (int i = 0; i < intersectionTileLocations.size(); i++) {
		level1.getIntersections().push_back(Intersection(topLeftPosition + (intersectionTileLocations[i] * (float)tileWidth)));
	}
	levels.push_back(level1);

	levels[0].initializeIntersection(0, { { -1, 1, 6, -1 } });	levels[0].initializeIntersection(1, { { -1, 2, 7, 0 } });	levels[0].initializeIntersection(2, { { -1, -1, 9, 1 } });	levels[0].initializeIntersection(3, { { -1, 4, 10, -1 } });	levels[0].initializeIntersection(4, { { -1, 5, 12, 3 } });	levels[0].initializeIntersection(5, { { -1, -1, 13, 4 } });
	levels[0].initializeIntersection(6, { { 0, 7, 14, -1 } });	levels[0].initializeIntersection(7, { { 1, 8, 15, 6 } });	levels[0].initializeIntersection(8, { { -1, 9, 16, 7 } });	levels[0].initializeIntersection(9, { { 2, 10, -1, 8 } });	levels[0].initializeIntersection(10, { { 3, 11, -1, 9 } });	levels[0].initializeIntersection(11, { { -1, 12, 19, 10 } });	levels[0].initializeIntersection(12, { { 4, 13, 20, 11 } });	levels[0].initializeIntersection(13, { { 5, -1, 21, 12 } });
	levels[0].initializeIntersection(14, { { 6, 15, -1, -1 } });	levels[0].initializeIntersection(15, { { 7, -1, 26, 14 } });	levels[0].initializeIntersection(16, { { 8, 17, -1, -1 } });	levels[0].initializeIntersection(17, { { -1, -1, 23, 16 } });	levels[0].initializeIntersection(18, { { -1, 19, 24, -1 } });	levels[0].initializeIntersection(19, { { 11, -1, -1, 18 } });	levels[0].initializeIntersection(20, { { 12, 21, 29, -1 } });	levels[0].initializeIntersection(21, { { 13, -1, -1, 20 } });
	levels[0].initializeIntersection(22, { { -1, 23, 27, -1 } }, false);	levels[0].initializeIntersection(23, { { 17, 24, -1, 22 } }, false);	levels[0].initializeIntersection(24, { { 18, 25, -1, 23 } }, false);	levels[0].initializeIntersection(25, { { -1, -1, 28, 24 } }, false);
	levels[0].initializeIntersection(26, { { 15, 27, 33, 64 } });	levels[0].initializeIntersection(27, { { 22, -1, 30, 26 } }, false);	levels[0].initializeIntersection(28, { { 25, 29, 31, -1 } }, false);	levels[0].initializeIntersection(29, { { 20, 65, 38, 28 } });
	levels[0].initializeIntersection(30, { { 27, 31, 34, -1 } }, false);	levels[0].initializeIntersection(31, { { 28, -1, 37, 30 } }, false);
	levels[0].initializeIntersection(32, { { -1, 33, 40, -1 } });	levels[0].initializeIntersection(33, { { 26, 34, 42, 32 } });	levels[0].initializeIntersection(34, { { 30, 35, -1, 33 } });	levels[0].initializeIntersection(35, { { -1, -1, 44, 34 } });	levels[0].initializeIntersection(36, { { -1, 37, 45, -1 } });	levels[0].initializeIntersection(37, { { 31, 38, -1, 36 } });	levels[0].initializeIntersection(38, { { 29, 39, 47, 37 } });	levels[0].initializeIntersection(39, { { -1, -1, 49, 38 } });
	levels[0].initializeIntersection(40, { { 32, 41, -1, -1 } });	levels[0].initializeIntersection(41, { { -1, -1, 51, 40 } });	levels[0].initializeIntersection(42, { { 33, 43, 52, -1 } });	levels[0].initializeIntersection(43, { { -1, 44, 53, 42 } });	levels[0].initializeIntersection(44, { { 35, 45, -1, 43 } });	levels[0].initializeIntersection(45, { { 36, 46, -1, 44 } });	levels[0].initializeIntersection(46, { { -1, 47, 56, 45 } });	levels[0].initializeIntersection(47, { { 38, -1, 57, 46 } });	levels[0].initializeIntersection(48, { { -1, 49, 58, -1 } });	levels[0].initializeIntersection(49, { { 39, -1, -1, 48 } });
	levels[0].initializeIntersection(50, { { -1, 51, 60, -1 } });	levels[0].initializeIntersection(51, { { 41, 52, -1, 50 } });	levels[0].initializeIntersection(52, { { 42, -1, -1, 51 } });	levels[0].initializeIntersection(53, { { 43, 54, -1, -1 } });	levels[0].initializeIntersection(54, { { -1, -1, 61, 53 } });	levels[0].initializeIntersection(55, { { -1, 56, 62, -1 } });	levels[0].initializeIntersection(56, { { 46, -1, -1, 55 } });	levels[0].initializeIntersection(57, { { 47, 58, -1, -1 } });	levels[0].initializeIntersection(58, { { 48, 59, -1, 57 } });	levels[0].initializeIntersection(59, { { -1, -1, 63, 58 } });
	levels[0].initializeIntersection(60, { { 50, 61, -1, -1 } });	levels[0].initializeIntersection(61, { { 54, 62, -1, 60 } });	levels[0].initializeIntersection(62, { { 55, 63, -1, 61 } });	levels[0].initializeIntersection(63, { { 59, -1, -1, 62 } });
	// warp
	levels[0].initializeIntersection(64, { { -1, 26, -1, 65 } }, false);	levels[0].initializeIntersection(65, { { -1, 64, -1, 29 } }, false);

	levels[0].setStartIntersection(44);
	levels[0].setStartPosition((levels[0].getIntersections()[44].getLocation() + levels[0].getIntersections()[45].getLocation()) / 2.0f);

	std::vector<sf::Vector2f> energizers;
	energizers.push_back(topLeftPosition + (sf::Vector2f(0, 2) *(float)tileWidth));
	energizers.push_back(topLeftPosition + (sf::Vector2f(25, 2) *(float)tileWidth));
	energizers.push_back(topLeftPosition + (sf::Vector2f(0, 22) *(float)tileWidth));
	energizers.push_back(topLeftPosition + (sf::Vector2f(25, 22) *(float)tileWidth));
	levels[0].setEnergizerLocations(energizers);

	//set ghost start positions
	std::array<Intersection, 4> ghostHouse;
	ghostHouse[0] = Intersection(topLeftPosition + (sf::Vector2f(12.5, 10) *(float)tileWidth)); // blinky, the exit
	ghostHouse[1] = Intersection(topLeftPosition + (sf::Vector2f(12.5, 13) *(float)tileWidth)); // pinky
	ghostHouse[2] = Intersection(topLeftPosition + (sf::Vector2f(10.5, 13) *(float)tileWidth)); // inky
	ghostHouse[3] = Intersection(topLeftPosition + (sf::Vector2f(14.5, 13) *(float)tileWidth)); // clyde
	levels[0].setGhostIntersections(ghostHouse);

	levels[0].initializeGhostExit(0, { { -1, 24, -1, 23 } }); // blinky
	levels[0].initializeGhostIntersection(1, { {  0,  0,  0,  0 } }); // pinky
	levels[0].initializeGhostIntersection(2, { {  1,  1,  1,  1 } }); // inky
	levels[0].initializeGhostIntersection(3, { {  1,  1,  1,  1 } }); // clyde

	std::array<sf::Vector2f, 4> ghostPositions;
	ghostPositions[0] = (levels[0].getGhostIntersections()[0].getLocation()); // blinky
	ghostPositions[1] = (levels[0].getGhostIntersections()[1].getLocation()); // pinky
	ghostPositions[2] = (levels[0].getGhostIntersections()[2].getLocation()); // inky
	ghostPositions[3] = (levels[0].getGhostIntersections()[3].getLocation()); // clyde
	levels[0].setGhostPositions(ghostPositions);

	std::array<sf::Vector2f, 4> ghostCorners;
	ghostCorners[0] = (levels[0].getIntersections()[5].getLocation()); // blinky
	ghostCorners[1] = (levels[0].getIntersections()[0].getLocation()); // pinky
	ghostCorners[2] = (levels[0].getIntersections()[63].getLocation()); // inky
	ghostCorners[3] = (levels[0].getIntersections()[60].getLocation()); // clyde
	levels[0].setGhostCorners(ghostCorners);
}

void LevelManager::loadLevel() {
	startClock = 0;
	pGame->getPlayerManager().spawn(levels[currentLevel % levels.size()].getStartIntersection(), levels[currentLevel % levels.size()].getStartPosition());
	pGame->getEnemyManager().spawnGhosts(levels[currentLevel % levels.size()].getGhostIntersections(), levels[currentLevel % levels.size()].getGhostPositions());
	pGame->getEnemyManager().restart();
	spawnPellets();
}

void LevelManager::levelUp() {
	currentLevel++;
	loadLevel();
}

void LevelManager::spawnPellets() {
	pellets.clear();
	for (int i = 0; i < levels[currentLevel % levels.size()].getIntersections().size(); i++) {
		Intersection* pIntersection = &levels[currentLevel % levels.size()].getIntersections()[i];
		if (pIntersection->spawnsPellets()) {
			pellets.push_back(Pellet(pIntersection->getLocation()));
			for (int j = 0; j < 2; j++) { // only need to spawn pellets in two directions
				Intersection* pOther = &pIntersection->getNeighbor((Direction)j);
				if (pOther != pIntersection && pOther->spawnsPellets()) {
					sf::Vector2f direction = pOther->getLocation() - pIntersection->getLocation();
					float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
					int n = distance / tileWidth; // 1 pellet per 'tile'
					direction = direction / (float)n;

					for (int k = 1; k < n; k++) {
						sf::Vector2f pelletLocation = pIntersection->getLocation() + (direction * (float)k);
						sf::Vector2f difference = pGame->getPlayerManager().getLocation() - pelletLocation;
						float distance = std::sqrt(difference.x * difference.x + difference.y * difference.y);
						if (distance > pGame->getPlayerManager().getRadius() * 2)
							pellets.push_back(Pellet(pelletLocation));
					}
				}
			}
		}
	}

	// set energizer pellets
	for (int i = 0; i < pellets.size(); i++)
		for (int j = 0; j < levels[currentLevel % levels.size()].getEnergizerLocations().size(); j++)
			if (pellets[i].getLocation() == levels[currentLevel % levels.size()].getEnergizerLocations()[j]) {
				pellets[i] = Pellet(pellets[i].getLocation(), 50, true);
			}
}

void LevelManager::update(float secondsSinceLastFrame) {
	startClock += secondsSinceLastFrame;
	endClock += secondsSinceLastFrame;

	if (pellets.size() < 1) {
		if (!isEnding())
			levelUp();
	}
}

void LevelManager::drawLanes() {
	for (int i = 0; i < levels[currentLevel % levels.size()].getIntersections().size(); i++) {
		Intersection* pIntersection = &levels[currentLevel % levels.size()].getIntersections()[i];
		for (int j = 0; j < 4; j++) {
			Intersection* pOther = &pIntersection->getNeighbor((Direction)j);
			if (pOther != pIntersection) {
				sf::FloatRect windowRect(0, 0, pGame->getWindow().getSize().x, pGame->getWindow().getSize().y);
				if (windowRect.contains(pIntersection->getLocation()) || windowRect.contains(pOther->getLocation())) {
					sf::Sprite lane(laneTexture);
					lane.setScale(pGame->getScale(), pGame->getScale());
					lane.setOrigin(lane.getLocalBounds().width / 2, lane.getLocalBounds().height);

					sf::Vector2f direction = pOther->getLocation() - pIntersection->getLocation();
					float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
					int n = distance / lane.getGlobalBounds().height;

					if (direction.y < 0)
						lane.setRotation(std::atan(-direction.x / direction.y) * 180.0f / PI);
					else
						lane.setRotation(std::atan(-direction.x / direction.y) * 180.0f / PI + 180.0f);

					direction = direction / (float)n;
					int numEmpty = (intersectionWidth / 2) / (distance / n);
					for (int k = numEmpty; k < n - numEmpty; k++) {
						if (k % 2 - 1 == 0) {
							lane.setPosition(pIntersection->getLocation() + (direction * (float)k));
							pGame->getWindow().draw(lane);
						}
					}
				}
			}
			else {
				sf::Sprite wallLeft(laneTexture);
				wallLeft.setScale(pGame->getScale(), pGame->getScale());
				wallLeft.setTextureRect(sf::IntRect(0, 0, wallLeft.getLocalBounds().width / 2, wallLeft.getLocalBounds().height));
				wallLeft.setOrigin(wallLeft.getLocalBounds().width, wallLeft.getLocalBounds().height);
				wallLeft.setRotation(90.0f);
				float thickness = wallLeft.getLocalBounds().height * 2;

				sf::Sprite wallRight(laneTexture);
				wallRight.setScale(pGame->getScale(), pGame->getScale());
				wallRight.setTextureRect(sf::IntRect(wallRight.getLocalBounds().width / 2, 0, wallRight.getLocalBounds().width / 2, wallRight.getLocalBounds().height));
				wallRight.setOrigin(0, wallRight.getLocalBounds().height);
				wallRight.setRotation(270.0f);

				sf::Vector2f wallStart, direction;
				for (int k = 0; k < j; k++) {
					wallLeft.rotate(90);
					wallRight.rotate(90);
				}
				if (j == 0) {
					wallStart = { pIntersection->getLocation().x - intersectionWidth / 2, pIntersection->getLocation().y };
					direction = { thickness, 0 };
				}
				else if (j == 1) {
					wallStart = { pIntersection->getLocation().x, pIntersection->getLocation().y - intersectionWidth / 2 };
					direction = { 0, thickness };
				}
				else if (j == 2) {
					wallStart = { pIntersection->getLocation().x + intersectionWidth / 2, pIntersection->getLocation().y };
					direction = { -thickness, 0 };
				}
				else if (j == 3) {
					wallStart = { pIntersection->getLocation().x, pIntersection->getLocation().y + intersectionWidth / 2 };
					direction = { 0, -thickness };
				}

				int n = intersectionWidth / thickness;
				for (int k = 1; k < n; k += 2) {
					wallLeft.setPosition(wallStart + direction * (float)k);
					wallRight.setPosition(wallStart + direction * (float)k);
					pGame->getWindow().draw(wallLeft);
					pGame->getWindow().draw(wallRight);
				}
			}
		}
	} 
}

void LevelManager::drawPellets() { 
	sf::Sprite pellet(pelletTexture);
	sf::Sprite energizer(energizerTexture);
	pellet.setScale(pGame->getScale(), pGame->getScale());
	pellet.setOrigin(pellet.getLocalBounds().width / 2, pellet.getLocalBounds().height / 2);
	energizer.setScale(pGame->getScale(), pGame->getScale());
	energizer.setOrigin(energizer.getLocalBounds().width / 2, energizer.getLocalBounds().height / 2);

	for (int i = 0; i < pellets.size(); i++) { 
		if (pellets[i].isEnergizer()) {
			energizer.setPosition(pellets[i].getLocation());
			pGame->getWindow().draw(energizer);
		}
		else {
			pellet.setPosition(pellets[i].getLocation());
			pGame->getWindow().draw(pellet);
		}
	}
}

void LevelManager::draw() {
	if (!isEnding())
		drawLanes();
	else if (std::cos(endClock * PI * 2) > 0)
		drawLanes();
	drawPellets();
}