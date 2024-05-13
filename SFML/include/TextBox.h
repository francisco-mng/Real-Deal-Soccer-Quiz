#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

constexpr auto DELETE_KEY = 8;
constexpr auto ENTER_KEY = 13;;
constexpr auto ESCAPE_KEY = 27;

using namespace sf;
using namespace std;


class TextBox {
	friend class UserInput;


private:
	Text textbox;
	ostringstream text	;	//Allows us to take input from user using the >> symbol
	bool isSelected =true;		//Not selected by default
	bool hasLimit = false;		//Can I enter unlimited characters?
	int limit = 0;					//Max numm characters to input. 

public:
	TextBox() {}
	TextBox(Color col, int size, bool selected, string t) {
		string str = "Francisco";
		
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

		textbox.setString(str);
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

	void inputLogic(int charTyped) {	//The unicode for the character typed (<=128)

		//If the character typed is not the delete key, enter key, or escape key we continue
		if ((charTyped != DELETE_KEY) && (charTyped != ENTER_KEY) && (charTyped != ESCAPE_KEY)) {

			//convert the character integer to an actual character
			text << static_cast<char>(charTyped);													
		}
		else if (charTyped == DELETE_KEY) {
			//Only delete if we have at least 1 character in our string
			if (text.str().length() > 0) {

				//Delete char
				deleteChar();
			}
		}
		textbox.setString(text.str() + "_");
	}

	void setFont(Font &font) {
		//Font created outside the class
		textbox.setFont(font);
	}

	//Pass the event object from the main class by reference to save memory. 
	//Actually use this for typing. Don't call the inputLogic() in main. Use this instead. 
	void typedOn(Event &input) {

		//Want to make sure that we type on the textbox that is selected. 
		if (isSelected) {
			int charTyped = input.text.unicode;
			if (charTyped <= 128) {
				if (hasLimit) {
					if (text.str().length() < limit) {			//Make sure to not take any input beyond limit
						inputLogic(charTyped);
					}
					else if (charTyped == DELETE_KEY) {			//Give chance to delete character.
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
		return text.str();			//Read what the user is typing in
	}

	void drawToWindow(RenderWindow &window) {
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
};
