#pragma once

#include "ParticleSystem.h"
#include <SFML/Graphics.hpp>		//for Graphics stuff


class Player
{
	private:
		sf::CircleShape body;
		sf::CircleShape boostIndicator;
		Emitter* tailPtr;

		int id;
		float boost;
		float speed;
		float rotation;

		sf::Vector2f pos;
		sf::Vector2f vel;
		sf::Vector2f acc;

	public:
		Player(int num);

		void update();
		void draw();

		~Player();

};