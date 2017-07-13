#include "ParticleSystem.h"
#include "Constants.h"
#include "HelpFunctions.h"
#include <cmath>
#include <iostream>


extern sf::RenderWindow window;						//The window that draws the graphics on the screen.
extern sf::Texture shipExhaust; 					//Global ship Exhaust texture from main.
extern float dt;									//Delta-time.


Particle::Particle(sf::Vector2f emitterPos, sf::Vector2f launchVel, float newLifeTime, sf::Color & newStartColor, sf::Color & newEndColor,
		 float newStartScale, float newEndScale, float newAirResistance, float newRotationSpeed)
{
	pos = emitterPos;
	vel = launchVel;
													//Copy particle settings into particle:
	lifeTime = newLifeTime;
	settings.startColor = newStartColor;
	settings.endColor = newEndColor;
	settings.startScale = newStartScale;
	settings.endScale = newEndScale;
	settings.airResistance = newAirResistance;
	settings.maxRotationSpeed = newRotationSpeed;
													//Set a random rotation value within max:
	rotationSpeed = (settings.maxRotationSpeed * (randomNumber(0,1) ? (-1) : 1)) * ((randomNumber(2, 10))/10.0f); 

	sprite.setTexture(shipExhaust);					//Set up sprite:
	sprite.setColor(settings.startColor);
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x/2.0f, sprite.getTexture()->getSize().y/2.0f));
	sprite.setScale(sf::Vector2f(newStartScale, newStartScale));
	sprite.setPosition(emitterPos);
	sprite.rotate(randomNumber(0, 360));			//Rotate to random angle to mix up apperance.

}


float Particle::update()
{
	vel *= settings.airResistance;
	pos += vel * dt;

	sprite.setPosition(pos);
	sprite.rotate(rotationSpeed * dt);


	lifeTime -= dt;									//Reduce time particle has left.
	return lifeTime;
}


void Particle::draw()
{
	window.draw(sprite);
}




Emitter::Emitter()
{

	pos = emitterStartPosition;

	settings.launchSpeed = LAUNCHSPEED;
	settings.emitterCooldown = EMITTERCOOLDOWN;
	settings.maxParticles = MAXPARTICLES;
	settings.startColor = STARTCOLOR;
	settings.endColor = ENDCOLOR;
	settings.startScale = STARTSCALE;
	settings.endScale = ENDSCALE;
	settings.airResistance = AIRRESISTANCE;
	settings.startLifeTime = STARTLIFETIME;
	settings.maxRotationSpeed = MAXROTATIONSPEED;


	cooldown = settings.emitterCooldown;
	particleCount = 0;

}

Emitter::Emitter(sf::Vector2f emitterPos, float newLaunchSpeed, float newEmitterCooldown, float newMaxParticles, sf::Color newStartColor, sf::Color newEndColor,
		 float newStartScale, float newEndScale, float newStartLifeTime, float newAirResistance, float newMaxRotationSpeed)
{
	pos = emitterPos;

	settings.launchSpeed = newLaunchSpeed;
	settings.emitterCooldown = newEmitterCooldown;
	settings.maxParticles = newMaxParticles;
	settings.startColor = newStartColor;
	settings.endColor = newEndColor;
	settings.startScale = newStartScale;
	settings.endScale = newEndScale;
	settings.airResistance = newAirResistance;
	settings.startLifeTime = newStartLifeTime;
	settings.maxRotationSpeed = newMaxRotationSpeed;


	cooldown = settings.emitterCooldown;
	particleCount = 0;

}


void Emitter::shootParticle()
{
	if (particleCount < settings.maxParticles)
    {
	    sf::Vector2f shootVel;

	    particleCount++;
	    for (int i = particleCount; i > 0; i--)
	    {
	        particles[i] = particles[i - 1];       //Shift all the particles one step further to make space for new particle.

	    }


	    int angle = randomNumber(0, 360);			//Set a random direction to shoot the particle.

	    shootVel.x = std::cos(angle) * settings.launchSpeed;
	    shootVel.y = std::sin(angle) * settings.launchSpeed;

	    sf::Vector2f shootPos = pos;

	    std::cout << "\n RandomNumber (4 - 10): " << randomNumber(4,10);
	    shootPos.x += (randomNumber(0, PARTICLEPOSOFFSET) - PARTICLEPOSOFFSET/2);
	    shootPos.y += (randomNumber(0, PARTICLEPOSOFFSET) - PARTICLEPOSOFFSET/2);

	    particles[0] = new Particle(shootPos, shootVel, settings.startLifeTime, settings.startColor, settings.endColor,
		settings.startScale, settings.endScale, settings.airResistance, settings.maxRotationSpeed);
    }
}


void Emitter::update(sf::Vector2f parrentPos)
{
	
	for (int i = 0; i < particleCount; i++)		//Go through particles, if any should be dead, kill em.
	{
		if (particles[i]->update() < 0) 
		{
			delete particles[particleCount-1];
			particleCount--;
		}
	}

	if (cooldown < 0)							//if done wiht cooldown, shoot new particle. 
    {											//Note: if cooldown is less than dt. It will shoot 1 particle per frame.
    	shootParticle();
    	cooldown = settings.emitterCooldown;
	}

    cooldown -= particleCooldownTimer.restart().asSeconds();

	pos = parrentPos;
}

void Emitter::draw()
{
	for (int i = 0; i < particleCount; i++)
	{
		particles[i]->draw();
	}
}