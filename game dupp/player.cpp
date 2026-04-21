#include "player.h"

using namespace sf;

void player::initPlayer()
{
	//get the texturs
	this->playerIdleTex.loadFromFile("assets/player/idle.png");
	this->playerAttakTex.loadFromFile("assets/player/attack.png");
	this->playerDeathTex.loadFromFile("assets/player/death.png");
	this->playerRunTex.loadFromFile("assets/player/run.png");
	this->playerRunBackTex.loadFromFile("assets/player/runB.png");
	this->playerJumpTex.loadFromFile("assets/player/jump.png");
	this->playerHurtTex.loadFromFile("assets/player/hurt.png");  // Fix: load hurt texture

	//initialse the state
	this->playerState = idle;
	this->playerSprite.setTexture(this->playerIdleTex);
	this->stateChanged = false;
	//initialse the speed
	this->currentFrame = 0;
	this->animationSpeed = 0.1f;

	//jump init
	this->gravity = 0.8f;
	this->groud = 200.f;
	this->upSpeed = 0.f;
	this->isJumping = false;
	this->jumpFps = 0.01f;

	//heath init
	this->maxHealth = 100;
	this->hp = maxHealth;

	//cooldown attake init
	this->canAttak = true;
	this->attakCooldown = 1.f;

	//hurt init
	this->isHurt = false;
	this->isDead = false;
}

bool player::isAttakFrame()
{
	if (this->isAttaking() && this->currentFrame == 2)
	{
		return true;
	}
	return false;
}

player::player()
{
	this->initPlayer();

}


void player::takeDamage(int damage)
{
	this->playerSprite.move(-100, 0);
	this->hp -= damage;
	if (this->hp < 0)
	{
		this->isDead = true;
		this->hp = 0;
	}

	// Trigger hurt state
	this->isHurt = true;
	this->hurtTimer.restart();
	std::cout << "Player took " << damage << " damage! HP: " << this->hp << std::endl;
}


void player::setPosition(float positionX, float positionY)
{
	this->playerSprite.setPosition(positionX, positionY);
}

void player::setGround(float g)
{
	this->groud = g;

}


void player::updateState()
{
	if (this->isDead)
	{
		this->maxFrame = 6;
		this->playerState = death;
		return;
	}
	// Check if hurt effect should end
	if (this->isHurt)
	{
		if (this->hurtTimer.getElapsedTime().asSeconds() >= this->hurtDuration)
		{
			this->isHurt = false;
		}
	}

	// If hurt, prioritize hurt state
	if (this->isHurt)
	{
		this->playerState = hurt;
		this->maxFrame = 0;  // Only 1 frame
		return;
	}

	this->playerState = idle;
	this->maxFrame = 3;

	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		this->playerState = run;
		this->maxFrame = 5;
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		this->playerState = runB;
		this->maxFrame = 5;
	}

	if (Keyboard::isKeyPressed(Keyboard::N) && this->canAttak)
	{
		this->playerState = attak;
		this->maxFrame = 4;
	}
	if (Keyboard::isKeyPressed(Keyboard::W) && !this->isJumping)
	{
		this->playerState = jump;
		this->upSpeed = -10.f;
		this->isJumping = true;
		this->maxFrame = 3;
	}
}

void player::updatePlayersprite()
{
	state displayState = this->playerState;
	if (this->isJumping && !this->isHurt)  // Don't jump while hurt
	{
		displayState = jump;
	}

	if (this->playerLastState != displayState)
	{
		switch (displayState) {
		case jump:
			this->playerSprite.setTexture(this->playerJumpTex);
			break;
		case run:
			this->playerSprite.setTexture(this->playerRunTex);
			break;
		case runB:
			this->playerSprite.setTexture(this->playerRunBackTex);
			break;
		case idle:
			this->playerSprite.setTexture(this->playerIdleTex);
			break;
		case attak:
			this->playerSprite.setTexture(this->playerAttakTex);
			this->attakeDealt = false;
			break;
		case hurt:
			this->playerSprite.setTexture(this->playerHurtTex);
			break;

		case death:
			this->playerSprite.setTexture(this->playerDeathTex);
			break;
		}
		this->playerLastState = displayState;
		this->currentFrame = 0;
	}
}

void player::updateJump()
{
	if (this->jumpTimer.getElapsedTime().asSeconds() >= this->jumpFps)
	{
		this->upSpeed += this->gravity;
		this->playerSprite.move(0, this->upSpeed);

		if (this->playerSprite.getPosition().y >= this->groud)
		{
			this->playerSprite.setPosition(this->playerSprite.getPosition().x, this->groud);
			this->upSpeed = 0;
			this->isJumping = false;
		}
		this->jumpTimer.restart();
	}

}

void player::updatePlayeranimation()
{
	if (this->isDead && this->currentFrame == 6)
	{
		return;
	}
	// Don't animate hurt state (it's just 1 frame)
	if (this->playerState == hurt)
	{
		this->playerSprite.setTextureRect(IntRect(0, 0, this->widthFrame, this->hightFrame));
		return;
	}

	if (this->animationClock.getElapsedTime().asSeconds() >= this->animationSpeed)
	{
		if (this->currentFrame < maxFrame)
		{
			currentFrame++;
		}
		else
		{
			currentFrame = 0;
		}
		this->playerSprite.setTextureRect(IntRect(this->widthFrame * currentFrame, 0, this->widthFrame, this->hightFrame));
		this->animationClock.restart();
	}
}



FloatRect player::getHitBox()
{
	if (this->playerState == attak)
	{
		return FloatRect(
			(float)this->playerSprite.getPosition().x + 200.f,//position infront of the player
			(float)this->playerSprite.getPosition().y + 100, //the ground as the player 
			60.f, 40.f);
	}
	return FloatRect(0, 0, 0, 0); // no attak
}


Sprite& player::getSrite() {
	return this->playerSprite;
}

Vector2f player::getPosition()
{
	return this->playerSprite.getPosition();
}

bool player::isAttaking()
{
	if (this->playerState == attak) return true;
	return false;
}

bool player::isMoving()
{
	if (this->playerState == run || this->playerState == runB || this->playerState == attak)
	{
		return true;
	}
	return false;
}

void player::update()
{
	this->updateState();

	if (this->playerState == run)
	{
		this->playerSprite.move(5.f, 0);
	}
	if (this->playerState == runB)
	{
		this->playerSprite.move(-5.f, 0);
	}
	if (this->isJumping) this->updateJump();
	this->updatePlayersprite();
	this->updatePlayeranimation();
}




