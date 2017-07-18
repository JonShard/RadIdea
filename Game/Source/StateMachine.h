#pragma once
#include "Constants.h"
#include <SFML/Graphics.hpp>		//for Graphics stuff
#include "Player.h"


class StateMachine
{
private:
	static void updateCollisions();			//Between Player-Player, Player-Projectile, Player-Boids, Boids-Projectile.
	static void updateAcceleration();		//Due to gravity wells.

public:
	static void update();

	static void updateInGame();
	static void updateStartMenu();
	static void updatePlayerSelect();
	
};

