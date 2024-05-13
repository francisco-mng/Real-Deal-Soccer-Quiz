// Real Deal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <Game.h>


using namespace std; using namespace sf;
void movePlayer(CircleShape& player, Event& event, int& speed, int& acc);
void movePlayer(CircleShape& player, Event event);

#include <SFML/Graphics.hpp>
#include <locale>

//My implementation of the TexBox and the Button. (Change these to fit project needs)


using namespace sf;


int main() {
    Game g;
    g.myWindow();//Starting point
}
/*




































/*
int mainasdas()
{
    //This will create a fullscreen window
    RenderWindow window(VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height), "Real Deal",Style::Fullscreen);
    CircleShape player;

    player.setRadius(20);
    player.setPosition(window.getSize().x / 2, window.getSize().y / 2);//Center the circle?
    TextBox textBox(Color::White, 40, false, "Francisco");
    Font loveFont;
    Button btn1(Color::Green, 20, Color::Black, "Click Me", Vector2f(200, 50), loveFont);
    btn1.setPosition(Vector2f(window.getSize().x / 2,100));
    //We need to setup the position and font values for the textbox.
    //Load the font in. Saved in project directory.

   
    loveFont.loadFromFile("Pinky Love.ttf");
    textBox.setFont(loveFont);
    textBox.setPosition(Vector2f(100, 100));
    
    //This is for constant time steps between frames. 
    Clock clock;
    Time timeSinceLastUpdate = Time::Zero;


    while (window.isOpen()) {

        //Now we want to be able to select our textbox. 
        //Press enter to start typing. 
        //Press escape when done typing

        //Define our own way of szelecting textboxes.

        if (Keyboard::isKeyPressed(Keyboard::Return)) {         //When press enter key. Select textbox;
            textBox.setSelected(true);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            textBox.setSelected(false);
        }

        Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case Event::Closed:
                window.close();
                break;
            case Event::TextEntered:
                textBox.typedOn(event);
                break;
            case Event::MouseMoved:
               /* if (btn1.isMouseOver(window)) {
                    btn1.changeBackCol(Color::Green);
                }
                else {
                    btn1.changeBackCol(Color::White);
                }

                break;
            case Event :: MouseLeft:
                btn1.dragButton(window, event);
           }

        }
        movePlayer(player, event);
        btn1.dragButton(window, event);
        //btn1.dragButton(window, event);
        window.clear();
        window.draw(player);
        textBox.drawToWindow(window);
        btn1.drawTo(window);
        window.display();
    }

    return 12;
    
}*/

void movePlayer(CircleShape& player, Event& event, int& spd, int& acc) {

    if (event.KeyPressed) {
        if (event.key.code == Keyboard::Up) {
            player.setPosition(player.getPosition().x, player.getPosition().y - spd);
        }

        if (event.key.code == Keyboard::Down) {
            player.setPosition(player.getPosition().x, player.getPosition().y + spd);
        }

        if (event.key.code == Keyboard::Left) {
            player.setPosition(player.getPosition().x - spd, player.getPosition().y);
        }

        if (event.key.code == Keyboard::Right) {
            player.setPosition(player.getPosition().x + spd, player.getPosition().y);
        }
    }
}

void movePlayer(CircleShape& player, Event event) {
    
    int spd = 2;
    if (event.KeyPressed) {
        if (event.key.code == Keyboard::Up) {
            player.setPosition(player.getPosition().x, player.getPosition().y - spd);
        }

        if (event.key.code == Keyboard::Down) {
            player.setPosition(player.getPosition().x, player.getPosition().y + spd);
        }

        if (event.key.code == Keyboard::Left) {
            player.setPosition(player.getPosition().x - spd, player.getPosition().y);
        }

        if (event.key.code == Keyboard::Right) {
            player.setPosition(player.getPosition().x + spd, player.getPosition().y);
        }
    }
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
