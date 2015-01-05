#include "Game.h"


Game::Game() 
	: window(sf::VideoMode(baseResolution.x * scale, baseResolution.y * scale), "Pac-Man", sf::Style::Close)
	, playerManager(this)
	, enemyManager(this)
	, collisionManager(this)
	, levelManager(this)
	, inputManager(this)
	, lifeUpScore(10000){
	percentDistribution = std::uniform_real_distribution<float>(0, 1);
	loadHiScore();
	initText();
}

Game::~Game() { }

void Game::initText() {
	font.loadFromFile("Fonts/PressStart2P.ttf");

	scoreText = sf::Text();
	scoreText.setFont(font);
	scoreText.setColor(sf::Color::White);
	scoreText.setCharacterSize(8 * scale);
	scoreText.setString("SCORE");
	scoreText.setPosition(0, 0);

	scoreCounter = sf::Text();
	scoreCounter.setFont(font);
	scoreCounter.setColor(sf::Color::White);
	scoreCounter.setCharacterSize(8 * scale);
	scoreCounter.setPosition(0, scoreText.getCharacterSize());

	hiScoreText = sf::Text();
	hiScoreText.setFont(font);
	hiScoreText.setColor(sf::Color::White);
	hiScoreText.setCharacterSize(8 * scale);
	hiScoreText.setString("HIGH SCORE");
	hiScoreText.setPosition(window.getSize().x - hiScoreText.getGlobalBounds().width + (1 * scale), 0);

	hiScoreCounter = sf::Text();
	hiScoreCounter.setFont(font);
	hiScoreCounter.setColor(sf::Color::White);
	hiScoreCounter.setCharacterSize(8 * scale);

	levelText = sf::Text();
	levelText.setFont(font);
	levelText.setColor(sf::Color::Yellow);
	levelText.setString("LEVEL: " + std::to_string(levelManager.getLevelNumber() + 1) + " \n\n\n\n\n\n\n\n\n\n\n\n READY!");
	levelText.setCharacterSize(8 * scale);
	levelText.setPosition(window.getSize().x / 2 - levelText.getGlobalBounds().width / 2, window.getSize().y / 2 - 80 * scale);

	startText = sf::Text();
	startText.setFont(font);
	startText.setColor(sf::Color::Cyan);
	startText.setCharacterSize(8 * scale);
	startText.setString("PRESS SPACE\n\n\n\n\n\n TO START");
	startText.setPosition(window.getSize().x / 2 - startText.getGlobalBounds().width / 2, window.getSize().y / 2 - 32 * scale);

	restartText = sf::Text();
	restartText.setFont(font);
	restartText.setColor(sf::Color::Cyan);
	restartText.setCharacterSize(8 * scale);
	restartText.setString("PRESS SPACE\n\n\n\n\n\nTO RESTART");
	restartText.setPosition(window.getSize().x / 2 - restartText.getGlobalBounds().width / 2, window.getSize().y / 2 - 32 * scale);

	gameOverText = sf::Text();
	gameOverText.setFont(font);
	gameOverText.setColor(sf::Color::Red);
	gameOverText.setCharacterSize(8 * scale);
	gameOverText.setString("GAME OVER");
	gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, window.getSize().y / 2 + 16 * scale);

}

int Game::getScale() { return scale; }

sf::RenderWindow& Game::getWindow() { return window; }

PlayerManager& Game::getPlayerManager() { return playerManager; }

EnemyManager& Game::getEnemyManager() { return enemyManager; }

LevelManager& Game::getLevelManager() { return levelManager; }

InputManager& Game::getInputManager() { return inputManager; }

float Game::getRandom() {
	return percentDistribution(randomEngine);
}

void Game::addScore(int value) {
	if (score < lifeUpScore && score + value >= lifeUpScore)
		playerManager.lifeUp();
	score += value;
}

void Game::loadHiScore() {
	std::ifstream saveFile;
	saveFile.open("hiscore.txt");
	if (saveFile.is_open()) {
		std::string line;
		std::getline(saveFile, line);
		hiScore = std::stoi(line);
	}
	else
		hiScore = 0;
	saveFile.close();
}

void Game::saveHiScore() {
	std::ofstream saveFile;
	saveFile.open("hiscore.txt");
	hiScore = score;
	saveFile << hiScore;
	saveFile.close();
}

void Game::run() {
	while (window.isOpen()) {
		update();
		display();
	}
}

void Game::update() {
	float secondsSinceLastFrame = frameTimer.restart().asSeconds();

	inputManager.update();
	if (inputManager.isCommandActive(Command::CloseWindow))
		window.close();
	if (inputManager.isCommandActive(Command::Start) && state != GameState::Play) {
		state = GameState::Play;
		levelManager.loadLevel();
		playerManager.restart();
		saveHiScore();
		score = 0;
	}

	if (state == GameState::Play  && !levelManager.isEnding()) {
		if (playerManager.isRespawning()) {
			playerManager.update(secondsSinceLastFrame);
		}
		else if (!levelManager.isStarting()) {
			playerManager.update(secondsSinceLastFrame);
			enemyManager.update(secondsSinceLastFrame);
			collisionManager.update();
			if (!playerManager.isAlive())
				state = GameState::GameOver;
		}
	}

	if (!playerManager.isRespawning()) {
		levelManager.update(secondsSinceLastFrame);
	}
	if (hiScore < score)
		hiScore = score;
}

void Game::display() {
	window.clear();
	// score message
	scoreCounter.setString(std::to_string(score));
	window.draw(scoreText);
	window.draw(scoreCounter);

	// hi score
	hiScoreCounter.setString(std::to_string(hiScore));
	hiScoreCounter.setPosition(window.getSize().x - hiScoreCounter.getGlobalBounds().width + (1 * scale), hiScoreText.getCharacterSize());
	window.draw(hiScoreText);
	window.draw(hiScoreCounter);

	if (state == GameState::Start) {
		window.draw(startText);
	}

	if (state == GameState::GameOver)
		window.draw(gameOverText);

	if (levelManager.isStarting() && state == GameState::Play)
		window.draw(levelText);

	levelManager.draw();
	if (state == GameState::Play) {
		playerManager.draw();
		enemyManager.draw();
	}

	window.display();
}