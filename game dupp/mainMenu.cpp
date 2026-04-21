#include "MainMenu.h"

Menu::Menu(float width, float height)
{
    windowWidth = width;
    windowHeight = height;
    windowSize = Vector2u((unsigned)width, (unsigned)height);
    currentGame = nullptr;                     // ADDED

    currentState = MAIN_MENU;
    isDraggingKnob = false;
    volumeLevel = 50.0f;
    selectedLevel = -1;

    if (!font.loadFromFile("assets/ARCADE.ttf"))
        cout << "Error loading font!" << endl;

    if (music.openFromFile("assets/sound/mixkit-love-787.mp3"))
    {
        music.setLoop(true);
        music.setVolume(volumeLevel);
        music.play();
    }

    if (!bgTexture1.loadFromFile("assets/bg1.png"))
        cout << "Error loading bg1.png!" << endl;
    if (!bgTexture2.loadFromFile("assets/bg2.png"))
        cout << "Error loading bg2.png!" << endl;

    backgroundMain.setTexture(bgTexture1);
    Vector2u textureSize1 = bgTexture1.getSize();
    backgroundMain.setScale(width / textureSize1.x, height / textureSize1.y);
    backgroundMain.setPosition(0, 0);

    backgroundOptions.setTexture(bgTexture2);
    Vector2u textureSize2 = bgTexture2.getSize();
    backgroundOptions.setScale(width / textureSize2.x, height / textureSize2.y);
    backgroundOptions.setPosition(0, 0);

    if (!buffer.loadFromFile("assets/sound/mixkit.wav"))
        cout << "Error loading sound!" << endl;
    sound.setBuffer(buffer);

    // MAIN MENU
    string options[3] = { "Play", "Options", "Exit" };
    float buttonWidth = width / 4;
    float buttonHeight = height / 10;

    for (int i = 0; i < 3; i++)
    {
        buttons[i].setSize(Vector2f(buttonWidth, buttonHeight));
        buttons[i].setFillColor(Color(100, 100, 100, 200));
        buttons[i].setOutlineColor(Color::White);
        buttons[i].setOutlineThickness(2);
        buttons[i].setPosition(width / 2 - buttonWidth / 2,
            height / 2 + i * (height / 6) - buttonHeight / 2);

        menu[i].setFont(font);
        menu[i].setString(options[i]);
        menu[i].setCharacterSize(height / 15);
        menu[i].setFillColor(Color::White);
        FloatRect textRect = menu[i].getLocalBounds();
        menu[i].setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        menu[i].setPosition(width / 2,
            height / 2 + i * (height / 6));
    }

    selectedIndex = 0;
    menu[selectedIndex].setFillColor(Color::Yellow);
    buttons[selectedIndex].setFillColor(Color(150, 150, 50, 200));

    // OPTIONS MENU
    optionsTitle.setFont(font);
    optionsTitle.setString("OPTIONS");
    optionsTitle.setCharacterSize(height / 10);
    optionsTitle.setFillColor(Color::Yellow);
    optionsTitle.setOutlineColor(Color::Black);
    optionsTitle.setOutlineThickness(3);
    FloatRect titleRect = optionsTitle.getLocalBounds();
    optionsTitle.setOrigin(titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f);
    optionsTitle.setPosition(width / 2, height / 6);

    volumeText.setFont(font);
    volumeText.setString("VOLUME");
    volumeText.setCharacterSize(height / 20);
    volumeText.setFillColor(Color::White);
    volumeText.setOutlineColor(Color::Black);
    volumeText.setOutlineThickness(2);
    volumeText.setPosition(width / 3, height / 3);

    volumeValue.setFont(font);
    volumeValue.setCharacterSize(height / 20);
    volumeValue.setFillColor(Color::Yellow);
    volumeValue.setOutlineColor(Color::Black);
    volumeValue.setOutlineThickness(2);
    updateVolume();

    float sliderWidth = width / 3;
    float sliderHeight = height / 30;
    volumeSlider.setSize(Vector2f(sliderWidth, sliderHeight));
    volumeSlider.setFillColor(Color(80, 80, 80, 200));
    volumeSlider.setOutlineColor(Color::White);
    volumeSlider.setOutlineThickness(2);
    volumeSlider.setPosition(width / 3, height / 3 + height / 15);

    float knobWidth = height / 20;
    float knobHeight = height / 15;
    volumeKnob.setSize(Vector2f(knobWidth, knobHeight));
    volumeKnob.setFillColor(Color::Yellow);
    volumeKnob.setOrigin(knobWidth / 2, knobHeight / 2);
    float knobX = volumeSlider.getPosition().x + (volumeLevel / 100.0f) * sliderWidth;
    volumeKnob.setPosition(knobX, volumeSlider.getPosition().y + sliderHeight / 2);

    float backWidth = width / 5;
    float backHeight = height / 12;
    backButtonRect.setSize(Vector2f(backWidth, backHeight));
    backButtonRect.setFillColor(Color(100, 100, 100, 200));
    backButtonRect.setOutlineColor(Color::White);
    backButtonRect.setOutlineThickness(2);
    backButtonRect.setPosition(width / 2 - backWidth / 2, height * 3 / 4);

    backButton.setFont(font);
    backButton.setString("BACK TO MENU");
    backButton.setCharacterSize(height / 25);
    backButton.setFillColor(Color::White);
    backButton.setOutlineColor(Color::Black);
    backButton.setOutlineThickness(2);
    FloatRect backTextRect = backButton.getLocalBounds();
    backButton.setOrigin(backTextRect.left + backTextRect.width / 2.0f,
        backTextRect.top + backTextRect.height / 2.0f);
    backButton.setPosition(width / 2, height * 3 / 4 + backHeight / 2);

    // LEVEL SELECT MENU
    levelTitle.setFont(font);
    levelTitle.setString("SELECT LEVEL");
    levelTitle.setCharacterSize(height / 10);
    levelTitle.setFillColor(Color::Yellow);
    levelTitle.setOutlineColor(Color::Black);
    levelTitle.setOutlineThickness(3);
    FloatRect levelTitleRect = levelTitle.getLocalBounds();
    levelTitle.setOrigin(levelTitleRect.left + levelTitleRect.width / 2.0f,
        levelTitleRect.top + levelTitleRect.height / 2.0f);
    levelTitle.setPosition(width / 2, height / 6);

    float boxSize = height / 3;
    float spacing = (width - (boxSize * 3)) / 4;

    if (!levelTextures[0].loadFromFile("assets/Backgrounds/PNG/summer6/Summer6.png"))
        cout << "Error loading level1.png!" << endl;
    if (!levelTextures[1].loadFromFile("assets/Backgrounds/PNG/summer5/Summer5.png"))
        cout << "Error loading level2.png!" << endl;
    if (!levelTextures[2].loadFromFile("assets/Backgrounds/PNG/summer 1/Summer1.png"))
        cout << "Error loading level3.png!" << endl;

    for (int i = 0; i < 3; i++)
    {
        levelBoxes[i].setSize(Vector2f(boxSize, boxSize));
        levelBoxes[i].setFillColor(Color(100, 100, 100, 200));
        levelBoxes[i].setOutlineColor(Color::White);
        levelBoxes[i].setOutlineThickness(3);
        levelBoxes[i].setPosition(spacing + (boxSize + spacing) * i,
            height / 2 - boxSize / 2);

        levelSprites[i].setTexture(levelTextures[i]);
        float imageSize = boxSize - 60;
        Vector2u textureSize = levelTextures[i].getSize();
        float scaleX = imageSize / textureSize.x;
        float scaleY = imageSize / textureSize.y;
        levelSprites[i].setScale(scaleX, scaleY);
        float imageX = levelBoxes[i].getPosition().x + (boxSize / 2) - (imageSize / 2);
        float imageY = levelBoxes[i].getPosition().y + (boxSize / 2) - (imageSize / 2);
        levelSprites[i].setPosition(imageX, imageY);

        levelTexts[i].setFont(font);
        string levelNum = "LEVEL " + to_string(i + 1);
        levelTexts[i].setString(levelNum);
        levelTexts[i].setCharacterSize(height / 25);
        levelTexts[i].setFillColor(Color::White);
        levelTexts[i].setOutlineColor(Color::Black);
        levelTexts[i].setOutlineThickness(2);
        FloatRect textRect = levelTexts[i].getLocalBounds();
        levelTexts[i].setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        levelTexts[i].setPosition(spacing + (boxSize + spacing) * i + boxSize / 2,
            height / 2 + boxSize / 2 + 40);

        levelHover[i] = false;
    }

    levelBackButton.setSize(Vector2f(backWidth, backHeight));
    levelBackButton.setFillColor(Color(100, 100, 100, 200));
    levelBackButton.setOutlineColor(Color::White);
    levelBackButton.setOutlineThickness(2);
    levelBackButton.setPosition(width / 2 - backWidth / 2, height * 3 / 4);

    levelBackText.setFont(font);
    levelBackText.setString("BACK TO MENU");
    levelBackText.setCharacterSize(height / 25);
    levelBackText.setFillColor(Color::White);
    levelBackText.setOutlineColor(Color::Black);
    levelBackText.setOutlineThickness(2);
    backTextRect = levelBackText.getLocalBounds();
    levelBackText.setOrigin(backTextRect.left + backTextRect.width / 2.0f,
        backTextRect.top + backTextRect.height / 2.0f);
    levelBackText.setPosition(width / 2, height * 3 / 4 + backHeight / 2);

    // REMOVED test player and camera setup
}

