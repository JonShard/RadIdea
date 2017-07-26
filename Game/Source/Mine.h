#pragma once

#include "ParticleSystem.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>		//for Graphics s



class Mine
{
	private:
		sf::Sprite body;
		Emitter* areaPtr;

		sf::Vector2f pos;
		sf::Vector2f vel;

		bool mineDead;
		float lifeTime;

	public:
		Mine(sf::Vector2f playerPos, sf::Vector2f playerVel, int playerId);
		sf::Vector2f getPos();
		void explode();
		void extinguish();

		bool update();
		void draw();

		~Mine();

};