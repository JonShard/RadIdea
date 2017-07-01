#pragma once

#include <SFML/Graphics.hpp>		//for Graphics stuff


//Window
const int windowHeight = 800;
const int windowWidth = 800;




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
									sf::Color(255, 102, 0),
									sf::Color(0, 204, 102),};


//Boids




//Particles



//Map
const int mapHeight = 3000;
const int mapWidth = 3000;