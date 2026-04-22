#include "boss.h"

void boss::initBoss()
{
	this->bAttackTex.loadFromFile("assets/org/orgAttack.png");
	this->bIdleTex.loadFromFile("assets/org/orgIdle.png");
	this->bWalkTex.loadFromFile("assets/org/orgWalk.png");
	this->bWalkBAckTex.loadFromFile("assets/org/orgeWalkBack.png");
	this->BDeathTex.loadFromFile("assets/org/orgDeath.png");

	this->widthFrame = 144.f;
	this->hightFrame = 80.f;
}

boss::boss() : enemie()
{
	this->initBoss();
	// Set initial texture
	this->getSprite().setTexture(this->bIdleTex);
}

void boss::spriteUp()
{
	// Override parent's spriteUp() to use boss textures
	switch (this->state) {
	case Eidle:
		this->maxFrame = 3;  // Adjust based on your sprite
		this->getSprite().setTexture(this->bIdleTex);
		this->animationTimeLimit = 0.45f;
		break;
	case Echase:
		this->maxFrame = 5;  // Adjust based on your sprite
		this->getSprite().setTexture(this->bWalkTex);
		this->animationTimeLimit = 0.15f;
		break;
	case EshaseB:
		this->maxFrame = 5;
		this->getSprite().setTexture(this->bWalkBAckTex);
		this->animationTimeLimit = 0.15f;
		break;
	case dead:
		this->maxFrame = 6;
		this->getSprite().setTexture(this->BDeathTex);  
		this->animationTimeLimit = 0.25f;
		break;
	case attack:
		this->maxFrame = 6;  // Adjust based on your sprite
		this->getSprite().setTexture(this->bAttackTex);
		this->animationTimeLimit = 0.18f;
		break;
	}
}

void boss::stateHundling(float playerPosX)
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

void boss::update(float playerPosX)
{
	// Call parent update (all the enemy logic)
	enemie::update(playerPosX);

	// Add boss-specific behavior here if needed
}

bool boss::getAttackFarme()
{
	if (this->currentFrame == 4)
	{
		return true;
	}
	return false;
}

int boss::getHeah()
{
	return this->hp;
}
