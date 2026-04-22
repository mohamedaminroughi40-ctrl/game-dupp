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

    if (!backgroundTex1.loadFromFile(bgPath1))
    {
        std::cout << "ERROR: Could not load " << bgPath1 << " for level " << level << std::endl;
        std::cout << "Falling back to assets/bg1.png" << std::endl;
        if (!backgroundTex1.loadFromFile("assets/bg1.png"))
            std::cout << "CRITICAL: Fallback background also missing!" << std::endl;
    }

    if (!backgroundTex2.loadFromFile(bgPath2))
    {
        std::cout << "ERROR: Could not load " << bgPath2 << " for level " << level << std::endl;
        std::cout << "Falling back to assets/bg2.png" << std::endl;
        if (!backgroundTex2.loadFromFile("assets/bg2.png"))
            std::cout << "CRITICAL: Fallback background 2 also missing!" << std::endl;
    }

    background1.setTexture(backgroundTex1);
    background2.setTexture(backgroundTex2);

    float scaleX1 = (float)winSize.x / backgroundTex1.getSize().x;
    float scaleY1 = (float)winSize.y / backgroundTex1.getSize().y;
    background1.setScale(scaleX1, scaleY1);

    float scaleX2 = (float)winSize.x / backgroundTex2.getSize().x;
    float scaleY2 = (float)winSize.y / backgroundTex2.getSize().y;
    background2.setScale(scaleX2, scaleY2);

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
        e.getSprite().setScale(1.5f, 1.5f);
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
    Mbooss.getSprite().setScale(3.f, 3.f);
    Mbooss.setHp(bossHp);
}

void GamePlay::reset(bool resetScore)
{
    initBackground(windowSize, currentLevel);
    initEnemies();
    iniBoss();

    P.setPosition(100.f, (float)windowSize.y - 280);
    P.setGround((float)windowSize.y - 280);
    P.getSrite().setScale(2.f, 2.f);
    P.resetHealth();

    if (resetScore)
        score = 0;

    bossDeathRewarded = false;
    levelCompleted = false;
    deathSequenceActive = false;
    deathScreenFinished = false;
    victorySequenceActive = false;
    victoryFinished = false;

    static bool fontLoaded = false;
    if (!fontLoaded)
    {
        if (!scoreFont.loadFromFile("assets/ARCADE.ttf"))
            std::cout << "Warning: Could not load assets/ARCADE.ttf for score" << std::endl;
        fontLoaded = true;
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

    // ----- Death sequence handling -----
    if (P.getHealth() <= 0 && !deathSequenceActive)
    {
        deathSequenceActive = true;
        deathTimer.restart();
        deathScreenFinished = false;
        return;
    }

    if (deathSequenceActive)
    {
        P.update();
        if (deathTimer.getElapsedTime().asSeconds() >= deathDuration)
            deathScreenFinished = true;
        return;
    }

    // ----- Victory sequence handling -----
    if (victorySequenceActive)
    {
        if (victoryTimer.getElapsedTime().asSeconds() >= victoryDuration)
        {
            victoryFinished = true;
            gameCompleted = true;
        }
        return;
    }

    // ----- Normal gameplay -----
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
            // All levels completed – start victory sequence
            victorySequenceActive = true;
            victoryTimer.restart();
            victoryFinished = false;
            std::cout << "Congratulations! You completed all levels!" << std::endl;
        }
    }
}

void GamePlay::drawHealthBar(RenderWindow& window)
{
    float barWidth = 300.f;
    float barHeight = 30.f;
    float barX = (float)windowSize.x - barWidth - 20.f;
    float barY = 20.f;

    RectangleShape healthBarBg(Vector2f(barWidth, barHeight));
    healthBarBg.setFillColor(Color(50, 50, 50, 200));
    healthBarBg.setOutlineColor(Color::White);
    healthBarBg.setOutlineThickness(2);
    healthBarBg.setPosition(barX, barY);

    float healthPercent = (float)P.getHealth() / (float)P.getMaxHealth();
    float currentWidth = barWidth * healthPercent;
    RectangleShape healthBarFg(Vector2f(currentWidth, barHeight));
    if (healthPercent > 0.6f)
        healthBarFg.setFillColor(Color::Green);
    else if (healthPercent > 0.3f)
        healthBarFg.setFillColor(Color::Yellow);
    else
        healthBarFg.setFillColor(Color::Red);
    healthBarFg.setPosition(barX, barY);

    Text hpText;
    hpText.setFont(scoreFont);
    hpText.setCharacterSize(24);
    hpText.setFillColor(Color::White);
    hpText.setOutlineColor(Color::Black);
    hpText.setOutlineThickness(2);
    std::stringstream ss;
    ss << P.getHealth() << "/" << P.getMaxHealth();
    hpText.setString(ss.str());
    FloatRect textRect = hpText.getLocalBounds();
    hpText.setPosition(barX + (barWidth - textRect.width) / 2,
        barY + (barHeight - textRect.height) / 2);

    window.draw(healthBarBg);
    window.draw(healthBarFg);
    window.draw(hpText);
}

void GamePlay::drawDeathScreen(RenderWindow& window)
{
    RectangleShape overlay(Vector2f((float)windowSize.x, (float)windowSize.y));
    overlay.setFillColor(Color(0, 0, 0, 200));
    window.draw(overlay);

    Text deathText;
    deathText.setFont(scoreFont);
    deathText.setString("YOU DIE");
    deathText.setCharacterSize(120);
    deathText.setFillColor(Color::Red);
    deathText.setOutlineColor(Color::Black);
    deathText.setOutlineThickness(5);
    FloatRect textRect = deathText.getLocalBounds();
    deathText.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    deathText.setPosition((float)windowSize.x / 2.0f, (float)windowSize.y / 2.0f);
    window.draw(deathText);
}

void GamePlay::drawVictoryScreen(RenderWindow& window)
{
    RectangleShape overlay(Vector2f((float)windowSize.x, (float)windowSize.y));
    overlay.setFillColor(Color(0, 0, 0, 200));
    window.draw(overlay);

    Text victoryText;
    victoryText.setFont(scoreFont);
    victoryText.setString("YOU WIN!");
    victoryText.setCharacterSize(120);
    victoryText.setFillColor(Color(255, 215, 0)); // Gold
    victoryText.setOutlineColor(Color::Black);
    victoryText.setOutlineThickness(5);
    FloatRect textRect = victoryText.getLocalBounds();
    victoryText.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    victoryText.setPosition((float)windowSize.x / 2.0f, (float)windowSize.y / 2.0f);
    window.draw(victoryText);
}

void GamePlay::render(RenderWindow& window)
{
    window.draw(background1);
    window.draw(background2);
    window.draw(Mbooss.getSprite());
    for (auto& e : enemies)
        window.draw(e.getSprite());
    window.draw(P.getSrite());

    if (!deathScreenFinished && P.getHealth() > 0 && !victorySequenceActive)
        drawHealthBar(window);

    window.draw(scoreBg);
    window.draw(scoreText);

    if (deathSequenceActive)
        drawDeathScreen(window);
    else if (victorySequenceActive)
        drawVictoryScreen(window);
}

bool GamePlay::isGameOver() const
{
    return deathScreenFinished;
}