Menu::~Menu()   // ADDED destructor
{
    delete currentGame;
}

void Menu::updateVolume()
{
    stringstream ss;
    ss << fixed << setprecision(0) << volumeLevel << "%";
    volumeValue.setString(ss.str());
    FloatRect valueRect = volumeValue.getLocalBounds();
    volumeValue.setPosition(volumeSlider.getPosition().x + volumeSlider.getSize().x + 20,
        volumeSlider.getPosition().y + volumeSlider.getSize().y / 2 - valueRect.height / 2);
    music.setVolume(volumeLevel);
}

void Menu::update(float deltaTime)
{
    if (currentState == GAME_PLAY && currentGame)
    {
        currentGame->update();
        if (currentGame->isGameOver())
        {
            // Game over, return to level select
            currentState = LEVEL_SELECT;
            delete currentGame;
            currentGame = nullptr;
            sound.play(); // optional feedback
        }
    }
}

void Menu::moveUp()
{
    if (currentState == MAIN_MENU)
    {
        menu[selectedIndex].setFillColor(Color::White);
        buttons[selectedIndex].setFillColor(Color(100, 100, 100, 200));
        selectedIndex = (selectedIndex - 1 + 3) % 3;
        menu[selectedIndex].setFillColor(Color::Yellow);
        buttons[selectedIndex].setFillColor(Color(150, 150, 50, 200));
        sound.play();
    }
}

