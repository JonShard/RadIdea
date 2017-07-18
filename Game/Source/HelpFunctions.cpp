#include "HelpFunctions.h"
#include "Constants.h"
#include <cstdlib>			//rand().
#include <cmath>

int randomNumber(int min, int max)												//Random int in a range.
{
	return (rand() + min) % max;
}


void randomPosition(sf::Vector2f & pos)
{
	pos.x = static_cast<float>(rand() % windowWidth);
	pos.y = static_cast<float>(rand() % windowHeight);
}

sf::Vector2f clampVector(sf::Vector2f v, float clamp)
{
	sf::Vector2f n;
	float magnitude = std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2)) + 0.0000001f;
	n.x = (v.x/magnitude) * clamp;
	n.y = (v.y/magnitude) * clamp;
	return n;
}



float getAngle(sf::Vector2f v1, sf::Vector2f v2)										//Retuns the angle between two points.
{
   	float angle = 0;
	
   	float deltaX = v1.x - v2.x;
   	float deltaY = v2.y - v1.y;		//revesed Y
   	
   	angle = std::atan2(deltaX, deltaY);
   	angle = toDegrees(angle);
   	
    return angle + 180;
}



float magnitude(sf::Vector2f v1, sf::Vector2f v2)										//Will return the distance between to points(x,y)
{
	return std::sqrt( std::pow(v1.x - v2.x,  2)
			   + std::pow(v1.y - v2.y,  2));	
}


float toRadians(float degrees)
{
	return degrees * (3.14f / 180);
}


float toDegrees(float radians)
{
	return radians * (180 / 3.14f);
}


bool checkCollision(sf::Vector2f v1, sf::Vector2f v2, float r1, float r2)
{
	bool collision = false;
	float thicc = r1 + r2;

	if (std::abs(v1.x - v2.x) < thicc && std::abs(v1.y - v2.y) < thicc)			//If Positions are 
	{
		if (magnitude(v1, v2) < thicc)
		{
			collision = true;
		}
	}

	return collision;
}