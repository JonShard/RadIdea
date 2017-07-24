#include <SFML/Graphics.hpp>		//for Graphics stuff
#include <iostream>			//Used for printing to screen(debug) and some input.
#include "Player.h"			//The controllable player.
#include "ParticleSystem.h"
#include "HelpFunctions.h"
#include "StateMachine.h"
#include "Constants.h"


sf::View cam(sf::Vector2f(0,0), sf::Vector2f(mapWidth, mapHeight));
sf::ContextSettings settings;					//The settings Context, all the config info the window needs to run.
sf::RenderWindow window;						//The window that draws the graphics on the screen.
sf::Clock deltaTime;
sf::CircleShape mapOutline;
Emitter* backgroundParticlesPtr;
Emitter* backgroundSmokePtr;



sf::Font font;									//The font imported from file used to print text on Screen.
sf::Texture squareParticleTexture;
sf::Texture circleParticleTexture;
sf::Texture bowParticleTexture;
sf::Texture smokeParticleTexture;
sf::Texture shieldTexture;
sf::Texture mineTexture;


State state = inGame;								//Set to in-game.
float dt = 0;
float buttonTimeout = 0;

int playerCount;
Player* players[maxPlayers];


int main ()		  														
{
	srand(static_cast<int>(time(NULL)));
	window.create(sf::VideoMode(windowWidth, windowHeight), "Game", //opens the window and sets the size.
						 sf::Style::Titlebar | sf::Style::Close);	

	cam.setCenter(mapWidth/2.0f, mapHeight/2.0f);
	window.setView(cam);
	window.setFramerateLimit(60);

	mapOutline.setRadius(mapWidth / 2);
	mapOutline.setFillColor(sf::Color(0,0,0,0));
	mapOutline.setOutlineColor(sf::Color::White);
	mapOutline.setOutlineThickness(6);


	if(font.loadFromFile("../OtherAssets/FONT.ttf"))					//Loads font from file. Gives error in console if
	{
		std::cout << "\n\nLoaded FONT.ttf";
	}

	if(squareParticleTexture.loadFromFile("../Textures/squareParticle.png"))					//Loads font from file. Gives error in console if
	{
		std::cout << "\nLoaded squareParticle.png";
	}

	if(circleParticleTexture.loadFromFile("../Textures/circleParticle.png"))					//Loads font from file. Gives error in console if
	{
		std::cout << "\nLoaded circleParticle.png";
	}

	if(bowParticleTexture.loadFromFile("../Textures/bowParticle.png"))					//Loads font from file. Gives error in console if
	{
		std::cout << "\nLoaded bowParticle.png";
	}

	if(smokeParticleTexture.loadFromFile("../Textures/smokeParticle.png"))					//Loads font from file. Gives error in console if
	{
		std::cout << "\nLoaded smokeParticle.png";
	}

	if(shieldTexture.loadFromFile("../Textures/shield.png"))					//Loads font from file. Gives error in console if
	{
		std::cout << "\nLoaded shield.png";
	}

	if(mineTexture.loadFromFile("../Textures/mine.png"))					//Loads font from file. Gives error in console if
	{
		std::cout << "\nLoaded mine.png";
	}

	cam.zoom(0.6f);													//Zomed out.
	//cam.zoom(0.2f);													//Zoomed in close.

	window.setView(cam);			


		
/*
	for (int i = 0; i < maxPlayers; i++)
	{																//Spawns players up to max.
		players[playerCount] = new Player(playerCount);
		playerCount++;
	}
*/	

	players[playerCount] = new Player(playerCount);
	playerCount++;
	{
	ParticleSettings backgroundSettings;
		backgroundSettings.particleTexture = squareParticleTexture;
		backgroundSettings.launchSpeed = 20;
		backgroundSettings.emitterCooldown = 0.01f;
		backgroundSettings.maxParticles = 300;
		backgroundSettings.emitterLifeTime = 0.0f;
		backgroundSettings.emissionArea = sf::Vector2f(mapWidth, mapHeight);

		backgroundSettings.startColor = sf::Color(40,60,200,100);
		backgroundSettings.endColor = sf::Color(255,135,255,245);
		backgroundSettings.startScale = 0.7f;
		backgroundSettings.endScale = 0.0f;
		backgroundSettings.particleLifeTime = 6.0f;
		backgroundSettings.airResistance = 1;
		backgroundSettings.maxRotationSpeed =  0.000001f;	
	backgroundParticlesPtr = new Emitter(sf::Vector2f(mapWidth/2, mapHeight/2), backgroundSettings);
	}
	{
	ParticleSettings backgroundSettings;
		backgroundSettings.particleTexture = smokeParticleTexture;
		backgroundSettings.launchSpeed = 35;
		backgroundSettings.emitterCooldown = 0.015f;
		backgroundSettings.maxParticles = 400;
		backgroundSettings.emitterLifeTime = 0.0f;
		backgroundSettings.emissionArea = sf::Vector2f(mapWidth, mapHeight);

		backgroundSettings.startColor = sf::Color(40,60,200,80);
		backgroundSettings.endColor = sf::Color(255,135,255,0);
		backgroundSettings.startScale = 0.0f;
		backgroundSettings.endScale = 30.0f;
		backgroundSettings.particleLifeTime = 7.0f;
		backgroundSettings.airResistance = 1;
		backgroundSettings.maxRotationSpeed =  0.01f;	
	backgroundSmokePtr = new Emitter(sf::Vector2f(mapWidth/2, mapHeight/2), backgroundSettings);
	}


	//update()
	while(window.isOpen())
	{
		dt = deltaTime.restart().asSeconds();						//Counts delta-time for consistant movement independent of framerate.
		
		if (buttonTimeout > 0)
		{
			buttonTimeout -= dt;
		}
		
		window.clear();													//Clears the canvas.

		backgroundSmokePtr-> update(sf::Vector2f(mapWidth/2, mapHeight/2), sf::Vector2f(0, 0));
		backgroundParticlesPtr-> update(sf::Vector2f(mapWidth/2, mapHeight/2), sf::Vector2f(0, 0));			


		backgroundSmokePtr-> draw();
		backgroundParticlesPtr-> draw();


		cam.setCenter(players[0]-> getPos());
		window.setView(cam);
		StateMachine::update();

		window.draw(mapOutline);
		window.display();												//Sends the buffer to the display.
		
	} 

	delete backgroundParticlesPtr;
	delete backgroundSmokePtr;


	std::cout << "\n\n";				
	return 0;
}

