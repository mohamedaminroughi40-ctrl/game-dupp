#ifndef MAINMENU_H
#define MAINMENU_H

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<sstream>
#include<iomanip>
#include "game.h"          // ADDED

using namespace sf;
using namespace std;

class Menu
{
private:
    enum GameState { MAIN_MENU, OPTIONS_MENU, LEVEL_SELECT, GAME_PLAY };
    GameState currentState;

    Font font;
    Text menu[3];
    RectangleShape buttons[3];
    int selectedIndex;

    Text optionsTitle;
    Text volumeText;
    Text volumeValue;
    Text backButton;
    RectangleShape volumeSlider;
    RectangleShape volumeKnob;
    RectangleShape backButtonRect;
    bool isDraggingKnob;

    Text levelTitle;
    RectangleShape levelBoxes[3];
    Text levelTexts[3];
    bool levelHover[3];
    int selectedLevel;
    RectangleShape levelBackButton;
    Text levelBackText;

    // REMOVED testPlayer, gameView, etc. – now using GamePlay
    GamePlay* currentGame;      // ADDED
    Vector2u windowSize;        // ADDED – store window size for game creation

    float windowWidth, windowHeight;

    Texture levelTextures[3];
    Sprite levelSprites[3];

    float volumeLevel;

    Texture bgTexture1;
    Texture bgTexture2;
    Sprite backgroundMain;
    Sprite backgroundOptions;

    SoundBuffer buffer;
    Sound sound;
    Music music;

public:
    Menu(float width, float height);
    ~Menu();   // ADDED destructor to delete currentGame

    void draw(RenderWindow& window);
    void handleInput(RenderWindow& window, Event& event);

    void moveUp();
    void moveDown();
    int getSelectedIndex();
    void handleMouseMainMenu(RenderWindow& window);
    void handleMouseOptions(RenderWindow& window, Event& event);
    void updateVolume();
    void update(float deltaTime);   // will call game update if needed
};

#endif