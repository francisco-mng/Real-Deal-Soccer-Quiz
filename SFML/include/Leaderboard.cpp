#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <chrono>
#include <thread> 
#include <string>
#include <SFML/Audio.hpp>
#include <Game.h>




using namespace sf;
using namespace std;

class feedback {

private:
   
    Text PlayerName;

public:
    Font font;
    Texture background;
    int Grand_total = 50;
    string comment;
    Sprite backgroundSprite;
    Text str;
    Button button; 


    feedback(string playerName){
        if (!font.loadFromFile("Roboto-BoldItalic.ttf"))
        {
            std::cout << "Failed to load font" << std::endl;
        }

        PlayerName.setString(playerName);
        PlayerName.setFont(font);
        PlayerName.setPosition(200, 200);

    }


    void playMusic(const string& song, string comment, RenderWindow* window) {


        sf::Music music;

        if (song == "Fail") {
            music.openFromFile("Boo.mp3");
            
        }
        else if (song == "Modarate") {
            music.openFromFile("Boo.mp3");
           
        }
        else if (song == "Success") {
            music.openFromFile("cheering.mp3");
           
        }
        else if (song == "Excellent") {
            music.openFromFile("song.mp3");
           
        }
        int i = 0;
        str.setString(comment);
        str.setCharacterSize(38);
        str.setFont(font);
        str.setFillColor(Color::Black);
        str.setPosition(425, 140);
        music.play();
        while (true) {
            // Keep the window open
            str.setString(comment.substr(0, i));
            window->clear();
            window->draw(backgroundSprite);
            window->draw(str);
            window->display();
            this_thread::sleep_for(std::chrono::milliseconds(50));
            if (i < comment.length()) {
                i++;
            }
            else {
                break;
            }
        }

    }

    // Wait until the music finishes playing
    void displayFeed(double score, RenderWindow *window) {

        double percent = (score / 230) * 100;

        background.loadFromFile("bgFeed.jpeg");
        backgroundSprite = Sprite(background);

        Text perc("%", font, 24);
        Text congra("Congratulations ", font, 24);
        Text wellDone("Well done ", font, 24);
        Text message1("You've got : ", font, 24);
        Text message2("Looks like you were marked absent\n\t\t\tduring the match!\n\t\t\t No worries even\n\t\tRonaldo misses penalties\n\tsometimes.Keep practicing,\n\t\tyou'll soon be scoring\n\t\t\t\t\that-tricks! ", font, 24);
        Text message3("You're dribbling your way through\n\t\t\tthe quiz, but the\n\t\t\tdefense is tough!\n\tRemember, every great player\n\t\thas faced setbacks.\n\tKeep your head up and keep\n\t\t\tpushing forward!", font, 24);
        Text message4("You're showing promise on the field!\n\t\tYour quiz performance\n\t\tis like a solid midfield\n\t\t\t\tplayer not bad,\n\t\tbut there's always room\n\t\t\tfor improvement.\nKeep training and aim for that goal!", font, 24);
        Text message5("\tWow, you're scoring goals\n\t\t\t\tleft and right!\n\t\tYour quiz performance\n\t\t\tis as legendary as\n\t\t\tMessi's footwork.\n\tKeep up the fantastic work\nand continue dominating the pitch...\n\t\tI mean, the quiz! LMAO:)", font, 24);

        backgroundSprite.setPosition(0, 0);


        String percentage = to_string(percent);
        size_t decimalPos = percentage.find('.');
        std::string croppedPercentage = percentage.substring(0, decimalPos + 3);



        Button button(Color::Blue, 34, Color::White, "Close Game!", Vector2f(250, 80), font);
        button.setPosition({ 550,670 });
        // Set the view of the window to the created view


        Vector2u windowSize = window->getSize();
        backgroundSprite.setScale(float(windowSize.x) / background.getSize().x, float(windowSize.y) / background.getSize().y);

        // Cose the appropriate message based on the percentage

        if (percent <= 25) {
            comment = PlayerName.getString() + "\n \n" + message1.getString() + " " + croppedPercentage + perc.getString() + "\n \n " + message2.getString();
            playMusic("Fail", comment, window);
        }
        else if (percent > 25 && percent <= 50) {
            comment = PlayerName.getString() + "\n \n" + message1.getString() + " " + croppedPercentage + perc.getString() + "\n \n " + message3.getString();
            playMusic("Modarate", comment, window);
        }
        else if (percent > 50 && percent <= 75) {
            comment = wellDone.getString() + PlayerName.getString() + "\n \n" + message1.getString() + " " + croppedPercentage + perc.getString() + "\n \n" + message4.getString();
            playMusic("Success", comment, window);

        }

        else if (percent > 75 && percent <= 100) {
            comment = congra.getString() + PlayerName.getString() + "\n \n" + message1.getString() + " " + croppedPercentage + perc.getString() + "\n \n" + message5.getString();
            playMusic("Excellent", comment, window);
        }

        // Keep the window open until manually closed
        Sprite tempImage;
        Texture background; 
        background.loadFromFile("loading2.jpg");
        tempImage.setTexture(background);
        float scaleX = static_cast<float>(windowSize.x) / tempImage.getLocalBounds().width;
        float scaleY = static_cast<float>(windowSize.y) / tempImage.getLocalBounds().height;
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

        while (window->isOpen()) {
            Event event;
            while (window->pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window->close();
                }

                if (button.clicked(*window, event)) {
                    sf::Clock delayClock;
                    while (delayClock.getElapsedTime().asSeconds() < 6) {
                        window->clear();
                        window->draw(tempImage);
                        window->draw(text);
                        window->display();
                    }
                    window->close();
                }if (button.isMouseOver(*window)) {
                    button.changeBackCol(Color::Green);
                }
                else {
                    button.changeBackCol(Color::Blue);
                }
            }
            window->clear();
            window->draw(backgroundSprite);
            button.drawTo(*window);
            window->draw(str);
            window->display();
        }
    }
};

/*
int main() {
    feedback game;
    int playerScore = 29;// Assuming the player scored 8 out of 10 questions
    double percent = (playerScore / static_cast<double>(game.Grand_total)) * 100; // Correct calculation
    game.displayFeed(percent);
}
*/
