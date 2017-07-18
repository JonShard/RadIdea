#pragma once

#include <SFML/Graphics.hpp>		//for Graphics stuff

//Window
const int windowHeight = 800;
const int windowWidth = 800;

enum State
{
	inGame, startMenu, playerSelect
};


//Game
const float buttonTimeoutTime = 0.15f;


//Map
const int mapHeight = 4500;
const int mapWidth = 4500;


//Player
const float bodyRadius = 34;
const float boostIndicatorRadius = 26;
const float playerMaxSpeed = 1300;
const float playerMinSpeed = 600;
const float playerAcc = 25;
const float playerDeAcc = 0.98f;
const float playerTurnSpeed = 18;
const int maxPlayers = 4;
const sf::Color playerDeadColor = sf::Color(30, 30, 30, 10);
const sf::Color playerColor[4] = {	sf::Color(51, 102, 255),
									sf::Color(255, 0, 255),
									sf::Color(225, 142, 80),
									sf::Color(0, 204, 102)};

const float SHIELDSCALE = 1.4f;
const float SHIELDOPACITY = 0.8f;
const int SHIELDCOVERAGE = 60;//Degrees. How wide is the shield.

const int MAXPROJECTILES = 50;
const float PROJECTILECOST = 0.0f;
const float PROJECTILESCALE = 2.0f;
const float PROJECTILESPEED = 1300;
const float PROJECTILELIFETIME = 2;
const float PROJECTILERADIUS = 50;		//Used to check collision. Not set sprite size.


//Boids




//Particles default values
const float LAUNCHSPEED = 180;
const int MAXPARTICLES = 10000;
const float EMITTERCOOLDOWN = 0.0002f;			//Time between each particle launch. 0.01f
const float EMITTERLIFETIME = 0.0f;				//Time the emitter will remain alive. -1 = forever.

const sf::Color STARTCOLOR = sf::Color (255, 20, 20, 225);
const sf::Color ENDCOLOR = sf::Color (20, 20, 255, 120);
const float STARTSCALE = 1.0f;
const float ENDSCALE = 0.1f;
const float AIRRESISTANCE = 0.996f;
const float STARTLIFETIME = 6.0f;
const float MAXROTATIONSPEED = 0.000001f;


const sf::Vector2f emitterStartPosition = sf::Vector2f(mapWidth / 2, mapHeight / 2);
const int PARTICLEPOSOFFSET = 25;
