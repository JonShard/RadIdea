#include "Mine.h"

#include "HelpFunctions.h"


extern sf::Texture mineTexture;
extern sf::Texture circleParticleTexture;
extern sf::Texture squareParticleTexture;
extern sf::RenderWindow window;						//The window that draws the graphics on the screen.
extern float dt;									//Delta-time.



Mine::Mine(sf::Vector2f playerPos, sf::Vector2f playerVel, int playerId)
{
	mineDead = false;
	lifeTime = MINELIFETIME;

	ParticleSettings areaSettings;
		areaSettings.particleTexture = squareParticleTexture;
		areaSettings.launchSpeed = 800;
		areaSettings.emitterCooldown = 0.01f;
		areaSettings.maxParticles = 200;
		areaSettings.emitterLifeTime = 0.0f;
		areaSettings.emissionArea = sf::Vector2f(20, 20);

		areaSettings.startColor = playerColor[playerId];
		areaSettings.endColor = sf::Color(250, 60, 90, 100);
		areaSettings.startScale = 1.2f;
		areaSettings.endScale = 0.0f;
		areaSettings.particleLifeTime = 0.4f;
		areaSettings.airResistance = 0.996f;
		areaSettings.maxRotationSpeed =  0.000001f;	
	areaPtr = new Emitter(pos, areaSettings);


	body.setTexture(mineTexture);					//Set up sprite:
	body.setColor(playerColor[playerId]);
	body.setOrigin(sf::Vector2f(body.getTexture()->getSize().x/2.0f, body.getTexture()->getSize().y/2.0f));
	body.setScale(sf::Vector2f(PROJECTILESCALE, PROJECTILESCALE));
	body.setPosition(playerPos);

	body.rotate(randomNumber(0, 360));			//Rotate to random angle to mix up apperance.

	pos = playerPos;
	vel = playerVel * 0.8f;
}


sf::Vector2f Mine::getPos()
{
	return pos;
}


void Mine::explode()
{
	mineDead = true;

	ParticleSettings areaSettings;
		areaSettings.particleTexture = squareParticleTexture;
		areaSettings.launchSpeed = 1200;
		areaSettings.emitterCooldown = 0.000001f;
		areaSettings.maxParticles = 1000;
		areaSettings.emitterLifeTime = 2;
		areaSettings.emissionArea = sf::Vector2f(20, 20);

		areaSettings.startColor = sf::Color(200, 60, 50, 255);
		areaSettings.endColor = sf::Color(10, 60, 200, 1);
		areaSettings.startScale = 1.2f;
		areaSettings.endScale = 6.0f;
		areaSettings.particleLifeTime = 2;
		areaSettings.airResistance = 0.990f;
		areaSettings.maxRotationSpeed =  0.000001f;	
	areaPtr-> changeSettings(areaSettings);
}


void Mine::extinguish()
{
	mineDead = true;
	
	ParticleSettings areaSettings;
		areaSettings.particleTexture = squareParticleTexture;
		areaSettings.launchSpeed = 0;
		areaSettings.emitterCooldown = 10;
		areaSettings.maxParticles = 0;
		areaSettings.emitterLifeTime = 0.0f;
		areaSettings.emissionArea = sf::Vector2f(0, 0);

		areaSettings.startColor = sf::Color(0, 0, 0, 0);
		areaSettings.endColor = sf::Color(0, 0, 0, 0);
		areaSettings.startScale = 0;
		areaSettings.endScale = 0;
		areaSettings.particleLifeTime = 0;
		areaSettings.airResistance = 0;
		areaSettings.maxRotationSpeed = 0;
	areaPtr-> changeSettings(areaSettings);
}


bool Mine::update()
{
	bool particlesAlive = true;

	vel *= MINEDEACC;
	pos += vel *dt;


	body.setPosition(pos);
	body.rotate(MINEROTATION);

	
	particlesAlive = areaPtr-> update(pos, sf::Vector2f(0,0));

	if(lifeTime <= 0)
	{
		extinguish();
	}
	if(lifeTime <= -0.4f) 
	{
		particlesAlive = false;
	}
	lifeTime -= dt;


	return !particlesAlive;
}


void Mine::draw()
{
	areaPtr->draw();
	if (!mineDead) window.draw(body);
}


Mine::~Mine()
{
	delete areaPtr;
}
