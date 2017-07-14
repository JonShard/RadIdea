#pragma once

#include "ParticleSystem.h"
#include <SFML/Graphics.hpp>		//for Graphics stuff


class Player
{
	private:
		sf::CircleShape body;
		sf::CircleShape boostIndicator;
		Emitter* tailPtr;
		sf::Sprite shield;

		int id;
		float boost;
		float speed;
		float shipRotation;
		float shieldRotation;

		sf::Vector2f pos;
		sf::Vector2f vel;
		sf::Vector2f acc;

	public:
		Player(int num);

		bool shieldEncounter(sf::Vector2f encPos, bool projectile); //True if player blocks encounter.
		void killPlayer();

		void update();
		void draw();

		~Player();

};