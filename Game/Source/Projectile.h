#pragma once

#include "ParticleSystem.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>		//for Graphics s



class Projectile
{
	private:
		sf::Sprite body;
		Emitter* tailPtr;

		sf::Vector2f pos;
		sf::Vector2f vel;

		bool projectileDead;

	public:
		Projectile(sf::Vector2f playerPos, sf::Vector2f playerVel, int playerId);

		void splatter();

		bool update();
		void draw();

		~Projectile();

};