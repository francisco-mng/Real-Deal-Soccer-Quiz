#pragma once
#include <SFML/graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
using namespace std;
using namespace sf;


class gameTest
{
	//RenderWindow window(VideoMode(600,400), "My Game", Style::Fullscreen);
public:
	gameTest(){
	}

	RenderWindow* window; //Pointer to the game window.

	//Logic to render window every frame
	void run() {
		window = new RenderWindow(VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height), "I'm sick");
		Event event;
		window->setFramerateLimit(60);
		while (window->isOpen()) {

			while (window->pollEvent(event)) {
				if (event.type == Event::Closed) {
					window->close();
				}
			}
			window->clear();
			move();
			window->draw(player);
		}
	}

	void move() {

		Clock clock;
		cout << clock.getElapsedTime().asSeconds() << endl;
		clock.restart();



		Vector2f pos = player.getPosition();
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			player.setPosition(pos.x, pos.y +speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			player.setPosition(pos.x, pos.y -speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			player.setPosition(pos.x - speed, pos.y );
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			player.setPosition(pos.x + speed, pos.y);
		}
	}


private:
	RectangleShape player;
	int speed = 10;


};

