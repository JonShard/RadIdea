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
sf::RectangleShape mapOutline;

sf::Font font;									//The font imported from file used to print text on Screen.

sf::Texture squareParticleTexture;
sf::Texture circleParticleTexture;
sf::Texture bowParticleTexture;
sf::Texture shieldTexture;

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

	mapOutline.setSize(sf::Vector2f(mapWidth, mapHeight));
	mapOutline.setFillColor(sf::Color::Black);
	mapOutline.setOutlineThickness(5);

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

	if(shieldTexture.loadFromFile("../Textures/shield.png"))					//Loads font from file. Gives error in console if
	{
		std::cout << "\nLoaded shield.png";
	}

	cam.zoom(1.01f);													//Zomed out.
	//cam.zoom(0.2f);													//Zoomed in close.

	window.setView(cam);			


		
/*
	for (int i = 0; i < maxPlayers; i++)
	{																//Spawns players up to max.
		players[playerCount] = new Player(playerCount);
		playerCount++;
	}
*/	



	//update()
	while(window.isOpen())
	{
		dt = deltaTime.restart().asSeconds();						//Counts delta-time for consistant movement independent of framerate.
		
		if (buttonTimeout > 0)
		{
			buttonTimeout -= dt;
		}
		
		window.clear();													//Clears the canvas.
		window.draw(mapOutline);


		StateMachine::update();

		window.display();												//Sends the buffer to the display.
		
	} 

	std::cout << "\n\n";				
	return 0;
}

