#include "game.h"

void GamePlay::initBackground(Vector2u winSize, int level)
{
    std::string bgPath1, bgPath2;

    switch (level)
    {
    case 1:
        bgPath1 = "assets/Backgrounds/PNG/summer6/Summer6.png";
        bgPath2 = "assets/bg2.png";
        break;
    case 2:
        bgPath1 = "assets/bg1.png";
        bgPath2 = "assets/bg2.png";
        break;
    case 3:
        bgPath1 = "assets/bg3.png";
        bgPath2 = "assets/bg2.png";
        break;
    default:
        bgPath1 = "assets/bg1.png";
        bgPath2 = "assets/bg2.png";
    }

    // Load background 1
    if (!backgroundTex1.loadFromFile(bgPath1))
    {
        std::cout << "ERROR: Could not load " << bgPath1 << " for level " << level << std::endl;
        std::cout << "Falling back to assets/bg1.png" << std::endl;
        if (!backgroundTex1.loadFromFile("assets/bg1.png"))
            std::cout << "CRITICAL: Fallback background also missing!" << std::endl;
    }

    // Load background 2
    if (!backgroundTex2.loadFromFile(bgPath2))
    {
        std::cout << "ERROR: Could not load " << bgPath2 << " for level " << level << std::endl;
        std::cout << "Falling back to assets/bg2.png" << std::endl;
        if (!backgroundTex2.loadFromFile("assets/bg2.png"))
            std::cout << "CRITICAL: Fallback background 2 also missing!" << std::endl;
    }

    // Assign textures to sprites (important even if the same texture is reused)
    background1.setTexture(backgroundTex1);
    background2.setTexture(backgroundTex2);

    // Calculate scales to exactly fill the window
    float scaleX1 = (float)winSize.x / backgroundTex1.getSize().x;
    float scaleY1 = (float)winSize.y / backgroundTex1.getSize().y;
    background1.setScale(scaleX1, scaleY1);

    float scaleX2 = (float)winSize.x / backgroundTex2.getSize().x;
    float scaleY2 = (float)winSize.y / backgroundTex2.getSize().y;
    background2.setScale(scaleX2, scaleY2);

    // Optional: print texture dimensions for debugging
    std::cout << "Level " << level << " background1 size: "
        << backgroundTex1.getSize().x << "x" << backgroundTex1.getSize().y
        << " scaled to " << scaleX1 << "," << scaleY1 << std::endl;
}

void GamePlay::initEnemies()
{
    enemies.clear();

    int numEnemies = 2;
    int enemyHp = 20;
    float startX = (float)windowSize.x - 200;

    switch (currentLevel)
    {
    case 1:
        numEnemies = 2;
        enemyHp = 20;
        break;
    case 2:
        numEnemies = 3;
        enemyHp = 30;
        break;
    case 3:
        numEnemies = 4;
        enemyHp = 40;
        break;
    default:
        numEnemies = 2;
        enemyHp = 20;
    }

    for (int i = 0; i < numEnemies; ++i)
    {
        enemie e;
        float xPos = startX - (i * 150);
        e.setposition(xPos, (float)windowSize.y - 190);
        e.getSprite().setScale(1.5f, 1.5f);   // FIXED: absolute scale
        e.setHp(enemyHp);
        enemies.push_back(e);
    }
}

void GamePlay::iniBoss()
{
    int bossHp = 50;
    switch (currentLevel)
    {
    case 1: bossHp = 40; break;
    case 2: bossHp = 60; break;
    case 3: bossHp = 80; break;
    default: bossHp = 50;
    }
    Mbooss.setposition(1000.f, (float)windowSize.y - 330);
    Mbooss.getSprite().setScale(3.f, 3.f);   // FIXED: absolute scale
    Mbooss.setHp(bossHp);
}

void GamePlay::reset(bool resetScore)
{
    initBackground(windowSize, currentLevel);
    initEnemies();
    iniBoss();

    P.setPosition(100.f, (float)windowSize.y - 280);
    P.setGround((float)windowSize.y - 280);
    P.getSrite().setScale(2.f, 2.f);   // FIXED: absolute scale
    P.resetHealth();

    if (resetScore)
    {
        score = 0;
    }
    bossDeathRewarded = false;
    levelCompleted = false;

    // Font and score text setup
    if (!scoreFont.loadFromFile("assets/ARCADE.ttf"))
    {
        std::cout << "Warning: Could not load assets/ARCADE.ttf for score" << std::endl;
    }
    scoreText.setFont(scoreFont);
    scoreText.setCharacterSize(48);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(3);
    scoreText.setPosition(30, 20);

    scoreBg.setSize(sf::Vector2f(250, 70));
    scoreBg.setFillColor(sf::Color(0, 0, 0, 180));
    scoreBg.setPosition(20, 10);

    updateScoreDisplay();
}

