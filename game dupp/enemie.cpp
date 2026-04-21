#include "enemie.h"

void enemie::initEnemie()
{
	this->idleTex.loadFromFile("assets/gost/Idle.png");
	this->chaseTex.loadFromFile("assets/gost/Chase.png");
	this->chaseBTex.loadFromFile("assets/gost/ChaseB.png");
	this->apearTex.loadFromFile("assets/gost/Appear.png");
	this->vanishTex.loadFromFile("assets/gost/Vanish.png");
	this->attackTex.loadFromFile("assets/gost/Attack.png");

	//heath init
	this->maxHp = 20;
	this->hp = maxHp;
	this->deathE = false;

	this->speed = 2.f;
}

enemie::enemie()
{
	this->initEnemie();
	this->enemieSprite.setTexture(this->idleTex);
	//animation
	this->currentFrame = 0;
	this->animationTimeLimit = 0.5f;
}

void enemie::setposition(float posX, float poseY)
{
	this->enemieSprite.setPosition(posX, poseY);
}

void enemie::animation()
{
	if (this->state == dead && this->currentFrame == 3)
	{
		this->currentFrame = 3;
		this->deathE = true;
		return;
	}
	if (this->animationTimer.getElapsedTime().asSeconds() >= this->animationTimeLimit)
	{
		if (this->currentFrame < this->maxFrame)
		{
			this->currentFrame++;
		}
		else
		{
			this->currentFrame = 0;
		}
		this->animationTimer.restart();
		this->enemieSprite.setTextureRect(IntRect(this->currentFrame * this->widthFrame, 0, this->widthFrame, this->hightFrame));
	}
}

void enemie::stateHundling(float playerPosX)
{
	if (this->hp <= 0)
	{
		this->state = dead;
		return;
	}
	float distance = abs(playerPosX - this->enemieSprite.getPosition().x + 100.f);
	if (distance < 400) {
		if (distance < 50)
		{
			this->state = attack;
		}
		else if (playerPosX < this->enemieSprite.getPosition().x)
		{
			this->state = Echase;
		}
		else
		{
			this->state = EshaseB;
		}
	}
	else this->state = Eidle;
}

void enemie::movUp(float playerPosX)
{
	float distance = abs(playerPosX - this->enemieSprite.getPosition().x);
	float stopDistance = 20;  // Stop at this distance from player

	if (this->state == Echase && distance > stopDistance)
	{
		this->enemieSprite.move(-this->speed, 0.f);
	}
	else if (this->state == EshaseB && distance > stopDistance)
	{
		this->enemieSprite.move(this->speed, 0.f);
	}


}


void enemie::spriteUp()
{
	stateMov previousState = this->state;  // Track previous state

	switch (this->state) {
	case Eidle:
		this->maxFrame = 6;
		this->enemieSprite.setTexture(this->idleTex);
		this->animationTimeLimit = 0.45f;
		break;
	case Echase:
		this->maxFrame = 3;
		this->enemieSprite.setTexture(this->chaseTex);
		this->animationTimeLimit = 0.15f;
		break;
	case EshaseB:
		this->maxFrame = 3;
		this->enemieSprite.setTexture(this->chaseBTex);
		this->animationTimeLimit = 0.15f;
		break;
	case dead:
		this->maxFrame = 3;
		this->enemieSprite.setTexture(this->vanishTex);
		this->animationTimeLimit = 0.25f;
		break;
	case attack:
		this->maxFrame = 3;
		this->enemieSprite.setTexture(this->attackTex);
		this->animationTimeLimit = 0.18f;
		break;
	}
}

void enemie::update(float playerPosX)
{
	stateMov lastState = this->state;  // Save current state

	this->stateHundling(playerPosX);

	// Reset frame when state changes
	if (this->state != lastState)
	{
		this->currentFrame = 0;
		this->animationTimer.restart();
		if (this->state == attack)
		{
			this->attackDealt = false;
		}
	}

	this->movUp(playerPosX);
	this->spriteUp();
	this->animation();
	this->updateHitEffect(); // handle flash and color reset
}

void enemie::updateHitEffect()
{
	if (!this->isHit) return;

	if (this->hitTimer.getElapsedTime().asSeconds() >= this->hitFlashDuration)
	{
		// end flash
		this->enemieSprite.setColor(Color::White);
		this->isHit = false;
	}
	else
	{
		// flash red while within duration
		this->enemieSprite.setColor(Color::Red);
	}
}

bool enemie::isAttaking()
{
	if (this->state == attack)
	{
		return true;
	}
	return false;
}

bool enemie::getAttackDealt()
{
	return this->attackDealt;
}

void enemie::setAttackDealt(bool dealt)
{
	this->attackDealt = dealt;
}

bool enemie::getAttackFarme()
{
	if (this->currentFrame == 2)
	{
		return true;
	}
	return false;
}

void enemie::setHp(int newHp)
{
	hp = newHp;
	if (hp > maxHp) maxHp = hp;
	if (hp <= 0) state = dead;
}



void enemie::takeDamage(int damage)
{
	this->hp -= damage;
	if (this->hp < 0)
	{
		this->hp = 0;
	}

	// Trigger hit flash
	this->isHit = true;
	this->hitTimer.restart();

	// If died, make sure death state and reset animation so vanish plays
	if (this->hp <= 0)
	{
		this->state = dead;
		this->currentFrame = 0;
		this->animationTimer.restart();
	}
}

Sprite& enemie::getSprite()
{
	return this->enemieSprite;
}

FloatRect enemie::getHitBox()
{
	// When attacking, use the enemy sprite bounds as hit area.
	// You can adjust the returned rect to be offset in front of the enemy if needed.
	return this->enemieSprite.getGlobalBounds();
}

bool enemie::death()
{
	return this->deathE;
}

int enemie::getHeah()
{
	return this->hp;
}




