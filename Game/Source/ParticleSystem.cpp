#include "ParticleSystem.h"
#include "Constants.h"
#include "HelpFunctions.h"
#include <cmath>
#include <iostream>


extern sf::RenderWindow window;						//The window that draws the graphics on the screen.
extern sf::Texture shipExhaust; 					//Global ship Exhaust texture from main.
extern float dt;									//Delta-time.


Particle::Particle(sf::Vector2f emitterPos, sf::Vector2f launchVel, float newEmitterLifeTime, float newParticleLifeTime, sf::Color & newStartColor, sf::Color & newEndColor,
		 float newStartScale, float newEndScale, float newAirResistance, float newRotationSpeed)
{
	pos = emitterPos;
	vel = launchVel;
													//Copy particle settings into particle:
	settings.particleLifeTime = newParticleLifeTime;
	settings.startColor = newStartColor;
	settings.endColor = newEndColor;
	settings.startScale = newStartScale;
	settings.endScale = newEndScale;
	settings.airResistance = newAirResistance;
	settings.maxRotationSpeed = newRotationSpeed;
													//Set a random rotation value within max:
	rotationSpeed = (settings.maxRotationSpeed * (randomNumber(0,1) ? (-1) : 1)) * ((randomNumber(2, 10))/10.0f); 
	lifeTime = settings.particleLifeTime;

	sprite.setTexture(shipExhaust);					//Set up sprite:
	sprite.setColor(settings.startColor);
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x/2.0f, sprite.getTexture()->getSize().y/2.0f));
	sprite.setScale(sf::Vector2f(newStartScale, newStartScale));
	sprite.setPosition(emitterPos);
	sprite.rotate(randomNumber(0, 360));			//Rotate to random angle to mix up apperance.

}


float Particle::update()
{
	
	float timeLeft = lifeTime / settings.particleLifeTime;	
	
	sf::Color newColor = settings.endColor;
	newColor -= sf::Color ( settings.endColor.r * timeLeft,
							settings.endColor.g * timeLeft,
							settings.endColor.b * timeLeft,
							settings.endColor.a * timeLeft);

	newColor += sf::Color ( settings.startColor.r * timeLeft,
							settings.startColor.g * timeLeft,
							settings.startColor.b * timeLeft,
							settings.startColor.a * timeLeft);

	
	sprite.setColor(newColor);


    sprite.setScale(settings.endScale * timeLeft + settings.startScale * timeLeft,
    				settings.endScale * timeLeft + settings.startScale * timeLeft);




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
	settings.emitterLifeTime = EMITTERLIFETIME;
	settings.startColor = STARTCOLOR;
	settings.endColor = ENDCOLOR;
	settings.startScale = STARTSCALE;
	settings.endScale = ENDSCALE;
	settings.airResistance = AIRRESISTANCE;
	settings.particleLifeTime = STARTLIFETIME;
	settings.maxRotationSpeed = MAXROTATIONSPEED;


	cooldown = settings.emitterCooldown;
	particleCount = 0;
	lifeTime = settings.emitterLifeTime;

}

Emitter::Emitter(sf::Vector2f emitterPos, float newLaunchSpeed, float newEmitterCooldown, float newMaxParticles, float newEmitterLifeTime, sf::Color newStartColor, sf::Color newEndColor,
		float newStartScale, float newEndScale, float newParticleLifeTime, float newAirResistance, float newMaxRotationSpeed)
{
	pos = emitterPos;

	settings.launchSpeed = newLaunchSpeed;
	settings.emitterCooldown = newEmitterCooldown;
	settings.maxParticles = newMaxParticles;
	settings.emitterLifeTime = newEmitterLifeTime;
	settings.startColor = newStartColor;
	settings.endColor = newEndColor;
	settings.startScale = newStartScale;
	settings.endScale = newEndScale;
	settings.airResistance = newAirResistance;
	settings.particleLifeTime = newParticleLifeTime;
	settings.maxRotationSpeed = newMaxRotationSpeed;


	cooldown = settings.emitterCooldown;
	particleCount = 0;
	lifeTime = settings.emitterLifeTime;

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

	    shootPos.x += (randomNumber(0, PARTICLEPOSOFFSET) - PARTICLEPOSOFFSET/2);
	    shootPos.y += (randomNumber(0, PARTICLEPOSOFFSET) - PARTICLEPOSOFFSET/2);

	    particles[0] = new Particle(shootPos, shootVel, settings.emitterLifeTime, settings.particleLifeTime, settings.startColor, settings.endColor,
		settings.startScale, settings.endScale, settings.airResistance, settings.maxRotationSpeed);
    }
}


bool Emitter::update(sf::Vector2f parrentPos)
{
	bool emitterIsAlive = true;

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
    	if(lifeTime >= 0)
    	{
    		shootParticle();
    		cooldown = settings.emitterCooldown;
  	  	}
	}

    cooldown -= particleCooldownTimer.restart().asSeconds();


	pos = parrentPos;



    if(settings.emitterLifeTime > 0)			//if emitter has limited lifetime.
    {
    	lifeTime -= dt;
    	if (lifeTime < -settings.particleLifeTime)//If emitter is dead and all particles has died.
    	{
    		emitterIsAlive = false;
    	}
    }

	return emitterIsAlive;
}

void Emitter::draw()
{
	for (int i = 0; i < particleCount; i++)
	{
		particles[i]->draw();
	}
}
