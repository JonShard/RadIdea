#include "StateMachine.h"
#include "Constants.h"

extern State state;
extern sf::RenderWindow window;						//The window that draws the graphics on the screen.
extern float dt;									//Delta-time.
extern float buttonTimeout;
extern Player* players[];
extern int playerCount;


void StateMachine::updateCollisions()			//Between Player-Player, Player-Projectile, Player-Boids, Boids-Projectile.
{

}


void StateMachine::updateAcceleration()			//Due to gravity wells.
{
	
}






void StateMachine::update()
{
	switch (state)
	{
		case inGame:
			updateInGame();
		break;

		case startMenu:
			updateStartMenu();
		break;

		case playerSelect:
			updatePlayerSelect();
		break;
	}
}


void StateMachine::updateInGame()
{

	sf::Event event;
	while(window.pollEvent(event))									
	{
		if(buttonTimeout <= 0)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))				// ESC to quit.
			{
				window.close();
				buttonTimeout = buttonTimeoutTime;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))				// ESC to quit.
			{
				if (playerCount < maxPlayers)
				{
					players[playerCount] = new Player(playerCount);
					playerCount++;
					buttonTimeout = buttonTimeoutTime;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))				// ESC to quit.
			{
				if (playerCount > 0)
				{
					players[playerCount-1] ->killPlayer();
					buttonTimeout = buttonTimeoutTime;
				}
			}
		}
		if (event.type == sf::Event::Closed)								//If the event happening is closed:
		{															//then close the window as well.
			window.close();
		}

	}


	for (int i = 0; i < playerCount; i++)							//Update all the players.
	{
		players[i]->update();
	}

	for (int i = 0; i < playerCount; i++)							//Draw the players.
	{
		players[i]->draw();
	}

}


void StateMachine::updateStartMenu()
{

}


void StateMachine::updatePlayerSelect()
{

}
