#ifndef ENEMIE_H
#define ENEMIE_H

#include <math.h>

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
using namespace sf;




class enemie
{


protected:
	enum stateMov
	{
		Eidle, Echase, EshaseB, dead, attack
	};

	stateMov state;
	Texture idleTex;
	Texture apearTex;
	Texture chaseTex;
	Texture chaseBTex;
	Texture vanishTex;
	Texture attackTex;
	Sprite enemieSprite;
	virtual void initEnemie();

	//heath sys
	int hp;
	int maxHp;
	float speed;
	bool deathE;

	//animation logic
	int currentFrame;
	int maxFrame;
	float widthFrame = 64;
	float hightFrame = 80;
	//animation timer
	Clock animationTimer;
	float animationTimeLimit;

	// hit / death effect
	bool attackDealt;
	Clock hitTimer;
	float hitFlashDuration = 0.18f;
	bool isHit = false;

public:
	enemie();
	void setposition(float posX, float poseY);
	void animation();
	virtual void stateHundling(float playerPosX);
	void movUp(float playerPosX);
	virtual void spriteUp();
	void update(float playerPosX);

	void takeDamage(int damage);

	Sprite& getSprite();
	int getHeah();
	virtual FloatRect getHitBox();
	bool death();

	// hit effect
	void updateHitEffect();
	bool isAttaking();
	bool getAttackDealt();
	void setAttackDealt(bool dealt);
	virtual bool getAttackFarme();
	void setHp(int newHp);
};

#endif 