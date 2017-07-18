#pragma once
#include "Constants.h"
#include <SFML/Graphics.hpp>		//for Graphics stuff


struct ParticleSettings
{
							//Emitter values
		float launchSpeed;
		float emitterCooldown;
		float maxParticles;
		float emitterLifeTime;

							//Particle values
		sf::Texture particleTexture;
		sf::Color startColor;
		sf::Color endColor;
		float startScale;
		float endScale;
		float particleLifeTime;
		float airResistance;
		float maxRotationSpeed;

};


class Particle
{
	private:
		ParticleSettings settings;
		sf::Vector2f pos;
		sf::Vector2f vel;
		sf::Vector2f acc;
		sf::Sprite sprite;

		float lifeTime;
		float rotationSpeed;


	public:
		Particle(sf::Vector2f emitterPos, sf::Vector2f launchVel, ParticleSettings newSettings);

		float update();
		void draw();

};


class Emitter
{
	private:
		ParticleSettings settings;
		sf::Vector2f pos;
		Particle* particles[MAXPARTICLES];
		
		sf::Clock particleCooldownTimer;                     //Used to time the spawns of particles.
		float cooldown;
		float lifeTime;
		int particleCount;


	public:
		Emitter();
		Emitter(sf::Vector2f emitterPos, ParticleSettings newSettings);

		void changeSettings(ParticleSettings newSettings);
		void shootParticle(sf::Vector2f grandParrentVel);
		
		bool update(sf::Vector2f parrentPos, sf::Vector2f parrentVel); 
		void draw();
};

