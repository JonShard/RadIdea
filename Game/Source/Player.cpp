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
	activeMines = 0;
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
		tailSettings.launchSpeed = 55;
		tailSettings.emitterCooldown = 0.004f;
		tailSettings.maxParticles = 300;
		tailSettings.emitterLifeTime = 0.0f;
		tailSettings.emissionArea = sf::Vector2f(10, 10);

		tailSettings.startColor = playerColor[id];
		tailSettings.endColor = sf::Color(255,0,0,0);
		tailSettings.startScale = 0.6f;
		tailSettings.endScale = 0.3f;
		tailSettings.particleLifeTime = 0.8f;
		tailSettings.airResistance = 1;
		tailSettings.maxRotationSpeed =  0.000001f;
	tailPtr = new Emitter(pos, tailSettings);



	ParticleSettings explotionSettings;
			explotionSettings.particleTexture = circleParticleTexture;
			explotionSettings.launchSpeed = 600;
			explotionSettings.emitterCooldown = 0.001f;
			explotionSettings.maxParticles = 600;
			explotionSettings.emitterLifeTime = 0.58f;
			explotionSettings.emissionArea = sf::Vector2f(5, 5);


			explotionSettings.startColor = sf::Color(255,100,0);
			explotionSettings.endColor = sf::Color(30,30,255, 0);
			explotionSettings.startScale = 1.0f;
			explotionSettings.endScale = 6.1f;
			explotionSettings.particleLifeTime = 0.6f;
			explotionSettings.airResistance = 0.999f;
			explotionSettings.maxRotationSpeed =  0.01f;
	explotionPtr = new Emitter(pos, explotionSettings);

}


sf::Vector2f Player::getPos()
{	return pos;		}



sf::Vector2f Player::getProjectilePos(int index)
{
	if(index < activeProjectiles && projectiles[index] != NULL)					//If that projectile exist.
	{
		return projectiles[index]-> getPos();
	}

	return sf::Vector2f(0,0);
}


sf::Vector2f Player::getMinePos(int index)
{
	if(index < activeMines && mines[index] != NULL)					//If that projectile exist.
	{
		return mines[index]-> getPos();
	}

	return sf::Vector2f(0,0);
}

int Player::getActiveProjectiles()
{	return activeProjectiles; 	}


int Player::getActiveMines()
{	return activeMines; 	}


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


bool Player::isAlive()
{	return !playerDead;		}



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



void Player::splatterProjectile(int index)
{
	projectiles[index]-> splatter();
}


