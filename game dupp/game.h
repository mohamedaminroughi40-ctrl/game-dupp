#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <sstream>
#include "player.h"
#include "enemie.h"
#include "boss.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class GamePlay
{
private:
    Texture backgroundTex1;
    Texture backgroundTex2;
    Sprite background1;
    Sprite background2;
    void initBackground(Vector2u winSize, int level);

    player P;
    boss Mbooss;
    std::vector<enemie> enemies;

    int currentLevel;
    Vector2u windowSize;

    // Score system
    int score;
    Font scoreFont;
    Text scoreText;
    RectangleShape scoreBg;
    bool bossDeathRewarded;
    void updateScoreDisplay();

    // Level progression
    bool levelCompleted;  // to avoid multiple transitions
    bool gameCompleted;   // after level 3

    void initEnemies();
    void iniBoss();
    void reset(bool resetScore = true);  // optional reset score

public:
    GamePlay(Vector2u winSize, int level = 1);
    void checkPattack();
    void checkEAttack();
    void checkBAttack();
    void update();
    void render(RenderWindow& window);
    bool isGameOver() const;
    bool isGameCompleted() const { return gameCompleted; }
    int getCurrentLevel() const { return currentLevel; }
};

#endif