void Menu::moveDown()
{
    if (currentState == MAIN_MENU)
    {
        menu[selectedIndex].setFillColor(Color::White);
        buttons[selectedIndex].setFillColor(Color(100, 100, 100, 200));
        selectedIndex = (selectedIndex + 1) % 3;
        menu[selectedIndex].setFillColor(Color::Yellow);
        buttons[selectedIndex].setFillColor(Color(150, 150, 50, 200));
        sound.play();
    }
}

int Menu::getSelectedIndex() { return selectedIndex; }

void Menu::handleMouseMainMenu(RenderWindow& window)
{
    Vector2i mousePos = Mouse::getPosition(window);
    Vector2f worldPos = window.mapPixelToCoords(mousePos);
    for (int i = 0; i < 3; i++)
    {
        if (buttons[i].getGlobalBounds().contains(worldPos))
        {
            if (selectedIndex != i)
            {
                menu[selectedIndex].setFillColor(Color::White);
                buttons[selectedIndex].setFillColor(Color(100, 100, 100, 200));
                selectedIndex = i;
                menu[selectedIndex].setFillColor(Color::Yellow);
                buttons[selectedIndex].setFillColor(Color(150, 150, 50, 200));
                sound.play();
            }
            break;
        }
    }
}

void Menu::handleMouseOptions(RenderWindow& window, Event& event)
{
    Vector2i mousePos = Mouse::getPosition(window);
    Vector2f worldPos = window.mapPixelToCoords(mousePos);

    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
    {
        if (volumeKnob.getGlobalBounds().contains(worldPos))
            isDraggingKnob = true;
        else if (backButtonRect.getGlobalBounds().contains(worldPos))
        {
            sound.play();
            currentState = MAIN_MENU;
        }
    }

    if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
        isDraggingKnob = false;

    if (isDraggingKnob || (event.type == Event::MouseMoved && Mouse::isButtonPressed(Mouse::Left) && isDraggingKnob))
    {
        float sliderLeft = volumeSlider.getPosition().x;
        float sliderRight = sliderLeft + volumeSlider.getSize().x;
        float knobX = worldPos.x;
        if (knobX < sliderLeft) knobX = sliderLeft;
        if (knobX > sliderRight) knobX = sliderRight;
        float percentage = (knobX - sliderLeft) / volumeSlider.getSize().x;
        volumeLevel = percentage * 100.0f;
        volumeKnob.setPosition(knobX, volumeKnob.getPosition().y);
        updateVolume();
    }
}

