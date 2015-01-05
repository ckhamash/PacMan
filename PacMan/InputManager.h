#pragma once
#include "SFML\Graphics.hpp"
#include "Keyboard.h"

class Game;

enum class Command { TurnUp, TurnRight, TurnDown, TurnLeft, Start, CloseWindow };

class InputManager {
private:
	Game *pGame;

	Keyboard prevKeyboard, currKeyboard;
public:
	InputManager();
	InputManager(Game *game);
	~InputManager();

	bool isDown(sf::Keyboard::Key key);
	bool isUp(sf::Keyboard::Key key);
	bool justPressed(sf::Keyboard::Key key);
	bool justReleased(sf::Keyboard::Key key);

	bool isCommandActive(Command command);

	void update();
};