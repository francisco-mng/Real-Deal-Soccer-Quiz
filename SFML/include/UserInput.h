#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "Button.h"



constexpr auto DELETE = 8;
constexpr auto ENTER = 13;;
constexpr auto ESCAPE = 27;

using namespace sf;
using namespace std;

class TextBox {

private:
    Text textbox;
    ostringstream text;	        //Allows us to take input from user using the >> symbol
    bool isSelected = false;    //Not selected by default
    bool hasLimit = false;		//Can I enter unlimited characters?
    int limit = 0;			    //Max numm characters to input. 

public:

    TextBox() {}
    TextBox(Color col, int size, bool selected, string t) {

        textbox.setFillColor(col);
        textbox.setCharacterSize(size);
        isSelected = selected;

        //This will help us keep track of the selected texboxes.
        if (isSelected) {
            textbox.setString("_");
        }
        else {
            textbox.setString("");
        }
        

    }

    //In case you want to create a constructor with a limit (Specify that hasLimit = true and the limit)
    TextBox(Color col, int size, bool selected, bool haslimit, int limit) {
        textbox.setFillColor(col);
        textbox.setCharacterSize(size);
        isSelected = selected;
        hasLimit = haslimit;
        this->limit = limit;

        //This will help us keep track of the selected texboxes.
        if (isSelected) {
            textbox.setString("_");
        }
        else {
            textbox.setString("");
        }
    }
    
    //Set character size
    void setCharSize(int s) {
        textbox.setCharacterSize(s);
    }

    Text getTextBox() {
        return textbox;
    }

    void inputLogic(int charTyped) {//The unicode for the character typed (<=128)
        
        //If the character typed is not the delete key, enter key, or escape key we continue
        if ((charTyped != DELETE) && (charTyped != ENTER) && (charTyped != ESCAPE)) {

            //convert the character integer to an actual character
            text << static_cast<char>(charTyped);
        }
        else if (charTyped == DELETE) {
            //Only delete if we have at least 1 character in our string
            if (text.str().length() > 0) {

                //Delete char
                deleteChar();
            }
        }
        textbox.setString(text.str() + "_");
    }

    void setFont(Font& font) {

        //font.loadFromFile("comeback.ttf");
        if (!font.loadFromFile("Roman SD.ttf")) {

            cout << "can not load font" << endl;
        }
        //Font created outside the class
        textbox.setFont(font);
    }

    //Pass the event object from the main class by reference to save memory. 
    //Actually use this for typing. Don't call the inputLogic() in main. Use this instead. 
    void typedOn(Event& input) {

        //Want to make sure that we type on the textbox that is selected. 
        if (isSelected) {
            int charTyped = input.text.unicode;
            if (charTyped <= 128) {
                if (hasLimit) {
                    if (text.str().length() < limit) {		//Make sure to not take any input beyond limit
                        inputLogic(charTyped);
                    }
                    else if (charTyped == DELETE) {			//Give chance to delete character.
                        deleteChar();
                    }

                }
                else {										//If there's no limit
                    inputLogic(charTyped);
                }
            }
        }
    }

    //Set the position of our textbox
    void setPosition(Vector2f pos) {
        textbox.setPosition(pos);
    }

    void mouseSelect() {
        //I want to be able to selext the textbox from mouse click. If there's more than one. 
    }

    void setLimit(bool hasLimit) {
        this->hasLimit = hasLimit;
    }

    void setLimit(bool hasLimit, int limit) {
        this->hasLimit = hasLimit;
        this->limit = limit;
    }

    //When textbox is selected we want to show the underscore.
    void setSelected(bool sel) {
        isSelected = sel;
        string t = text.str();
        //If it's not selected, then remove the '_' symbol.
        if (!sel) {

            if (t.length() > 1) {
                textbox.setString(t);
            }
        }
        else {
            textbox.setString(t + "_");
        }
    }

    string getText() {
        return text.str();  //Read what the user is typing in
    }

