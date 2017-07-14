#include "Player.h"
#include "Constants.h"
#include "HelpFunctions.h"
#include <cmath>
#include <iostream>

extern sf::RenderWindow window;						//The window that draws the graphics on the screen.
extern float dt;
extern int playerCount;
extern sf::Texture shieldTexture;


Player::Player(int num)
{
	id = num;
	pos = sf::Vector2f(mapWidth/2 , mapHeight/2);
	vel = sf::Vector2f(0,0);
	acc = sf::Vector2f(0,0);
	
	shipRotation = 0;
	boost = 0.8f;
	speed = playerMaxSpeed;
 

	randomPosition(pos);

	body.setOrigin(sf::Vector2f(bodyRadius, bodyRadius));
	body.setRadius(bodyRadius);
	body.setPosition(pos);
	body.setPointCount(3);
	body.setScale(1, 1.3f);
	body.setFillColor(playerColor[playerCount] - sf::Color(10, 10, 10, 110));
	body.setOutlineColor(playerColor[playerCount]);
	body.setOutlineThickness(bodyRadius - boostIndicatorRadius);

	boostIndicator.setOrigin(sf::Vector2f(bodyRadius * boost, bodyRadius * boost));
	boostIndicator.setRadius(bodyRadius * boost);
	boostIndicator.setPosition(pos);
	boostIndicator.setPointCount(3);
	boostIndicator.setScale(1, 1.3f);
	boostIndicator.setFillColor(sf::Color::Yellow);

	shield.setTexture(shieldTexture);					//Set up sprite:
	shield.setColor(sf::Color(255,255,255, 255*SHIELDOPACITY));
	shield.setOrigin(sf::Vector2f(shield.getTexture()->getSize().x/2.0f, shield.getTexture()->getSize().y/2.0f));
	shield.setScale(sf::Vector2f(SHIELDSCALE, SHIELDSCALE));
	shield.setPosition(pos);

															//Construct tail particle effect:
	tailPtr = new Emitter(emitterStartPosition, 35, 0.005f, 300,
						 playerColor[id], sf::Color(255,0,0,200),
						 0.4f, 0.22f, 0.7, 0.996f, 0.000001f);

}



bool Player::shieldEncounter(sf::Vector2f encPos, bool projectile) //True if player blocks encounter.
{
	bool playerBlocksEncounter = false;
	int encAngle = getAngle(pos, encPos);


	if (projectile)										//If player tries to block projectile.
	{
		if (std::abs(encAngle - shieldRotation) < SHIELDCOVERAGE)
		{
			playerBlocksEncounter = true;
		}
	}
	else												//If player tries to block a ramming attack.
	{
		if (std::abs(((encAngle + 180) % 360) -shieldRotation) < SHIELDCOVERAGE)
		{
			playerBlocksEncounter = true;
		}
	}


	return playerBlocksEncounter;
}





void Player::update()
{															
	float leftStickAngle = getAngle(sf::Vector2f(0,0),		//Get input form left stick into euler angles.
								sf::Vector2f(sf::Joystick::getAxisPosition(id, sf::Joystick::X), 
								sf::Joystick::getAxisPosition(id, sf::Joystick::Y)));
	float rightStickAngle = getAngle(sf::Vector2f(0,0),		//Get input form left stick into euler angles.
								sf::Vector2f(sf::Joystick::getAxisPosition(id, sf::Joystick::U), 
								sf::Joystick::getAxisPosition(id, sf::Joystick::V)));

	shipRotation = getAngle(sf::Vector2f(0,0), vel);
	shieldRotation = rightStickAngle;

	if (std::abs(leftStickAngle - shipRotation) < 180)			//Steers the player left or right depending on current cource:
	{
		if (leftStickAngle < shipRotation)
			vel += sf::Vector2f(std::cos(toRadians(shipRotation + 180)) * playerTurnSpeed, std::sin(toRadians(shipRotation + 180)) * playerTurnSpeed);			//Turn clockwise.
		else
			vel += sf::Vector2f(std::cos(toRadians(shipRotation)) * playerTurnSpeed, std::sin(toRadians(shipRotation)) * playerTurnSpeed);			//Turn counter clockwise.
	}

	else
	{
		if (leftStickAngle > shipRotation)
		
			vel += sf::Vector2f(std::cos(toRadians(shipRotation + 180)) * playerTurnSpeed, std::sin(toRadians(shipRotation + 180)) * playerTurnSpeed);			//Turn clockwise.
		else
			vel += sf::Vector2f(std::cos(toRadians(shipRotation)) * playerTurnSpeed, std::sin(toRadians(shipRotation)) * playerTurnSpeed);			//Turn counter clockwise.		
	}


	if(speed < playerMaxSpeed)																		//Accelerate with game-pad triggers.
		speed += (sf::Joystick::getAxisPosition(id, sf::Joystick::R) + 100)/200 * playerAcc;
	if(speed > playerMinSpeed)
		speed -= (sf::Joystick::getAxisPosition(id, sf::Joystick::Z) + 100)/200 * playerAcc;

	
	vel = clampVector(vel, speed);																	//Limit speed.
	pos += vel * dt;																				//Update position.


	if((pos.x > mapWidth && vel.x > 0) || (pos.x < 0 && vel.x < 0))		vel.x *= -1;				//Reflective walls.	
	if((pos.y > mapHeight && vel.y > 0) || (pos.y < 0 && vel.y < 0))	vel.y *= -1;


	body.setRotation(shipRotation);
	body.setPosition(pos);
	
	boostIndicator.setRotation(shipRotation);
	boostIndicator.setPosition(pos);

	shield.setRotation(rightStickAngle);
	shield.setPosition(pos);

	tailPtr->update(pos);

	if (shieldEncounter(sf::Vector2f(0,0), false))
	{
		std::cout << "\nShield blocked encounter.";
	}
	else
	{
		std::cout << "\nShield NOT blocked encounter.";
	}


}


void Player::draw()
{
	tailPtr->draw();
	window.draw(shield);
	window.draw(body);
	window.draw(boostIndicator);
}

Player::~Player()
{
	delete tailPtr;
}