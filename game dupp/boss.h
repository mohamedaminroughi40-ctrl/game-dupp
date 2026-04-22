#ifndef BOSS_H
#define BOSS_H

#include "enemie.h"
#include<SFML/Graphics.hpp>

using namespace sf;

class boss : public enemie
{
private:
	Texture bAttackTex;
	Texture bIdleTex;
	Texture bWalkTex;
	Texture bWalkBAckTex;
	Texture BDeathTex;

	void initBoss();
	void spriteUp(); 
	void stateHundling(float playerPosX);
public:
	boss();
	void update(float playerPosX);
	bool getAttackFarme();
	int getHeah();


};

#endif