void Menu::handleInput(RenderWindow& window, Event& event)
{
    if (currentState == MAIN_MENU)
    {
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Up) moveUp();
            if (event.key.code == Keyboard::Down) moveDown();
            if (event.key.code == Keyboard::Enter)
            {
                int selected = getSelectedIndex();
                if (selected == 0)
                {
                    sound.play();
                    currentState = LEVEL_SELECT;
                }
                else if (selected == 1)
                {
                    sound.play();
                    currentState = OPTIONS_MENU;
                }
                else if (selected == 2)
                    window.close();
            }
        }

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        {
            Vector2i mousePos = Mouse::getPosition(window);
            Vector2f worldPos = window.mapPixelToCoords(mousePos);
            for (int i = 0; i < 3; i++)
            {
                if (buttons[i].getGlobalBounds().contains(worldPos))
                {
                    if (i == 0)
                    {
                        sound.play();
                        currentState = LEVEL_SELECT;
                    }
                    else if (i == 1)
                    {
                        sound.play();
                        currentState = OPTIONS_MENU;
                    }
                    else if (i == 2)
                        window.close();
                    break;
                }
            }
        }
        handleMouseMainMenu(window);
    }
    else if (currentState == OPTIONS_MENU)
    {
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Left)
            {
                volumeLevel -= 5;
                if (volumeLevel < 0) volumeLevel = 0;
                float knobX = volumeSlider.getPosition().x + (volumeLevel / 100.0f) * volumeSlider.getSize().x;
                volumeKnob.setPosition(knobX, volumeKnob.getPosition().y);
                updateVolume();
                sound.play();
            }
            else if (event.key.code == Keyboard::Right)
            {
                volumeLevel += 5;
                if (volumeLevel > 100) volumeLevel = 100;
                float knobX = volumeSlider.getPosition().x + (volumeLevel / 100.0f) * volumeSlider.getSize().x;
                volumeKnob.setPosition(knobX, volumeKnob.getPosition().y);
                updateVolume();
                sound.play();
            }
            else if (event.key.code == Keyboard::Escape)
            {
                sound.play();
                currentState = MAIN_MENU;
            }
        }
        handleMouseOptions(window, event);
    }
    else if (currentState == LEVEL_SELECT)
    {
        Vector2i mousePos = Mouse::getPosition(window);
        Vector2f worldPos = window.mapPixelToCoords(mousePos);

        for (int i = 0; i < 3; i++)
        {
            if (levelBoxes[i].getGlobalBounds().contains(worldPos))
            {
                if (!levelHover[i])
                {
                    levelBoxes[i].setFillColor(Color(150, 150, 50, 200));
                    levelBoxes[i].setOutlineColor(Color::Yellow);
                    levelHover[i] = true;
                }
            }
            else
            {
                if (levelHover[i])
                {
                    levelBoxes[i].setFillColor(Color(100, 100, 100, 200));
                    levelBoxes[i].setOutlineColor(Color::White);
                    levelHover[i] = false;
                }
            }
        }

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        {
            for (int i = 0; i < 3; i++)
            {
                if (levelBoxes[i].getGlobalBounds().contains(worldPos))
                {
                    sound.play();
                    selectedLevel = i + 1;
                    cout << "Starting LEVEL " << selectedLevel << "!" << endl;

                    // Create new game
                    delete currentGame;
                    currentGame = new GamePlay(windowSize, selectedLevel);
                    currentState = GAME_PLAY;
                    break;
                }
            }
            if (levelBackButton.getGlobalBounds().contains(worldPos))
            {
                sound.play();
                currentState = MAIN_MENU;
            }
        }

        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
        {
            sound.play();
            currentState = MAIN_MENU;
        }
    }
    else if (currentState == GAME_PLAY)
    {
        // Optionally handle Escape to go back to level select
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
        {
            sound.play();
            currentState = LEVEL_SELECT;
            delete currentGame;
            currentGame = nullptr;
        }
        // All other input is handled inside GamePlay::update() via player class
    }
}

void Menu::draw(RenderWindow& window)
{
    if (currentState == MAIN_MENU)
    {
        window.draw(backgroundMain);
        for (int i = 0; i < 3; i++)
        {
            window.draw(buttons[i]);
            window.draw(menu[i]);
        }
    }
    else if (currentState == OPTIONS_MENU)
    {
        window.draw(backgroundMain);
        window.draw(backgroundOptions);
        window.draw(optionsTitle);
        window.draw(volumeText);
        window.draw(volumeValue);
        window.draw(volumeSlider);
        window.draw(volumeKnob);
        window.draw(backButtonRect);
        window.draw(backButton);
    }
    else if (currentState == LEVEL_SELECT)
    {
        window.draw(backgroundMain);
        window.draw(backgroundOptions);
        window.draw(levelTitle);
        for (int i = 0; i < 3; i++)
        {
            window.draw(levelBoxes[i]);
            window.draw(levelSprites[i]);
            window.draw(levelTexts[i]);
        }
        window.draw(levelBackButton);
        window.draw(levelBackText);
    }
    else if (currentState == GAME_PLAY && currentGame)
    {
        currentGame->render(window);
    }
}