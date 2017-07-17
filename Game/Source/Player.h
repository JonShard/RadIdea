#pragma once

#include "ParticleSystem.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>		//for Graphics stuff


class Player
{
	private:
		sf::CircleShape body;
		sf::CircleShape boostIndicator;
		Emitter* tailPtr;
		Emitter* explotionPtr;
		sf::Sprite shield;

		Projectile* projectiles[MAXPROJECTILES];

		int id;
		bool playerDead;
		float boost;
		float speed;
		float shipRotation;
		float shieldRotation;
		int activeProjectiles;
		float buttonTimeout;


		sf::Vector2f pos;
		sf::Vector2f vel;

	public:
		Player(int num);

		bool shieldEncounter(sf::Vector2f encPos, bool projectile); //True if player blocks encounter.
		void killPlayer();

		void update();
		void draw();

		~Player();

};