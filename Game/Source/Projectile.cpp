#include "Projectile.h"
#include "HelpFunctions.h"
#include <cmath>


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
{ 	return pos;		}

void Projectile::splatter()
{
	projectileDead = true;
}

void Projectile::extinguish()
{
	//Wait for tail to run out before killing object.

	ParticleSettings tailSettings;
		tailSettings.particleTexture = bowParticleTexture;
		tailSettings.launchSpeed = 0;
		tailSettings.emitterCooldown = 1;
		tailSettings.maxParticles = 0;
		tailSettings.emitterLifeTime = 0.0f;

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

	particlesAlive = tailPtr-> update(pos, sf::Vector2f(0,0));
	body.setPosition(pos);


	if(lifeTime <= 0) extinguish();
	lifeTime -= dt;

	return (projectileDead && !particlesAlive);
}


void Projectile::draw()
{
	if (!projectileDead) window.draw(body);
	tailPtr->draw();
}


Projectile::~Projectile()
{
	delete tailPtr;
}