GamePlay::GamePlay(Vector2u winSize, int level)
    : currentLevel(level), windowSize(winSize), gameCompleted(false), levelCompleted(false)
{
    reset(true);
}

void GamePlay::updateScoreDisplay()
{
    std::stringstream ss;
    ss << "SCORE: " << score;
    scoreText.setString(ss.str());

    float textWidth = scoreText.getLocalBounds().width;
    scoreBg.setSize(sf::Vector2f(textWidth + 40, 70));
}

void GamePlay::checkPattack()
{
    FloatRect playerHitBox = P.getHitBox();

    for (size_t i = 0; i < enemies.size(); ++i)
    {
        if (playerHitBox.intersects(enemies[i].getSprite().getGlobalBounds()))
        {
            if (P.isAttakFrame() && !P.getAttakeDealt())
            {
                enemies[i].takeDamage(10);
                P.setAttakeDealt(true);
                std::cout << "Player attacked enemy " << i << " health: " << enemies[i].getHeah() << std::endl;
            }
        }
    }

    if (playerHitBox.intersects(Mbooss.getSprite().getGlobalBounds()))
    {
        if (P.isAttakFrame() && !P.getAttakeDealt())
        {
            Mbooss.takeDamage(10);
            P.setAttakeDealt(true);
            std::cout << "Player attacked BOSS! Health: " << Mbooss.getHeah() << std::endl;
        }
    }
}

void GamePlay::checkEAttack()
{
    for (size_t i = 0; i < enemies.size(); ++i)
    {
        FloatRect enemyHitBox = enemies[i].getHitBox();
        if (enemyHitBox.intersects(P.getSrite().getGlobalBounds()))
        {
            if (enemies[i].isAttaking() && !enemies[i].getAttackDealt() && enemies[i].getAttackFarme())
            {
                enemies[i].setAttackDealt(true);
                P.takeDamage(10);
                std::cout << "Player damaged by enemy!" << std::endl;
            }
        }
    }
}

void GamePlay::checkBAttack()
{
    FloatRect bossHitBox = Mbooss.getHitBox();
    if (bossHitBox.intersects(P.getSrite().getGlobalBounds()))
    {
        if (Mbooss.isAttaking() && !Mbooss.getAttackDealt() && Mbooss.getAttackFarme())
        {
            Mbooss.setAttackDealt(true);
            P.takeDamage(10);
            std::cout << "Player damaged by BOSS!" << std::endl;
        }
    }
}

void GamePlay::update()
{
    if (gameCompleted) return;

    checkPattack();
    checkEAttack();
    checkBAttack();
    P.update();
    Mbooss.update(P.getPosition().x);

    for (size_t i = 0; i < enemies.size(); ++i)
    {
        enemies[i].update(P.getPosition().x);
        if (enemies[i].death())
        {
            score += 50;
            updateScoreDisplay();
            enemies.erase(enemies.begin() + i);
            --i;
        }
    }

    if (Mbooss.death() && !bossDeathRewarded)
    {
        score += 100;
        updateScoreDisplay();
        bossDeathRewarded = true;
    }

    if (!levelCompleted && enemies.empty() && Mbooss.death())
    {
        levelCompleted = true;
        if (currentLevel < 3)
        {
            currentLevel++;
            std::cout << "Level " << currentLevel - 1 << " completed! Moving to level " << currentLevel << std::endl;
            reset(false);
        }
        else
        {
            gameCompleted = true;
            std::cout << "Congratulations! You completed all levels!" << std::endl;
        }
    }
}

void GamePlay::render(RenderWindow& window)
{
    window.draw(background1);
    window.draw(background2);
    window.draw(Mbooss.getSprite());
    for (auto& e : enemies)
        window.draw(e.getSprite());
    window.draw(P.getSrite());
    window.draw(scoreBg);
    window.draw(scoreText);
}

bool GamePlay::isGameOver() const
{
    return (P.getHealth() <= 0);
}