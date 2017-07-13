#include <SFML/Graphics.hpp>		//for Graphics stuff
#include <iostream>			//Used for printing to screen(debug) and some input.
#include "Player.h"			//The controllable player.
#include "ParticleSystem.h"
#include "HelpFunctions.h"

#include "Constants.h"


sf::View cam(sf::Vector2f(0,0), sf::Vector2f(mapWidth, mapHeight));
sf::ContextSettings settings;					//The settings Context, all the config info the window needs to run.
sf::RenderWindow window;						//The window that draws the graphics on the screen.
sf::Font font;									//The font imported from file used to print text on Screen.
sf::Clock deltaTime;
sf::RectangleShape mapOutline;

sf::Texture shipExhaust;
sf::Texture shieldTexture;


float dt = 0;
int playerCount = 0;
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

	if(shipExhaust.loadFromFile("Textures/shipExhaust.png"))					//Loads font from file. Gives error in console if
	{
		std::cout << "\n\nLoaded shipExhaust.png";
	}

	if(shieldTexture.loadFromFile("Textures/shield.png"))					//Loads font from file. Gives error in console if
	{
		std::cout << "\n\nLoaded shield.png";
	}

	cam.zoom(1.01f);													//Zomed out.
	//cam.zoom(0.2f);													//Zoomed in close.

	window.setView(cam);			


																		//Spawns 4 players.
	players[playerCount] = new Player(playerCount);
	playerCount++;

	players[playerCount] = new Player(playerCount);
	playerCount++;

	players[playerCount] = new Player(playerCount);
	playerCount++;

	players[playerCount] = new Player(playerCount);
	playerCount++;
	


	//update()
	while(window.isOpen())
	{
		dt = deltaTime.restart().asSeconds();						//Counts delta-time for consistant movement independent of framerate.

		

		sf::Event event;
		while(window.pollEvent(event))									
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))				// ESC to quit.
			{
				window.close();
			}
			if(event.type == sf::Event::Closed)								//If the event happening is closed:
			{															//then close the window as well.
				window.close();
			}

		}


		//////////////		Screen drawing		///////////////
		window.clear();													//Clears the canvas.
		window.draw(mapOutline);

		for (int i = 0; i < playerCount; i++)							//Update all the players.
		{
			players[i]->update();
		}


		for (int i = 0; i < playerCount; i++)							//Draw the players.
		{
			players[i]->draw();
		}


		window.display();												//Sends the buffer to the display.
		//////////////		Screen drawn		///////////////
	} 

	std::cout << "\n\n";				
	return 0;
}

