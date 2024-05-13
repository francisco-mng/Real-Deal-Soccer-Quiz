#pragma once
#include <SFML/graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <vector>
using namespace sf;
using namespace std;


class GameQuestion {

    friend class Game;


    
    Sprite button1, button2, button3;								                //In our case the button is the option sprite(to be animated)
    bool isMCQ = false;												                //Will affect the type of graphics to draw.
    string answer;														            //The answer for this GameButtons instance.

    string squestion, soption1, soption2, soption3, stTrue = "True", tFalse = "False";   //Question Text
public:
    GameQuestion() {}			//Empty constructor.

    //Constructor for True or False. (Pre-set font and text color)
    GameQuestion(string questionString, string ans) {
        isMCQ = false;
        squestion = questionString;
        answer = ans;
    }

    bool isMultipleChoice() {
        return isMCQ;
    }

    //Constructor for MCQ
    GameQuestion(string questionString, string op1, string op2, string op3, string ans) {
        isMCQ = true;
        squestion = questionString;
        soption1 = op1;
        soption2 = op2;
        soption3 = op3;
        answer = ans;
    }

    inline string getAnswer()   { return answer;   }			//In-line function that returns the correct answer.
    inline string getQuestion() { return squestion;}			//In-line function that returns the question.
    inline string getOption1()  { return soption1; }			//In-line function that returns the first option.
    inline string getOption2()  { return soption2; }			//In-line function that returns the second option.
    inline string getOption3()  { return soption3; }			//In-line function that returns the third option.

};


//	GameButtons(string questionString, Color textColor, Font& font, int questionCharSize, int optionCharSize) {
//	GameButtons(string questionString, string op1, string op2, string op3, Color optionTextColor, Color questionTextColor, Font& font, int questionCharSize, int optionCharSize) {

/*
int mainASDaD() {
	RenderWindow win(VideoMode(VideoMode::getDesktopMode().width / 1.3, VideoMode::getDesktopMode().height / 1.3), "Game", Style::Close);
	Text text;
	win.setFramerateLimit(15);
	Font font;
	font.loadFromFile("Pinky Love.ttf");
	text.setFont(font);

	GameButtons buttons("what is your name?", Color::Cyan, font, 30, 25, "Answer");
	//GameButtons tButton("what is your name?", "Ok", "Yah", "Nah", Color::White, Color::Cyan, font, 30, 25, "Answer");

	Event event;
	while (win.isOpen()) {
		while (win.pollEvent(event)) {
			if (event.type == Event::Closed) {
				win.close();
			}
		}
		buttons.showQuestion(win);

		win.display();
	}
	return 0;
}*/


