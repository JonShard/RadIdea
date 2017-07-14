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
		Particle(sf::Vector2f emitterPos, sf::Vector2f launchVel, float newEmitterLifeTime, float newParticleLifeTime, sf::Color & newStartColor, sf::Color & newEndColor,
		 float newStartScale, float newEndScale, float newAirResistance, float newRotationSpeed);



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
		Emitter(sf::Vector2f emitterPos, float newLaunchSpeed, float newEmitterCooldown, float newMaxParticles, float newEmitterLifeTime, sf::Color newStartColor, sf::Color newEndColor,
		float newStartScale, float newEndScale, float newParticleLifeTime, float newAirResistance, float newMaxRotationSpeed);

		void shootParticle();
		bool update(sf::Vector2f parrentPos); 
		void draw();
};

