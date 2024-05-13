#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Button.h>
#include "UserInput.h"

class Menu{

	friend class Game;
	sf::RenderWindow* window;
	sf::RectangleShape* winClose;
	sf::Font* font;
	sf::Texture* image;
	sf::Sprite* background;

	Button* buttonOne;
	Button* buttonTwo;
	Button* buttonThree;
	Button* playButton;
	TextBox* nameInput;
	//bool* isMusicPlayed;

protected:
	void login();
	void about();
	void addMusic(sf::String dtr , std::string pic);


public:
	Menu();
	~Menu();
	void myWindow();
};