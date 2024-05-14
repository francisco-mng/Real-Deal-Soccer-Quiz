#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <GameButtons.h>
#include <fstream>
#include "vector"
#include <sstream>
#include <thread>
#include <stack>
#include <UserInput.h>
#include "Leaderboard.cpp"



//Linking Menu and game




using namespace std;
using namespace sf;


void drawElements(); //Forward declaration
void selectLevel();


class Game
{
    bool attemptedLevel1 = false;
    bool attemptedLevel2 = false;
    bool attemptedLevel3 = false;

    string playerName;
    

    friend class GameButtons;
    int currentLevel = 1;
    double scoreCounter = 0;                  //Each question that's right gets 10 points. 


    float timeLeft =0;                      //How much time is left? 



    Texture bacground;
    Sprite playerSprite;                    //Player
    float playerScale = 1.8;                //The size of the player.
    float playerWidth = 0;                  //The width of the player given the current playing animation

    bool isWalking = true;                  //Is my player moving?
    bool canHit = false;                    //Is my player smacking? True by default. Similar to semaphore. 
    bool isLookingRight = true;


    //Black and white background
    RectangleShape backg;

    //Using friends to give access to private fields
    friend class TextBox;
    friend class feedback;


    float spd = 20;                         //Speed of the player movement
    Vector2f playerPosition = { 0,450 };     //Initially the position of the player is top left corner;
    Texture texture;                        //Our game animation spritemap
    Sprite background;                      //The sprite that draws our texture
    Sprite backGroundLayer;


    //Spritemap
    int ithWalkingSprite = 0;       //Keeps track of current sprite for the walking animation. (How many units to the right?)
    int ithHittingSprite = 0;       //Keeps track of current sprite for attacking animation. 
    int ithStandingSprite = 0;      //Keeps track of current sprite for standing animation.

    Vector2f tempPosition;          //Used for keeping the player on the same vertical level when attacking. Otherwise the player seems to jump down. 

    //Loaded for each question loaded from text file.
    Text question, option1, option2, option3, tTrue, tFalse, tScore, tClock;

    bool isMultipleChoice;

    //Level Selection components
    Font afont;
    Text Quest;
    Text Instructions;
    Texture backgroundt;
    Texture bg;
    Sprite backgroundSprite;


    //Do we have an open window? (Avoiding creating new windows)
    bool opened = false;
    bool isLevelSelected = false;



    //Main menu content
    //friend class Game;
    sf::RenderWindow* window;
    sf::RectangleShape* winClose;
    sf::Font* font;
    sf::Texture* image;
    sf::Sprite* menuBackground;

    Button* buttonOne;
    Button* buttonTwo;
    Button* buttonThree;
    Button* playButton;
    TextBox* nameInput;

    //Music 
    SoundBuffer buffer;         //This is our sound buffer. 
    Sound sound;                //Our actual sound to be loaded from the sound buffer.




public:
    
    //Game logic and loading questions. 
    std::vector<GameQuestion> questions;        //Every question object is stored in array, later to be selected at random.
    std::vector<int> numbersSelected;              //Store a set of selected question numbers (preventing a question from repeating).
    std::stack<GameQuestion> questionStack;
    //Question to show on screen



    

    //Set's the position of text elements in screen. This is a fixed position.
    void setQuesitonElementsPosition(Vector2u windowSize, Text& question, Text& option1, Text& option2, Text& option3, Text& tTrue, Text& tFalse) {
        //Set position of question and options depending on isMCQ and windowSize.
        question.setPosition((windowSize.x / 2.0f) - 630, (windowSize.y / 2.0f) - 120);
        tScore.setPosition(windowSize.x/2 - tScore.getGlobalBounds().width/2, 100);
        tClock.setPosition(windowSize.x / 2 - tClock.getGlobalBounds().width / 2, 20);             //Draw clock behind character. 
        if (currentQuestion.isMultipleChoice()) {
            option1.setPosition(40, windowSize.y - 130);
            option2.setPosition((windowSize.x / 2.0f) - 40, windowSize.y - 130);
            option3.setPosition(windowSize.x - 260, windowSize.y - 130);
        }
        else {
            tTrue.setPosition(400, windowSize.y - 130);
            tFalse.setPosition(windowSize.x - 310, windowSize.y - 130);
        }
    }

