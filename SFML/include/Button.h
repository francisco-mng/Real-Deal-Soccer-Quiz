#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>

using namespace sf;
using namespace std;


//Basically a shape object with a text in the middle. 
//A button is just a rectangle with text at the center. 
class Button {


private:
	Text text;
	RectangleShape button;
	bool isClicked = false;				//Has the button been clicked
	Vector2f offset;					//How far is the mouse from the origin of the button.
	bool changeColOnMouseMove = false;	//Do I want to change the color of my button when I hover mouse over it?


public:
	Button() {}
	Button(Color backGroundCol, int charSize, Color textColor, string t, Vector2f size, Font& font) {
		text.setString(t);
		text.setFont(font);
		text.setCharacterSize(charSize);
		text.setFillColor(textColor);
		button.setSize(size);
		button.setFillColor(backGroundCol);
		width = 0;
		height = 0;
	}
	int width;
	int height;


	void drawTo(RenderWindow& window) {	//Ensure text is over the button, not vice versa
		window.draw(button);
		window.draw(text);
	}

	//Logic for detecting if mouse is over our button. 
	bool isMouseOver(RenderWindow& window) {

		float mousex = Mouse::getPosition(window).x;
		float mousey = Mouse::getPosition(window).y;

		float x = button.getPosition().x;
		float y = button.getPosition().y;

		float btnPosWidthx = x + button.getLocalBounds().width;		//x coordinate of the end of the button rectangle.
		float btnPosHeight = y + button.getLocalBounds().height;	//y coordinate of the end of the button rectangle.

		//Now detemining it the mouse is on the bounds of my object. 
//This doesn't take into account the z-index of each element.

		if ((mousex >= x) && (mousex <= btnPosWidthx) && (mousey >= y) && (mousey <= btnPosHeight)) {
			//cout << "Mouse is over" << endl;
			return true;
		}
		else {
			return false;
			//cout << "Mouse is over" << endl;
		}
	}

	void setFont(Font font) {
		//Font created outside the class
		text.setFont(font);
		updateState();
	}

	void setText(string t) {
		text.setString(t);
		updateState();
	}

	void changeBackCol(Color col) {
		button.setFillColor(col);
	}
	void changeTextCol(Color col) {
		text.setFillColor(col);
	}

	//Allows for draggable button. 
	void runAwayFromMouse(RenderWindow& window, Event& event) {

		int mousex, mousey;
		mousex = Mouse::getPosition().x;
		mousey = Mouse::getPosition().y;
		Vector2f offset;

		if (isMouseOver(window)) {
			cout << "Dragging";
			offset = { mousex - getPosition().x ,mousey - getPosition().y };
			setPosition(Vector2f(mousex + offset.x, mousey + offset.y));
		}

		if (event.type == Event::MouseButtonReleased) {
			isClicked = false;
		}
	}

	//Call this in game while loop to enable button drag
	void dragButton(RenderWindow& window, Event& event) {

		int mousex, mousey;
		mousex = Mouse::getPosition().x;
		mousey = Mouse::getPosition().y;
		if ((event.key.code == Mouse::Left) && (isMouseOver(window) && isClicked == false)) {
			//cout << "Mouse button pressed";

			//Add additional code here if necessary

			isClicked = true;
			offset = { mousex - getPosition().x ,mousey - getPosition().y };
		}

		if (isClicked) {
			//cout << "Dragging";
			setPosition(Vector2f(mousex - offset.x, mousey - offset.y));
		}

		if (event.type == Event::MouseButtonReleased) {
			isClicked = false;
		}
		updateState();
	}

	//Call this in game while loop to enable mouse click
	bool clicked(RenderWindow& window, Event& event) {
		bool temp;
		if ((event.key.code == Mouse::Left) && (isMouseOver(window) && isClicked == false)) {
			isClicked = true;
		}
		if (isClicked) {
			//Enter your code here.
			//text.setString("You clicked me!");

		}

		if (event.type == Event::MouseButtonReleased) {
			isClicked = false;
		}
		updateState();
		temp = isClicked;
		isClicked = false;
		return temp;
	}

	//Call this when you've made changes to text of button or something like that. 
	//This centers the text of the button
	void updateState() {
		setPosition(getPosition());
	}

	Vector2f getPosition() {
		return button.getPosition();
	}

	void setPositionRelativeToWindow() {

	}
	void setPosition(Vector2f position) {
		float x = position.x + button.getLocalBounds().width / 2 - text.getLocalBounds().width / 2;
		float y = position.y + button.getLocalBounds().height / 4 - text.getLocalBounds().height / 4;
		button.setPosition(position.x, position.y);
		text.setPosition(x, y);
	}

	void setSize(const std::vector<int>& size) {
		if (size.size() >= 2) {
			width = size[0];
			height = size[1];
			std::cout << "Button size set to width: " << width << ", height: " << height << std::endl;
		}
		else {
			std::cerr << "Error: Vector size should be at least 2 (width and height)." << std::endl;
		}
	}

};