void Player::explodeMine(int index)
{
	mines[index]-> explode();
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



		sf::Vector2f velAdjustment = sf::Vector2f(0,0);

		if (std::abs(leftStickAngle - shipRotation) < 180)			//Steers the player left or right depending on current cource:
		{
			if (leftStickAngle < shipRotation)
				velAdjustment = sf::Vector2f(std::cos(toRadians(shipRotation + 180)) * playerTurnSpeed, std::sin(toRadians(shipRotation + 180)) * playerTurnSpeed);			//Turn clockwise.
			else
				velAdjustment = sf::Vector2f(std::cos(toRadians(shipRotation)) * playerTurnSpeed, std::sin(toRadians(shipRotation)) * playerTurnSpeed);			//Turn counter clockwise.
		}
		else
		{
			if (leftStickAngle > shipRotation)
			
				velAdjustment = sf::Vector2f(std::cos(toRadians(shipRotation + 180)) * playerTurnSpeed, std::sin(toRadians(shipRotation + 180)) * playerTurnSpeed);			//Turn clockwise.
			else
				velAdjustment = sf::Vector2f(std::cos(toRadians(shipRotation)) * playerTurnSpeed, std::sin(toRadians(shipRotation)) * playerTurnSpeed);			//Turn counter clockwise.		
		}

		velAdjustment = (velAdjustment * (1- speed / playerMaxSpeed)) + (velAdjustment * 1.2f);
		vel += velAdjustment;



		if (speed < playerMaxSpeed)												//Accelerate with game-pad triggers.
			speed += (sf::Joystick::getAxisPosition(id, sf::Joystick::R) + 100)/200 * playerAcc;	//Bumpers to controll speed.
		

		if (speed > playerMinSpeed)
			speed -= (sf::Joystick::getAxisPosition(id, sf::Joystick::Z) + 100)/200 * playerAcc;



		if (bool pressed = sf::Joystick::isButtonPressed(0, 0) &&
			boost > PROJECTILECOST &&
			activeProjectiles < MAXPROJECTILES &&
			buttonTimeout <= 0)
		{
			boost -= PROJECTILECOST;
			activeProjectiles++;

			for (int i = activeProjectiles; i > 0; i--)
	    	{
	        	projectiles[i] = projectiles[i - 1];       //Shift all the projectiles one step further to make space for new projecticle.
	    	}

			projectiles[0] = new Projectile(pos, vel, id);
			buttonTimeout = buttonTimeoutTime;

		}


		if (bool pressed = sf::Joystick::isButtonPressed(0, 1) &&
			boost > MINECOST &&
			activeMines < MAXMINES &&
			buttonTimeout <= 0)
		{
			boost -= MINECOST;
			activeMines++;

			for (int i = activeMines; i > 0; i--)
	    	{
	        	mines[i] = mines[i - 1];       //Shift all the mines one step further to make space for new mine.
	    	}

			mines[0] = new Mine(pos, vel, id);
			buttonTimeout = buttonTimeoutTime;

		}




	}



	for (int i = activeProjectiles -1; i >= 0; i--)								//Update 
	{
		if (projectiles[i]-> update())
		{
			std::cout << "\nDeleting player" << id << "'s projectile no " << activeProjectiles;

			activeProjectiles--;
			delete projectiles[activeProjectiles];
			projectiles[activeProjectiles] = NULL;
		}
	}

	for (int i = activeMines -1; i >= 0; i--)								//Update 
	{
		if (mines[i]-> update())
		{
			std::cout << "\nDeleting player" << id << "'s mine no " << activeMines;

			activeMines--;
			delete mines[activeMines];
			mines[activeMines] = NULL;
		}
	}

																				//Update position.

	if(magnitude(sf::Vector2f(mapWidth/2, mapHeight/2), pos) > mapWidth/2)	vel *= -1.0f;				//Reflective walls.	

	if (!playerDead)vel = clampVector(vel, speed);
	vel *= playerDeAcc;																//Limit speed.
	pos += vel * dt;	


	body.setRotation(shipRotation);
	body.setPosition(pos);

	boostIndicator.setRadius(bodyRadius * boost);
	boostIndicator.setOrigin(sf::Vector2f(bodyRadius * boost, bodyRadius * boost));
	boostIndicator.setRotation(shipRotation);
	boostIndicator.setPosition(pos);

	shield.setRotation(rightStickAngle + 180);
	shield.setPosition(pos);

	tailPtr-> update(pos, -vel * (1- (speed/playerMaxSpeed)) * 1.6f);


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
		tailSettings.emissionArea = sf::Vector2f(30, 30);

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
	tailPtr->draw();

	if (playerDead && explotionPtr != NULL)
	{
		explotionPtr->draw();
	}
	
	for (int i = 0; i < activeProjectiles; i++)
	{
		if (projectiles[i] != NULL)
		{
			projectiles[i] ->draw();
		}
	} 
	for (int i = 0; i < activeMines; i++)
	{
		if (mines[i] != NULL)
		{
			mines[i] ->draw();
		}
	} 

	window.draw(body);
	window.draw(boostIndicator);
	if(!playerDead)
	{
		window.draw(shield);
	}
}

Player::~Player()
{
	delete tailPtr;
	
	if (explotionPtr != NULL) 
		delete explotionPtr;
}