    void setScreenFont(Font font) {
        question.setFont(font);
        option1.setFont(font);
        option2.setFont(font);
        option3.setFont(font);
        tTrue.setFont(font);
        tFalse.setFont(font);
    }

    void setCharSize(Text& t, int value) {
        t.setCharacterSize(value);
    }

    void changeTextCol(Color col) {
        question.setFillColor(col);
        option1.setFillColor(col);
        option2.setFillColor(col);
        option3.setFillColor(col);
        tTrue.setFillColor(col);
        tFalse.setFillColor(col);
    }
    GameQuestion currentQuestion;                       //Current question showing on screen
    

    void run(int level) {

       //Lower volume of music to create a focused environment
        sound.setVolume(20.f);
       

        //Initializing current level tracker
        currentLevel = level;

        //Initializing all containers to be empty; Every run must load these values from scratch
        questions = vector<GameQuestion>();        
        numbersSelected = vector<int>();                           
        questionStack = stack<GameQuestion>();
        

        //Initializing RenderWindow
        

        //Game background texture
        if (!(attemptedLevel1 || attemptedLevel2 || attemptedLevel3)) {
            texture.loadFromFile("Game environment.jpg");
        }
        
        background;
        background.setTexture(texture);
        background.setScale({ .9, .9 });
        background.setPosition(background.getPosition() - Vector2f(300, -500));
        backGroundLayer = background;
        backGroundLayer.setPosition(background.getPosition() - Vector2f(0, 600));

        //Each level will ask 10 questions each. Each question loaded into a stack.
        ifstream questionStream;

        //Banele created the screen to select a level.
        if (currentLevel == 1) {
            questionStream = ifstream("level1.txt"); //Replace with Level1.txt
            timeLeft = 100;
            cout << "Loaded level1";
        }
        else if (currentLevel == 2) {
            timeLeft = 60;
            questionStream = ifstream("level2.txt"); //Replace with Level2.txt
            cout << "Loaded level2";
        }
        else {
            timeLeft = 30;
            questionStream = ifstream("level3.txt"); //Replace with Level3.txt
            cout << "Loaded level3";
        }


        //Questions text file loads. Question objects loaded into a vector.
        //Input file stream for questions text file. 


        if (questionStream.is_open()) {
            //If opening the file is successful. 
            string line;
            string token;

            //While loop for reading file. 
            while (getline(questionStream, line)) {

                vector<string> data;                //Store individual words in our line
                stringstream s(line);               //Input line from text file to string stream. 


                //Separate the using the # symbol to get individual tokes of each line.
                while (getline(s, token, '#')) {
                    data.push_back(token);
                }
                cout << data[0] << endl;

                if (data[0] == "MCQ") {
                    //Create and load MCQ Question into the questions vector.
                    questions.push_back(GameQuestion(data[1], data[2], data[3], data[4], data[5]));
                    //cout << "Created MCQ object" <<endl;
                }
                else if (data[0] == "TF") {
                    //Create and load TF Question into the questions vector.
                    questions.push_back(GameQuestion(data[1], data[2]));
                    //cout << "Created TF object" << endl;
                }
                // return;
                cout << questions.size() << endl;
            }


        }


        //At this point we have a vector of all game objects.
        //Now we need to generate 10 random numbers and push the questions into a stack.
        //Select the randomly generated number to load that corresponding question.


        //Generating Random Numbers for questions to be selected
        // Set the range for random numbers
        int minValue = 0;
        int maxValue = questions.size() -1;

        // Seed the random number generator with the current time
        srand(static_cast<unsigned int>(std::time(nullptr)));

        // Vector to store the generated numbers
        vector<int> generatedNumbers;

        while (generatedNumbers.size() < 10) {
                int randomNum = rand() % (maxValue - minValue + 1) + minValue;

            // Check if the number is already in the vector
            bool alreadyPresent = false;
            for (int num : generatedNumbers) {
                if (num == randomNum) {
                    alreadyPresent = true;
                    break;
                }
            }

            // If the number is not already in the vector, add it
            if (!alreadyPresent) {
                generatedNumbers.push_back(randomNum);
            }
        }

        for (int i = 0; i < generatedNumbers.size(); i++) {
            cout << generatedNumbers[i] << endl;
            questionStack.push(questions[generatedNumbers[i]]);
        }




        //Push randdomly selected questions onto game stack.




//Question showing on screen from here below\\//
        Font font;
        font.loadFromFile("Pinky Love.ttf");


        //Now we have GameQuestions, what next? Let's look at the loadNextQuestions()       

        loadNextQuestion();                                                 //Loads up and updates our current question graphics objects.

        //The idea is for this game is tha the player is going to move around smacking the correct answer to the different questions.
        bacground.loadFromFile("Bubba the Cave Duck.png");
        playerSprite.setTexture(bacground);
        window->setFramerateLimit(18);

        //Read the text from the textfile and meke sure that questions picked are at random. I'll use a stack to load the question game buttons.
        //So create a stack of GameButtons. 



        Clock clock;
        while (window->isOpen()) {
            Event event;
            while (window->pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window->close();
                    break;
                }
            }
            if (questionStack.size() == 1) {    //Question stack is empty :: Finished a certain level
                updateAttemptedQuestins();              
                
                //window->close();
                DivideLevel();
                return;
            }

            cout << playerPosition.x;

            
            Time elapsed = clock.restart();
            timeLeft -= elapsed.asSeconds();

            if (timeLeft <= 0) {
                updateAttemptedQuestins();                      //Which level did I attempt.
                DivideLevel();
            }

            movePlayer(event, playerSprite, playerWidth);  //Handles cycling through the animations and the playere hitting the answer.
                     //For each frame, display time left. 

        }
    }

    void updateAttemptedQuestins() {            //Which level did I attempt; Let's keep that on record. 
        if (currentLevel == 1) {
            attemptedLevel1 = true;
        }
        else if (currentLevel == 2) {
            attemptedLevel2 = true;
        }
        else if (currentLevel == 3) {
            attemptedLevel3 = true;
        }
    }


    void showleaderboard() {
        //Music for leaderboard
        //Cool animations


    }

    //Updates currentquestion. Pops question out of question stack. 
    void loadNextQuestion() {
        //Get question at the top of the stack. 
        //Remove element from questionStack.
        currentQuestion = questionStack.top();
        isMultipleChoice = currentQuestion.isMultipleChoice();
        if (questionStack.size() > 1) {

            questionStack.pop();

        }
        else {
            cout << "No more question";
            //Show review
        }


        Font font;
        font.loadFromFile("Pinky Love.ttf");


        //Update Text elements to be shown on screen.
        question.setString(currentQuestion.getQuestion());
        cout << currentQuestion.isMCQ << "IVE REACHED HERE";

        // return;

        if (isMultipleChoice) {
            //Create MCQ text
            option1.setString(currentQuestion.getOption1());
            option2.setString(currentQuestion.getOption1());
            option3.setString(currentQuestion.getOption1());
        }
        else {
            //Create ToF text
            tTrue.setString("True");
            tFalse.setString("False");
        }



        //Set character size of question and options
        setCharSize(question, 0);
        setCharSize(option1, 60);
        setCharSize(option2, 60);
        setCharSize(option3, 60);
        setCharSize(tTrue, 60);
        setCharSize(tFalse, 60);

        setScreenFont(font);
        changeTextCol(Color::White);




    }


    //Show question elements on screen
    void drawElements(RenderWindow& win)
    {

        Font font;

        font.loadFromFile("Pinky Love.ttf");
        question.setString(currentQuestion.getQuestion());
        //question.setString(to_string(playerPosition.x));
        question.setFont(font);
        question.setCharacterSize(27); 
        question.setFillColor(Color::White);
        question.setOutlineColor(Color::Black);



        tScore.setString("Score:"+to_string((int)scoreCounter));            //Load score to Score Text
        ///tScore.setString(to_string(playerPosition.x));
        tScore.setFont(font);
        tScore.setCharacterSize(50);

        
        tClock.setString("Time left: "+to_string((int)(timeLeft)));
        tClock.setFont(font);
        tClock.setCharacterSize(50);
        tClock.setFillColor(Color::Red);


        option1.setString(currentQuestion.getOption1());
        option1.setFont(font);
        option1.setCharacterSize(43);


        option2.setString(currentQuestion.getOption2());
        option2.setFont(font);
        option2.setCharacterSize(43);

        option3.setString(currentQuestion.getOption3());
        option3.setFont(font);
        option3.setCharacterSize(43);

        tTrue.setString("True");
        tTrue.setFont(font);
        tTrue.setCharacterSize(43);

        tFalse.setString("False");
        tFalse.setFont(font);
        tFalse.setCharacterSize(43);

        isMultipleChoice = currentQuestion.isMCQ;
        setQuesitonElementsPosition(win.getSize(), question, option1, option2, option3, tTrue, tFalse);

        option1.setFillColor(Color::Black);
        option2.setFillColor(Color::Black);
        option3.setFillColor(Color::Black);
        tTrue.setFillColor(Color::Black);
        tFalse.setFillColor(Color::Black);
        tScore.setFillColor(Color::White);
        

        win.draw(question);
        window->draw(tClock);
        //Make it look nice by adding shape to option

        if (currentQuestion.isMultipleChoice()) {
            win.draw(option1);
            win.draw(option2);
            win.draw(option3);
        }
        else {
            win.draw(tTrue);
            win.draw(tFalse);
        }

        window->draw(tScore);
        

    }


    void standingAnim(int& ithStandingSprite, float& width) {
        backg.setFillColor(Color::White);
        backg.setSize(Vector2f(window->getSize().x, window->getSize().y));
        backg.setPosition(0, window->getSize().y - window->getSize().y / 3);

        //Make this into a function(Loads the prite every frame (Full code here, repeeating parts commented)
           //bacground.loadFromFile("Bubba the Cave Duck.png");
           // shape.setTexture(bacground);

        canHit = false;                   //If I'm currently standing, I cannot hit : Similar to semaphores in 313


        int widthDiff = 141;
        width = widthDiff;                 //Update the global variable-Width
        int heightDiff = 307 - 203;
        // int ithSprite = 1;             //Keeps track of the current sprite in our animation sprite. 



         //This is the position of the top-left pixel in our sprite-map?
        Vector2i currPos = { 1,203 };
        currPos += {ithStandingSprite* widthDiff, 0};


        //No special case here. The images are in a horizontal line
        if (ithStandingSprite == 7) {
            currPos = { 1,203 };    //Go back to the first image
            cout << ithStandingSprite << endl;
            ithStandingSprite = 1;
        }
        else {
            ithStandingSprite++;
        }
        IntRect rect(currPos, { widthDiff - 1, heightDiff });
        playerSprite.setTextureRect(rect);

        //Make the player look in the right direction. This will set the scale as well
        faceCorrectDirection(playerSprite, width);
        cout << isLookingRight;

        window->clear();
        window->draw(backg);   //Draw the background before the sprite. 
        //Draw each element of the current question gameobject.
        drawElements(*window);
        window->draw(playerSprite);
        window->display();
    }


    void walkAnim(int& ithSprite, float& width) {

        //Make this into a function(Loads the prite every frame (Full code here, repeeating parts commented)
           //bacground.loadFromFile("Bubba the Cave Duck.png");
           // shape.setTexture(bacground);


        int widthDiff = 123;
        width = widthDiff;                 //Update the global variable-Width
        int heightDiff = 111;


        canHit = false;                  //Because I'm walking, I cannot hit.


        //This is the position of the top-left pixel in our sprite-map?
        Vector2i currPos = { 1,310 };
        currPos += {ithSprite* widthDiff, 0};


        //Handle special case (The sprite below the other ones)
        if (ithSprite == 11) {
            currPos = { 1, 424 };
            cout << ithSprite << endl;
            ithSprite = 1;
        }
        else {
            ithSprite++;
        }
        IntRect rect(currPos, { widthDiff - 1, heightDiff });
        playerSprite.setTextureRect(rect);

        //Make the player look in the right direction. This will set the scale as well
        faceCorrectDirection(playerSprite, width);
        cout << isLookingRight;

        window->clear();
        window->draw(backg);                      //Draw the background before the sprite. 
        drawElements(*window);                          //Draw each element of the current question gameobject.
        window->draw(playerSprite);
        window->display();

    }


    //For simplicity, the player can only move from left to right. 
    void movePlayer(Event event, Sprite player, float playerWidth) {

        //The player moves from left to right and smacks the correct answer.


        if (event.type == Event::KeyPressed) {

            //If I'm walking to then process the walking animation and return. 
            //The program won't run the method for the standing animation.

            if (event.key.code == Keyboard::Left) {
                playerPosition = Vector2f(playerPosition.x - spd, playerPosition.y);
                isLookingRight = false;
                cout << "Moving to left";

                //  standingAnim(ithWalkingSprite, playerWidth);
                //  smackSprite(ithSmackingSprite, playerWidth);
                walkAnim(ithWalkingSprite, playerWidth);
                return;
            }

            if (event.key.code == Keyboard::Right) {
                playerPosition = Vector2f(playerPosition.x + spd, playerPosition.y);
                cout << "Moving to Right";
                isLookingRight = true;
                walkAnim(ithWalkingSprite, playerWidth);
                return;
            }


            if (event.key.code == Keyboard::Down) {
                cout << "Hitting";
                canHit = true;
                //lOGIC FOR DETECTING THE SELECTED ANSWER (USE PLAYER POSITION AND RANGE)
            }

        }

        if (canHit) {

            //Figure it out
            cout << "Smacking";
            hitAnimation(ithHittingSprite, playerWidth);
            return;
        }

        standingAnim(ithStandingSprite, playerWidth);
    }


    void faceCorrectDirection(Sprite& player, float& width) {        //What is the width of your sprite? 
        //Let's make your player face the correct direction
        if (!isLookingRight) {

            if (canHit) {
                //Subtract 50 from the position of the player on the y axis. Eliminating the jump effect. 
                player.setScale({ -playerScale,playerScale });
                player.setPosition(playerPosition + Vector2f(width * playerScale, -50));
            }
            else {
                player.setScale({ -playerScale,playerScale });
                player.setPosition(playerPosition + Vector2f(width * playerScale, 0));
            }

        }
        else {
            if (canHit) {
                player.setScale({ playerScale , playerScale });
                player.setPosition(playerPosition - Vector2f(width / 3, 50));         //Adjust position when player gets back to facing right (Accounting for hitting animation)
            }
            else {
                player.setScale({ playerScale , playerScale });
                player.setPosition(playerPosition);                                 //Adjust position when player gets back to facing right
            }

        }
    }


    //Plays the sequence of images for the hitting animation. 
    void hitAnimation(int& ithHittingSprite, float& width) {

        //Make this into a function(Loads the prite every frame (Full code here, repeeating parts commented)
           //bacground.loadFromFile("Bubba the Cave Duck.png");
           // shape.setTexture(bacground);
        int widthDiff = 185;
        width = widthDiff;
        int heightDiff = 142;

        tempPosition = playerPosition;




        Vector2i currPos;

        //Selecting the current sprite frame to be selected from spritemap.
        if (ithHittingSprite < 6) {
            currPos = { 1, 537 };
            currPos += {ithHittingSprite* widthDiff, 0};
        }
        else {
            //Position of sprites below the current horizontal line
            currPos = { 1, 681 };
            currPos += {(ithHittingSprite - 6)* widthDiff, 0};
        }

        ithHittingSprite++;

        faceCorrectDirection(playerSprite, width);
        // cout << isLookingRight;
        //cout << ithSmackingSprite;


        //Reset the ith Smacking sprite (Done hitting)
        if (ithHittingSprite > 8) {
            cout << "Here" << canHit;
            ithHittingSprite = 0;
            canHit = false;       //The player is done hitting.
            //Detect which button hit. Depending on type isMCQ.
            detectOptionHit();
            return;
        }

        IntRect rect(currPos, { widthDiff - 1, heightDiff });
        playerSprite.setTextureRect(rect);
        //playerSprite.setPosition(playerPosition + Vector2f(0, 600));   //Line is irrelevant
        faceCorrectDirection(playerSprite, width);

        window->clear();
        window->draw(backg);
        window->draw(playerSprite);
        drawElements(*window);
        window->display();

    }

    void detectOptionHit() {

        GameQuestion& curr = currentQuestion;
        int x = playerPosition.x;                   //x position of player. 
        cout << currentQuestion.isMCQ;
        if (currentQuestion.isMCQ) {                //MCQ Question
            if ( x < 140) {
                cout << "Option1";
                if (curr.getAnswer() == curr.getOption1()) {
                    //Correct!
                    incrementScore();
                }
                loadNextQuestion();
            }
            else if (x > 520 && x < 780) {
                cout << "Option2";
                if (curr.getAnswer() == curr.getOption2()) {
                    //Correct!
                    incrementScore();
                }
                loadNextQuestion();
            }
            else if (x > 960 && x < 1260) {
                cout << "Option3";
                if (curr.getAnswer() == curr.getOption3()) {
                    //Correct!
                    incrementScore();
                }
                loadNextQuestion();
            }
            
        }
        else {                                      //TrueFalse Question   
            if (x > 220 && x < 460) {
                //True
                cout << "True";
                loadNextQuestion();
            }
            else if (x > 840 && x < 1080) {
                //False
                cout << "False";
                loadNextQuestion();
            }

        }


    }

    void incrementScore() {
        if (currentLevel == 1) {        //Level1 counts 5 marks per correct answer
            scoreCounter += 5;
        }
        else if (currentLevel == 2) {
            scoreCounter += 8;
        }
        else {
            scoreCounter += 10;
        }
    }


    //Level Selection Methods
    void DivideLevel() {
       
        
        backgroundt.loadFromFile("Background1.jpeg");
        backgroundSprite = Sprite(backgroundt);

        Vector2u windowSize = window->getSize();
        backgroundSprite.setScale(float(windowSize.x) / backgroundt.getSize().x, float(windowSize.y) / backgroundt.getSize().y);
        
        Font bfont;
        bfont.loadFromFile("Roboto-BoldItalic.ttf");
        

        if (!afont.loadFromFile("Roman SD.ttf")) {
            cout << "Failed";

        }

        Button LevelOne(Color::Blue, 28, Color::White, "Level 1", Vector2f(250, 80), bfont);
        LevelOne.setPosition({ 100,450 });
        Button LevelTwo(Color::Blue, 28, Color::White, "Level 2", Vector2f(250, 80), bfont);
        LevelTwo.setPosition({ 550,450 });
        Button LevelThree(Color::Blue, 28, Color::White, "Level 3", Vector2f(250, 80), bfont);
        LevelThree.setPosition({ 100, 600 });
        Button FinAttempt(Color::Blue, 28, Color::White, "Finish Attempt", Vector2f(250, 80), bfont);
        FinAttempt.setPosition({ 550, 600 });


        string ChooseText = "Please select the level of difficulty you want to play\nor Click Finish attempt after attempting atleast one Level.";
        Quest.setString(ChooseText);
        Quest.setCharacterSize(30);
        Quest.setOutlineThickness(6);
        Quest.setFont(afont);
        Quest.setFillColor(Color::White);
        Quest.setPosition(100, 100);



        while (window->isOpen()) {
            // Check if the any level is selected and not show instructions again
            if (!isLevelSelected) {
                instructions(afont);
                isLevelSelected = true;
            }

            Event event;
            while (window->pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window->close();
                }
                if (LevelOne.isMouseOver(*window)) {
                    LevelOne.changeBackCol(Color::Green);
                }
                else {
                    LevelOne.changeBackCol(Color::Blue);

                }
                if (LevelTwo.isMouseOver(*window)) {
                    LevelTwo.changeBackCol(Color::Green);
                }
                else {
                    LevelTwo.changeBackCol(Color::Blue);

                }
                if (LevelThree.isMouseOver(*window)) {
                    LevelThree.changeBackCol(Color::Green);
                }
                else {
                    LevelThree.changeBackCol(Color::Blue);

                }if (FinAttempt.isMouseOver(*window)) {
                    FinAttempt.changeBackCol(Color::Green);
                }
                else {
                    FinAttempt.changeBackCol(Color::Blue);

                }

                if (LevelOne.clicked(*window, event)) {
                    //Load level 1
                    run(1);
                }
                else if (LevelTwo.clicked(*window, event)) {
                    //Load level 2
                    run(2);
                }
                else if (LevelThree.clicked(*window, event)) {
                    //Load level 3
                    run(3);
                }
                else if (FinAttempt.clicked(*window, event)&&(attemptedLevel1&&attemptedLevel2&&attemptedLevel3)) {
                    //Display feedback screen
                    window->clear();
                    feedback f(playerName);
                    cout.clear();
                   

                    f.displayFeed(scoreCounter, window);
                    myWindow();
                    return;
                }

            }
            window->clear();
            window->draw(backgroundSprite);
            window->draw(Quest);
            LevelOne.drawTo(*window);
            LevelTwo.drawTo(*window);
            LevelThree.drawTo(*window);
            FinAttempt.drawTo(*window);
            window->display();


        }
    }


    void instructions(Font& font) {

        Texture bg;
        bg.loadFromFile("M10.jpeg");
        Sprite bgSprite(bg);
        Vector2u windowSize = window->getSize();
        bgSprite.setScale(float(windowSize.x) / bg.getSize().x, float(windowSize.y) / bg.getSize().y);
        Button proceed(Color::Blue, 40, Color::White, "Proceed", Vector2f(350, 100), font);
        proceed.setPosition({300,600 });


        string Instr = "\t\t\t\t\tGame Instructions :\n\n\n You control your character using the arrow keys: \n\t\t\tPress the down key to select options.\n\t\t\tPress left arrow key to move left.\n\t\t\tPress right arrow key to move right. ";
        Instructions.setString(Instr);
        Instructions.setCharacterSize(24);
        Instructions.setOutlineThickness(6);
        Instructions.setFont(font);
        Instructions.setFillColor(Color::White);
        Instructions.setPosition(30, 260);

        bool isBreak = false;
        while (window->isOpen()) {
            Event event;
            while (window->pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window->close();
                }
                if (proceed.isMouseOver(*window)) {
                    proceed.changeBackCol(Color::Green);
                    if (proceed.clicked(*window, event)) {
                        isBreak = true;
                        break;
                    }
                }
                else {
                    proceed.changeBackCol(Color::Blue);
                }
            }
            window->clear();
            window->draw(bgSprite);
            window->draw(Instructions);
            proceed.drawTo(*window);
            window->display();

            if (isBreak)
                break;
        }
    }


    //Main Menu Methods
    void myWindow() {
        SoundBuffer buffer;
        if (!buffer.loadFromFile("MainMenu1.mp3")) {
            return;
        }
        sound = Sound(buffer);
        sound.setVolume(600);

        font = new Font();

        if (attemptedLevel1 || attemptedLevel2 || attemptedLevel3) {
            //Do nothing
        }
        else {
           
            window = new RenderWindow(VideoMode(1336, 768), "Game", Style::Close);
            font->loadFromFile("Roboto-BoldItalic.ttf");
            addMusic("uefa.ogg", "Fifa7.jpg");//Playing the loading image
        }

        //Creating my Sprite
        winClose = new RectangleShape();
        image = new Texture();
        menuBackground = new Sprite();
        buttonOne = new Button();
        buttonTwo = new Button();
        buttonThree = new Button();
        playButton = new Button();
        nameInput = new TextBox(Color::Green, 30, true, "");

        image->loadFromFile("Fifa8.jpg");
        menuBackground->setTexture(*image);

        Texture backG;
        backG.loadFromFile("loading2.jpg");
        Sprite tempImage(backG);
        //Sound for the main menu
      
        //Setting the size of the Sprite to equal that of the window
        Vector2u windowSize = window->getSize();
        float scaleX = static_cast<float>(windowSize.x) / menuBackground->getLocalBounds().width;
        float scaleY = static_cast<float>(windowSize.y) / menuBackground->getLocalBounds().height;
        menuBackground->setScale(scaleX, scaleY);
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
        buttonOne.setPosition(Vector2f(xCo, yCo - 100));
        Button buttonTwo(Color::Blue, 30, Color::White, "About", Vector2f(180.0f, 60.0f), *font);
        buttonTwo.setPosition(Vector2f(xCo, yCo));
        Button buttonThree(Color::Blue, 30, Color::White, "Quit", Vector2f(180.0f, 60.0f), *font);
        buttonThree.setPosition(Vector2f(xCo, yCo + 100));

       
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
            window->draw(*menuBackground);
            buttonOne.drawTo(*window);
            buttonTwo.drawTo(*window);
            buttonThree.drawTo(*window);
            window->display();
        }
    }
    void about() {
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
    void addMusic(String str, string pic) {

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
            text.setPosition((windowSize.x) - 250, (windowSize.y) - 100);

            float scaleX = static_cast<float>(windowSize.x) / backG.getLocalBounds().width;
            float scaleY = static_cast<float>(windowSize.y) / backG.getLocalBounds().height;
            backG.setScale(scaleX, scaleY);
            sound.play();

            while (sound.getStatus() == Sound::Playing) {
                window->draw(backG);
                window->draw(text);
                window->display();
                sleep(sf::milliseconds(1));
            }
        }
    }
    void login() {//Text writting functions
        image->loadFromFile("loading1.jpg");
        Sprite backG(*image);
        Vector2u windowSize = window->getSize();//Duplicate 

        float scaleX = static_cast<float>(windowSize.x) / backG.getLocalBounds().width;
        float scaleY = static_cast<float>(windowSize.y) / backG.getLocalBounds().height;
        backG.setScale(scaleX, scaleY);

        Button playButton(Color::Blue, 30, Color::White, "Launch>>", Vector2f(180.0f, 60.0f), *font);
        playButton.setPosition(Vector2f(windowSize.x - 220, windowSize.y - 100));
        nameInput->userInput(*window, backG, playButton);
        DivideLevel();
    }
};




