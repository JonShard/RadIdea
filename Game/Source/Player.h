#pragma once

#include "ParticleSystem.h"
#include "Projectile.h"
#include "Mine.h"
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
		Mine* mines[MAXMINES];

		int id;
		bool playerDead;
		float boost;
		float speed;
		float shipRotation;
		float shieldRotation;
		int activeProjectiles;
		int activeMines;
		float buttonTimeout;


		sf::Vector2f pos;
		sf::Vector2f vel;

	public:
		Player(int num);

		sf::Vector2f getPos();
		sf::Vector2f getProjectilePos(int index);
		sf::Vector2f getMinePos(int index);
		int getActiveProjectiles();
		int getActiveMines();
		bool shieldEncounter(sf::Vector2f encPos, bool projectile); //True if player blocks encounter.
		bool isAlive();
		void killPlayer();
		void splatterProjectile(int index);						//When blocked by other players shield.
		void explodeMine(int index);						//When blocked by other players shield.
		void update();
		void draw();

		~Player();

};