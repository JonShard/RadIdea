#include "Player.h"
#include "Constants.h"
#include "HelpFunctions.h"
#include <cmath>
#include <iostream>

extern sf::RenderWindow window;						//The window that draws the graphics on the screen.
extern float dt;
extern int playerCount;
extern sf::Texture shieldTexture;
extern sf::Texture squareParticleTexture;
extern sf::Texture circleParticleTexture;


Player::Player(int num)
{
	id = num;
	playerDead = false;
	pos = sf::Vector2f(mapWidth/2 , mapHeight/2);
	vel = sf::Vector2f(0,0);
	
	shipRotation = 0;
	boost = 0.99f;
	speed = playerMaxSpeed;
	activeProjectiles = 0;
	buttonTimeout = 0;

 

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

	ParticleSettings tailSettings;
		tailSettings.particleTexture = squareParticleTexture;
		tailSettings.launchSpeed = 30;
		tailSettings.emitterCooldown = 0.005f;
		tailSettings.maxParticles = 300;
		tailSettings.emitterLifeTime = 0.0f;

		tailSettings.startColor = playerColor[id];
		tailSettings.endColor = sf::Color(255,0,0,200);
		tailSettings.startScale = 0.4f;
		tailSettings.endScale = 0.0f;
		tailSettings.particleLifeTime = 0.7f;
		tailSettings.airResistance = 0.996f;
		tailSettings.maxRotationSpeed =  0.000001f;
	tailPtr = new Emitter(pos, tailSettings);

	ParticleSettings explotionSettings;
			explotionSettings.particleTexture = circleParticleTexture;
			explotionSettings.launchSpeed = 600;
			explotionSettings.emitterCooldown = 0.001f;
			explotionSettings.maxParticles = 600;
			explotionSettings.emitterLifeTime = 0.58f;

			explotionSettings.startColor = sf::Color(255,100,0);
			explotionSettings.endColor = sf::Color(30,30,255, 0);
			explotionSettings.startScale = 1.0f;
			explotionSettings.endScale = 6.1f;
			explotionSettings.particleLifeTime = 0.6f;
			explotionSettings.airResistance = 0.999f;
			explotionSettings.maxRotationSpeed =  0.01f;
	explotionPtr = new Emitter(pos, explotionSettings);

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



void Player::killPlayer()
{
	playerDead = true;
	body.setFillColor(body.getFillColor() - playerDeadColor);
	body.setOutlineColor(body.getOutlineColor() - playerDeadColor);
	boostIndicator.setFillColor(boostIndicator.getFillColor() - playerDeadColor - 
																playerDeadColor - 
																playerDeadColor);


	//Activarte player # label.

}




void Player::update()
{			
	float leftStickAngle = 0;
	float rightStickAngle = 0;


	if (buttonTimeout > 0)
	{
		buttonTimeout -= dt;
	}

	if (!playerDead)
	{	
		leftStickAngle = getAngle(sf::Vector2f(0,0),		//Get input form left stick into euler angles.
									sf::Vector2f(sf::Joystick::getAxisPosition(id, sf::Joystick::X), 
									sf::Joystick::getAxisPosition(id, sf::Joystick::Y)));
		rightStickAngle = getAngle(sf::Vector2f(0,0),		//Get input form left stick into euler angles.
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



		for (int i = 0; i < activeProjectiles; i++)
		{
			if (projectiles[i]-> update())
			{
				activeProjectiles--;
				delete projectiles[i];
				projectiles[i] = NULL;
			}
		}

		if (bool pressed = sf::Joystick::isButtonPressed(0, 0) &&
			boost > PROJECTILECOST &&
			activeProjectiles < MAXPROJECTILES &&
			buttonTimeout <= 0)
		{
			boost -= PROJECTILECOST;
			projectiles[activeProjectiles] = new Projectile(pos, vel, id);
			activeProjectiles++;
			buttonTimeout = buttonTimeoutTime;

		}



	}


																				//Update position.



	if((pos.x > mapWidth && vel.x > 0) || (pos.x < 0 && vel.x < 0))		vel.x *= -1;				//Reflective walls.	
	if((pos.y > mapHeight && vel.y > 0) || (pos.y < 0 && vel.y < 0))	vel.y *= -1;

	if (!playerDead)vel = clampVector(vel, speed);
	vel *= playerDeAcc;																//Limit speed.
	pos += vel * dt;	


	body.setRotation(shipRotation);
	body.setPosition(pos);

	boostIndicator.setRadius(bodyRadius * boost);
	boostIndicator.setOrigin(sf::Vector2f(bodyRadius * boost, bodyRadius * boost));
	boostIndicator.setRotation(shipRotation);
	boostIndicator.setPosition(pos);

	shield.setRotation(rightStickAngle);
	shield.setPosition(pos);

	tailPtr->update(pos, sf::Vector2f(0,0));


	if (playerDead && (explotionPtr != NULL && explotionPtr->update(pos, vel * 0.6f) == false)) 	//2.0f scaler for cool effect.
	{
		delete explotionPtr;
		explotionPtr = NULL;

	ParticleSettings tailSettings;
		tailSettings.particleTexture = circleParticleTexture;
		tailSettings.launchSpeed = 90;
		tailSettings.emitterCooldown = 0.015f;
		tailSettings.maxParticles = 400;
		tailSettings.emitterLifeTime = 0.0f;

		tailSettings.startColor = sf::Color(255,100,0);
		tailSettings.endColor = sf::Color(30,30,255, 0);
		tailSettings.startScale = 0.5f;
		tailSettings.endScale = 1.8f;
		tailSettings.particleLifeTime = 2.7f;
		tailSettings.airResistance = 0.99f;
		tailSettings.maxRotationSpeed =  0.000001f;
	tailPtr->changeSettings(tailSettings);
	}

}


void Player::draw()
{
	if(!playerDead)
	{
		window.draw(shield);
	}
	tailPtr->draw();
	window.draw(body);
	window.draw(boostIndicator);


	if (playerDead && explotionPtr != NULL)
	{
		explotionPtr->draw();
	}
	
	for (int i = 0; i < activeProjectiles; i++)
	{
		projectiles[i] ->draw();
	} 
}

Player::~Player()
{
	delete tailPtr;
	
	if (explotionPtr != NULL) 
		delete explotionPtr;
}