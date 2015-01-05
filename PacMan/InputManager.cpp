#include "InputManager.h"
#include "Game.h"

InputManager::InputManager() { }

InputManager::InputManager(Game *game)
	: pGame(game) { }

InputManager::~InputManager() { }

void InputManager::update() {
	prevKeyboard = currKeyboard;
	currKeyboard = Keyboard();
}

bool InputManager::isDown(sf::Keyboard::Key key) { return currKeyboard.isKeyPressed(key); }

bool InputManager::isUp(sf::Keyboard::Key key) { return !currKeyboard.isKeyPressed(key); }

bool InputManager::justPressed(sf::Keyboard::Key key) {
	return (!prevKeyboard.isKeyPressed(key) && isDown(key));
}

bool InputManager::justReleased(sf::Keyboard::Key key) {
	return (prevKeyboard.isKeyPressed(key) && !isDown(key));
}

bool InputManager::isCommandActive(Command command) {
	switch(command) {
	case Command::TurnUp:
		return isDown(sf::Keyboard::Up);
		break;
	case Command::TurnRight:
		return isDown(sf::Keyboard::Right);
		break;
	case Command::TurnDown:
		return isDown(sf::Keyboard::Down);
		break;
	case Command::TurnLeft:
		return isDown(sf::Keyboard::Left);
		break;
	case Command::Start:
		return justPressed(sf::Keyboard::Space);
		break;
	case Command::CloseWindow:
		sf::Event event;
		while (pGame->getWindow().pollEvent(event))
			if (event.type == sf::Event::Closed)
				return true;
		break;
	}
}