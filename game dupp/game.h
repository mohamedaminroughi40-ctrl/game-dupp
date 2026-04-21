#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
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

    void initEnemies();   // now uses currentLevel and windowSize
    void iniBoss();
    void reset();

public:
    GamePlay(Vector2u winSize, int level = 1);
    void checkPattack();
    void checkEAttack();
    void checkBAttack();
    void update();
    void render(RenderWindow& window);
    bool isGameOver() const;
};

#endif