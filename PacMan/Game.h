#pragma once
#include "SFML\Graphics.hpp"
#include <iostream>
#include <random>
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"
#include "LevelManager.h"
#include "InputManager.h"
#include <fstream>

const double PI = 3.141592653589793238463;

enum class GameState { Start, Play, GameOver };

class Game {
private:
	const int scale = 2;
	const sf::Vector2f baseResolution = sf::Vector2f(224, 288);

	sf::RenderWindow window;
	sf::Clock frameTimer;

	PlayerManager playerManager;
	EnemyManager enemyManager;
	CollisionManager collisionManager;
	LevelManager levelManager;
	InputManager inputManager;

	GameState state = GameState::Start;
	int score = 0, hiScore, lifeUpScore;

	std::random_device randomEngine;
	std::uniform_real_distribution<float> percentDistribution;

	sf::Font font;

	sf::Text scoreText, scoreCounter, hiScoreText, hiScoreCounter, gameOverText, startText, restartText, levelText;
public:
	Game();
	~Game();

	void initText();

	int getScale();
	sf::RenderWindow& getWindow();
	PlayerManager& getPlayerManager();
	EnemyManager& getEnemyManager();
	LevelManager& getLevelManager();
	InputManager& getInputManager();

	float getRandom();

	void addScore(int value);
	void loadHiScore();
	void saveHiScore();

	void run();
	void update();
	void display();
};

