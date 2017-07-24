#include "Projectile.h"
#include "HelpFunctions.h"
#include <cmath>
#include <iostream>


extern sf::Texture bowParticleTexture;
extern sf::Texture circleParticleTexture;
extern sf::RenderWindow window;						//The window that draws the graphics on the screen.
extern float dt;									//Delta-time.


Projectile::Projectile(sf::Vector2f playerPos, sf::Vector2f playerVel, int playerId)
{
	projectileDead = false;
	lifeTime = PROJECTILELIFETIME;

	ParticleSettings tailSettings;
		tailSettings.particleTexture = bowParticleTexture;
		tailSettings.launchSpeed = 10;
		tailSettings.emitterCooldown = 0.01f;
		tailSettings.maxParticles = 300;
		tailSettings.emitterLifeTime = 0.0f;
		tailSettings.emissionArea = sf::Vector2f(10, 10);

		tailSettings.startColor = sf::Color(255,255,0,200);
		tailSettings.endColor = sf::Color(0,100,0,100);
		tailSettings.startScale = 1.2f;
		tailSettings.endScale = 0.0f;
		tailSettings.particleLifeTime = 1.0f;
		tailSettings.airResistance = 0.996f;
		tailSettings.maxRotationSpeed =  0.000001f;	
	tailPtr = new Emitter(pos, tailSettings);


	body.setTexture(circleParticleTexture);					//Set up sprite:
	body.setColor(playerColor[playerId]);
	body.setOrigin(sf::Vector2f(body.getTexture()->getSize().x/2.0f, body.getTexture()->getSize().y/2.0f));
	body.setScale(sf::Vector2f(PROJECTILESCALE, PROJECTILESCALE));
	body.setPosition(playerPos);

	float angle = getAngle(sf::Vector2f(0,0), playerVel);
	body.setRotation(angle);

	pos = playerPos;
	vel = playerVel + clampVector(playerVel, PROJECTILESPEED);

}


sf::Vector2f Projectile::getPos()
{ 	
	return (projectileDead)? sf::Vector2f(0,0) : pos;		
}

void Projectile::splatter()
{
	ParticleSettings tailSettings;
		tailSettings.particleTexture = circleParticleTexture;
		tailSettings.launchSpeed = 400;
		tailSettings.emitterCooldown = 0.00001f;
		tailSettings.maxParticles = 300;
		tailSettings.emitterLifeTime = 0.8f;	//Crashes if this is more than 0; Memory access violation.
		tailSettings.emissionArea = sf::Vector2f(10, 10);

		tailSettings.startColor = sf::Color(255,255,0,200);
		tailSettings.endColor = sf::Color(0,100,0,10);
		tailSettings.startScale = 3.2f;
		tailSettings.endScale = 0.2f;
		tailSettings.particleLifeTime = 1.4f;
		tailSettings.airResistance = 0.996f;
		tailSettings.maxRotationSpeed = 0.000001f;
	tailPtr-> changeSettings(tailSettings);

	projectileDead = true;
}

void Projectile::extinguish()
{
	ParticleSettings tailSettings;
		tailSettings.particleTexture = bowParticleTexture;
		tailSettings.launchSpeed = 0;
		tailSettings.emitterCooldown = 1;
		tailSettings.maxParticles = 0;
		tailSettings.emitterLifeTime = 0.0f;
		tailSettings.emissionArea = sf::Vector2f(10, 10);


		tailSettings.startColor = sf::Color(255,255,0,200);
		tailSettings.endColor = sf::Color(0,100,0,100);
		tailSettings.startScale = 1.2f;
		tailSettings.endScale = 0.0f;
		tailSettings.particleLifeTime = 1.0f;
		tailSettings.airResistance = 0.996f;
		tailSettings.maxRotationSpeed = 0.000001f;
	tailPtr-> changeSettings(tailSettings);
	projectileDead = true;
}


bool Projectile::update()
{
	bool particlesAlive = true;
	pos += vel * dt;
	
	if (projectileDead) particlesAlive = tailPtr-> update(pos, vel * -1.3f);
	else 				particlesAlive = tailPtr-> update(pos, sf::Vector2f(0,0));

	body.setPosition(pos);


	if(lifeTime <= 0)
	{
		extinguish();
	}
	if(lifeTime <= -1.4f) 
	{
		particlesAlive = false;
	}
	lifeTime -= dt;


	return !particlesAlive;
}


void Projectile::draw()
{
	tailPtr->draw();
	if (!projectileDead) window.draw(body);
}


Projectile::~Projectile()
{
	delete tailPtr;
}