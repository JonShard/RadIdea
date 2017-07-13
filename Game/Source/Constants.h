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



//Map
const int mapHeight = 3000;
const int mapWidth = 3000;


//Player
const float bodyRadius = 34;
const float boostIndicatorRadius = 26;
const float playerMaxSpeed = 1300;
const float playerMinSpeed = 600;
const float playerAcc = 25;
const float playerTurnSpeed = 18;
const int maxPlayers = 4;

const sf::Color playerColor[4] = {	sf::Color(51, 102, 255),
									sf::Color(255, 0, 255),
									sf::Color(225, 142, 0),
									sf::Color(0, 204, 102)};

const float SHIELDSCALE = 1.4f;


//Boids




//Particles default values
const float LAUNCHSPEED = 180;
const int MAXPARTICLES = 10000;
const float EMITTERCOOLDOWN = 0.0002f;			//Time between each particle launch. 0.01f

const sf::Color STARTCOLOR = sf::Color (255, 20, 20, 225);
const sf::Color ENDCOLOR = sf::Color (20, 20, 255, 120);
const float STARTSCALE = 1.0f;
const float ENDSCALE = 0.1f;
const float AIRRESISTANCE = 0.996f;
const float STARTLIFETIME = 6.0f;
const float MAXROTATIONSPEED = 0.000001f;


const sf::Vector2f emitterStartPosition = sf::Vector2f(mapWidth / 2, mapHeight / 2);
const int PARTICLEPOSOFFSET = 25;
