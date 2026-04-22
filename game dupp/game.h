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

    // Death sequence
    bool deathSequenceActive;
    Clock deathTimer;
    float deathDuration = 2.0f;
    bool deathScreenFinished;   // tells menu that game is really over
    void drawDeathScreen(RenderWindow& window);

    // Victory sequence
    bool victorySequenceActive;
    Clock victoryTimer;
    float victoryDuration = 3.0f;
    bool victoryFinished;
    void drawVictoryScreen(RenderWindow& window);

    void initEnemies();
    void iniBoss();
    void reset(bool resetScore = true);
    void drawHealthBar(RenderWindow& window);

public:
    GamePlay(Vector2u winSize, int level = 1);
    void checkPattack();
    void checkEAttack();
    void checkBAttack();
    void update();
    void render(RenderWindow& window);
    bool isGameOver() const;                // returns true when death screen finished
    bool isGameCompleted() const { return gameCompleted; }
    int getCurrentLevel() const { return currentLevel; }
};

#endif