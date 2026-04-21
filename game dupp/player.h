#ifndef  PLAYER_H

#define PLAYER_H


#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include <iostream>
using namespace sf;

//stats
enum state
{
	idle,
	run,
	runB,
	jump,
	death,
	attak,
	hurt  // Add hurt state
};


class player
{
	Event ev;

	//texturs and sprite
	Texture playerIdleTex;
	Texture playerAttakTex;
	Texture playerJumpTex;
	Texture playerRunTex;
	Texture playerRunBackTex;
	Texture playerDeathTex;
	Texture playerHurtTex;  // Add hurt texture
	Sprite playerSprite;
	state playerState;
	state playerLastState;
	bool stateChanged;
	virtual void initPlayer();


	//jump logic
	bool isJumping;
	float gravity;
	float upSpeed;
	float groud;
	Clock jumpTimer;
	float jumpFps;

	//animation logic
	int currentFrame;
	int maxFrame;
	float widthFrame = 160.f;
	float hightFrame = 90.f;
	//time
	Clock animationClock;
	float animationSpeed;

	//hit box
	FloatRect hitBox;


	//health sys
	int maxHealth;
	int hp;

	//logic attak
	Clock cooldown;
	float attakCooldown;
	bool canAttak;
	bool attakeDealt;  // Add here in private section

	// hurt effect
	Clock hurtTimer;
	float hurtDuration = 0.4f;  // Show hurt for 0.4 seconds
	bool isHurt;
	bool isDead;

public:
	player();

	void setPosition(float positionX, float positionY);
	void setGround(float g);
	void takeDamage(int damage);


	void updateState();

	void updateJump();

	void updatePlayersprite();

	void updatePlayeranimation();

	void update();


	Sprite& getSrite();
	FloatRect getHitBox();
	Vector2f getPosition();
	bool isAttaking();
	bool isMoving();
	bool isAttakFrame();
	bool getAttakeDealt() const { return this->attakeDealt; }  // Add getter instead
	void setAttakeDealt(bool dealt) { this->attakeDealt = dealt; }  // Add setter
	int getHealth() const { return this->hp; }
	void resetHealth() { this->hp = maxHealth; }

};



#endif 