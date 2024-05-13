/*#include "Menu.h"
#include <SFML/Audio.hpp>
#include "Button.h"
#include <iostream>
#include <thread>

using namespace sf;
using namespace std;
/*
Menu::Menu() {
	window = new RenderWindow(VideoMode(VideoMode::getDesktopMode().width / 1.3, VideoMode::getDesktopMode().height / 1.3), "Game", Style::Fullscreen);
	winClose = new RectangleShape();
	font = new Font();
	image = new Texture();
	background = new Sprite();
	buttonOne = new Button();
	buttonTwo = new Button();
	buttonThree = new Button();
	playButton = new Button();
	nameInput = new TextBox(Color::Green, 30, true, "");

}

Menu::~Menu() {
	delete window;
	delete winClose;
	delete font;
	delete image;
	delete background;
	delete nameInput;
}

//Creating my window
/*
void Menu::myWindow() {

	//Creating my Sprite
	font->loadFromFile("Roboto-BoldItalic.ttf");
	image->loadFromFile("Fifa8.jpg");
	background->setTexture(*image);

	Texture backG;
	backG.loadFromFile("loading2.jpg");
	Sprite tempImage(backG);
	//Sound for the main menu
	SoundBuffer buffer;
	if (!buffer.loadFromFile("MainMenu1.mp3")) {
		return;
	}
	Sound sound(buffer);

	//Setting the size of the Sprite to equal that of the window
	Vector2u windowSize = window->getSize();
	float scaleX = static_cast<float>(windowSize.x) / background->getLocalBounds().width;
	float scaleY = static_cast<float>(windowSize.y) / background->getLocalBounds().height;
	background->setScale(scaleX, scaleY);
	tempImage.setScale(scaleX, scaleY);
	
	Text text;
	Font tempFont;
	tempFont.loadFromFile("Roman SD.ttf");
	text.setFont(tempFont);
	text.setOutlineColor(Color::Black);
	text.setOutlineThickness(2);
	text.setCharacterSize(50);
	text.setFillColor(sf::Color::White);
	text.setString("Closing...");
	text.setPosition((windowSize.x) - 300, (windowSize.y) - 100);

	Vector2u wSize = window->getSize(); //Duplicate
	float xCo = (wSize.x / 2.0f) + 200;
	float yCo = wSize.y / 2.0f;
	Button buttonOne(Color::Blue, 30, Color::White, "Login", Vector2f(180.0f, 60.0f), *font);
	buttonOne.setPosition(Vector2f(xCo , yCo - 100));
	Button buttonTwo(Color::Blue, 30, Color::White, "About", Vector2f(180.0f, 60.0f), *font);
	buttonTwo.setPosition(Vector2f(xCo, yCo));
	Button buttonThree(Color::Blue, 30, Color::White, "Quit", Vector2f(180.0f, 60.0f), *font);
	buttonThree.setPosition(Vector2f(xCo, yCo + 100));

	//addMusic("uefa.ogg", "Fifa7.jpg");//Playing the loading image
	sound.play();
	while (window->isOpen()) {
		Event event;
		while (window->pollEvent(event)) {
			if (event.type == Event::Closed) {
				window->close();
			}			
			if (buttonOne.isMouseOver(*window)) {
				buttonOne.changeBackCol(Color::Green);
				if (buttonOne.clicked(*window, event)) {
					login();
				}
			}
			else {
				buttonOne.changeBackCol(Color::Blue);
			}

			if (buttonTwo.isMouseOver(*window)) {
				buttonTwo.changeBackCol(Color::Green);
				if (buttonTwo.clicked(*window, event)) {
					about();
					buttonTwo.clicked(*window, event);
				}
			}
			else {
				buttonTwo.changeBackCol(Color::Blue);
			}

			if (buttonThree.isMouseOver(*window)) {
				buttonThree.changeBackCol(Color::Green);
				if (buttonThree.clicked(*window, event)) {
					sf::Clock delayClock;
					while (delayClock.getElapsedTime().asSeconds() < 6) {
						window->clear();
						window->draw(tempImage);
						window->draw(text);
						window->display();
					}
					window->close();
				}
			}
			else {
				buttonThree.changeBackCol(Color::Blue);
			}
		}
		window->clear();
		window->draw(*background);
		buttonOne.drawTo(*window);
		buttonTwo.drawTo(*window);
		buttonThree.drawTo(*window);
		window->display();
	}
} 


//Telling a user more about this quiz game
void Menu::about() {
	Texture tempT;
	Text text;
	Event event;

	tempT.loadFromFile("CR7.jpg");
	Sprite backG(tempT);

	Vector2u windowSize = window->getSize(); //Duplicate

	text.setFont(*font);
	text.setCharacterSize(25);
	text.setFillColor(Color::Cyan);
	text.setPosition(50, 50);
	string comment = "Welcome to the Football Quiz Game, where your football knowledge will be \nput to the test! \nThis interactive quiz game is designed to challenge football enthusiasts of all \nlevels with questions ranging from basic trivia to advanced statistics."
		"\n\nThe objective of the Football Quiz Game is to answer as many \nquestions correctly as possible within the given time limit. \nEach level presents a set of 10 questions, and \nplayers must select the correct answers to progress to the next level. \nBe quick and accurate to earn high scores and climb the leaderboard!";
	float scaleX = static_cast<float>(windowSize.x) / backG.getLocalBounds().width;
	float scaleY = static_cast<float>(windowSize.y) / backG.getLocalBounds().height;
	backG.setScale(scaleX, scaleY);

	Button backButton(Color::Blue, 30, Color::White, "<<back", Vector2f(180.0f, 60.0f), *font);
	backButton.setPosition(Vector2f(windowSize.x - 230.0f, windowSize.y - 100.0f));

	bool isBreak = false;
	// Animate the text typing effect
	
	int i = 0;
	while (window->isOpen()) {
		while (window->pollEvent(event)) {
			if (backButton.isMouseOver(*window)) {
				backButton.changeBackCol(Color::Green);
				if (backButton.clicked(*window, event)) {
					isBreak = true;
					break;
				}
			}
			else {
				backButton.changeBackCol(Color::Blue);
			}
		}

		if (isBreak)
			break;

		if (i < comment.length()) {
			text.setString(comment.substr(0, i));
			window->clear();
			window->draw(backG);
			window->draw(text);
			//window->display();
			++i;
		}
		else {//To prevent the last two charecters from flashing
			text.setString(comment);
			window->draw(backG);
			window->draw(text);
		}
		backButton.drawTo(*window);
		window->display();
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	}
}

void Menu::login() {//Text writting functions
	image->loadFromFile("loading1.jpg");
	Sprite backG(*image);
	Vector2u windowSize = window->getSize();//Duplicate 

	float scaleX = static_cast<float>(windowSize.x) / backG.getLocalBounds().width;
	float scaleY = static_cast<float>(windowSize.y) / backG.getLocalBounds().height;
	backG.setScale(scaleX, scaleY);

	Button playButton(Color::Blue, 30, Color::White, "Launch>>", Vector2f(180.0f, 60.0f), *font);
	playButton.setPosition(Vector2f(windowSize.x - 220, windowSize.y - 100));
	nameInput->userInput(*window , backG , playButton);

}

void Menu::addMusic(String str, string pic) {

	SoundBuffer buffer;
	if (!buffer.loadFromFile(str)) {
		return;
	}
	Sound sound(buffer);

	if (pic != "") {
		Texture tempT;
		tempT.loadFromFile(pic);
		Sprite backG(tempT);

		Vector2u windowSize = window->getSize();
		Text text;
		text.setFont(*font);
		text.setCharacterSize(50);
		text.setFillColor(sf::Color::White);
		text.setString("loading...");
		text.setPosition((windowSize.x) - 250 , (windowSize.y) - 100);
		
		float scaleX = static_cast<float>(windowSize.x) / backG.getLocalBounds().width;
		float scaleY = static_cast<float>(windowSize.y) / backG.getLocalBounds().height;
		backG.setScale(scaleX, scaleY);
		sound.play();
	
		while (sound.getStatus() == Sound::Playing) {
			window->draw(backG);
			window->draw(text);
			window->display();
			sleep(sf::milliseconds(100));
		}
	}
}
	
/*
int main() {
	Menu* menu = new Menu();
	menu->myWindow();
	delete menu;
	menu = nullptr;
	return 0;
}
*/