    void drawToWindow(RenderWindow& window) {
        window.draw(textbox);
    }

    void deleteChar() {
        //Pop out the last character of the string. 
        //Set our text value to the new value.
        string t = text.str();
        t.pop_back();
        text.str("");
        text << t;
        textbox.setString(text.str());
    }

    //Transiction from main menu to the actual game
    void loading(RenderWindow& window) {
        Texture temp;
        temp.loadFromFile("Loading.jpg");
        Sprite tempBackG(temp);

        Vector2u windowSize = window.getSize();
        float scaleX = static_cast<float>(windowSize.x) / tempBackG.getLocalBounds().width;
        float scaleY = static_cast<float>(windowSize.y) / tempBackG.getLocalBounds().height;
        tempBackG.setScale(scaleX, scaleY);

        Font tempFont;
        tempFont.loadFromFile("Roboto-BoldItalic.ttf");

        Text level;
        level.setFont(tempFont);
        level.setCharacterSize(50);
        level.setFillColor(Color::White);
        level.setPosition(windowSize.x / 2 -130, windowSize.y /2 +110 );
        string message = "Please Wait...";

        sf::Clock delayClock;
        while (delayClock.getElapsedTime().asSeconds() < 6) {
            Time elapsed = delayClock.getElapsedTime();
            int charCount = static_cast<int>(elapsed.asSeconds() * 7);
            if (charCount < message.length()) {
                level.setString(message.substr(0, charCount));
            }
            else {
                level.setString(message);
            }
            window.clear();
            window.draw(tempBackG); 
            window.draw(level);
            window.display();
        }
    }

    void userInput(RenderWindow& window , Sprite& backG , Button& playButton) {
      //  Game g;
       // Text textBox;
        TextBox textBox(Color::Green, 40, false, "");
        Font myFont;
        textBox.setFont(myFont);
        textBox.setPosition(Vector2f(100, 150));

        Text animatedText;
        animatedText.setFont(myFont);
        animatedText.setOutlineColor(Color::Black);
        animatedText.setOutlineThickness(3);
        animatedText.setCharacterSize(42);
        animatedText.setFillColor(Color::Blue);
        animatedText.setPosition(50, 100);
        string message = "Enter your username";

        Text greetingText;
        greetingText.setFont(myFont);
        greetingText.setOutlineColor(Color::Black);
        greetingText.setOutlineThickness(3);
        greetingText.setCharacterSize(42);
        greetingText.setFillColor(Color::Blue);
        greetingText.setPosition(50, 200);
        //Button will only be displayed after enetering the username
        bool isLaunch = false;

        Clock clock;
        bool showGreeting = false;
        int count = 0;
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;
                case Event::TextEntered:
                    textBox.typedOn(event);
                    break;
                }

                if (event.type == Event::TextEntered) {
                    typedOn(event);
                }
                if (playButton.isMouseOver(window)) {
                    playButton.changeBackCol(Color::Green);
                    
                    if (playButton.clicked(window, event)) {
                        loading(window);
                        //Franciscos class
                        return;
                    }
                }
                else {
                    playButton.changeBackCol(Color::Blue);
                }
                textBox.setSelected(true);
                if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                    isLaunch = true;
                    textBox.setSelected(false);
                    showGreeting = true;
                    string greetingMessage = "Hello  " + textBox.getText() + " " + "WELCOME TO THE GAME";
                    //g->playerName = textBox.getText();
                    greetingText.setString(greetingMessage);
                }
            }

            Time elapsed = clock.getElapsedTime();
            int charCount = static_cast<int>(elapsed.asSeconds() * 10);
            if (charCount < message.length()) {
                animatedText.setString(message.substr(0, charCount));
            }
            else {
                animatedText.setString(message);
            }
            window.clear();
            window.draw(backG);
            window.draw(animatedText);
            if(isLaunch)
                 playButton.drawTo(window);
            textBox.drawToWindow(window);
            if (showGreeting) 
                window.draw(greetingText);
            window.display();
        }
    }
};


