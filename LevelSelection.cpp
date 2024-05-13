class LevelSelection {
private:
    RenderWindow window;

public:
    Font font;
    Font afont;
    Text Quest;
    Text Instructions;
    Texture background;
    Texture bg;
    Sprite backgroundSprite;

    // Button button; 


    LevelSelection() : window(sf::VideoMode(800, 600), "Quiz Game", Style::Fullscreen)
    {
        if (!afont.loadFromFile("Roman SD.ttf"))
        {
            std::cout << "Failed to load font" << std::endl;
        }
    }


    void DivideLevel(int& Level) {

        background.loadFromFile("Background1.jpeg");
        backgroundSprite = Sprite(background);

        Vector2u windowSize = window.getSize();
        backgroundSprite.setScale(float(windowSize.x) / background.getSize().x, float(windowSize.y) / background.getSize().y);


        if (!afont.loadFromFile("Roman SD.ttf")) {
            cout << "Failed";

        }

        Button LevelOne(Color::Blue, 40, Color::White, "Level 1", Vector2f(350, 120), afont);
        LevelOne.setPosition({ 200,450 });
        Button LevelTwo(Color::Blue, 40, Color::White, "Level 2", Vector2f(350, 120), afont);
        LevelTwo.setPosition({ 750,450 });
        Button LevelThree(Color::Blue, 40, Color::White, "Level 3", Vector2f(350, 120), afont);
        LevelThree.setPosition({ 200, 650 });
        Button FinAttempt(Color::Blue, 40, Color::White, "Finish Attempt", Vector2f(350, 120), afont);
        FinAttempt.setPosition({ 750, 650 });


        string ChooseText = "Please select the level of difficulty you want to play\nor Click Finish attempt after attempting atleast one Level.";
        Quest.setString(ChooseText);
        Quest.setCharacterSize(40);
        Quest.setOutlineThickness(6);
        Quest.setFont(font);
        Quest.setFillColor(Color::White);
        Quest.setPosition(100, 250);

        bool isLevelSelected = false;

        while (window.isOpen()) {
            if (!isLevelSelected) {
                instructions(afont);
                isLevelSelected = true;
            }

            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                if (LevelOne.isMouseOver(window)) {
                    LevelOne.changeBackCol(Color::Green);
                }
                else {
                    LevelOne.changeBackCol(Color::Blue);

                }
                if (LevelTwo.isMouseOver(window)) {
                    LevelTwo.changeBackCol(Color::Green);
                }
                else {
                    LevelTwo.changeBackCol(Color::Blue);

                }
                if (LevelThree.isMouseOver(window)) {
                    LevelThree.changeBackCol(Color::Green);
                }
                else {
                    LevelThree.changeBackCol(Color::Blue);

                }if (FinAttempt.isMouseOver(window)) {
                    FinAttempt.changeBackCol(Color::Green);
                }
                else {
                    FinAttempt.changeBackCol(Color::Blue);

                }

                if (LevelOne.clicked(window, event)) {
                    window.close();
                    Level = 1;
                }
                else if (LevelTwo.clicked(window, event)) {
                    window.close();
                    Level = 2;
                }
                else if (LevelThree.clicked(window, event)) {
                    window.close();
                    Level = 3;
                }
                else if (FinAttempt.clicked(window, event)) {
                    window.close();
                    Level = 3;
                }

            }
            window.clear();
            window.draw(backgroundSprite);
            window.draw(Quest);
            LevelOne.drawTo(window);
            LevelTwo.drawTo(window);
            LevelThree.drawTo(window);
            FinAttempt.drawTo(window);
            window.display();


        }
    }

    void instructions(Font& font) {

        Texture bg;
        bg.loadFromFile("M10.jpeg");
        Sprite bgSprite(bg);
        Vector2u windowSize = window.getSize();
        bgSprite.setScale(float(windowSize.x) / bg.getSize().x, float(windowSize.y) / bg.getSize().y);

        Button proceed(Color::Blue, 40, Color::White, "Proceed", Vector2f(350, 100), font);
        proceed.setPosition({ 400,800 });


        string Instr = "\t\t\t\t\t\t\tGame Instructions :\n\n\n You control your character using the arrow keys: \n\t\t\tPress the down key to select options.\n\t\t\tPress left arrow key to move left.\n\t\t\tPress right arrow key to move right. ";
        Instructions.setString(Instr);
        Instructions.setCharacterSize(50);
        Instructions.setOutlineThickness(6);
        Instructions.setFont(font);
        Instructions.setFillColor(Color::White);
        Instructions.setPosition(30, 360);

        bool isBreak = false;
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                if (proceed.isMouseOver(window)) {
                    proceed.changeBackCol(Color::Green);
                    if (proceed.clicked(window, event)) {
                        isBreak = true;
                        break;
                    }
                }
                else {
                    proceed.changeBackCol(Color::Blue);
                }
            }
            window.clear();
            window.draw(bgSprite);
            window.draw(Instructions);
            proceed.drawTo(window);
            window.display();

            if (isBreak)
                break;
        }
    }
};