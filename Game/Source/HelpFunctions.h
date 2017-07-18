#pragma once

#include <SFML/Graphics.hpp>		//for Graphics stuff


int randomNumber(int min, int max);
void randomPosition(sf::Vector2f & pos);
sf::Vector2f clampVector(sf::Vector2f v, float clamp);
float getAngle(sf::Vector2f v1, sf::Vector2f v2);										//Retuns the angle between two points.
float toRadians(float degrees);
float toDegrees(float radians);
bool checkCollision(sf::Vector2f v1, sf::Vector2f v2, float r1, float r2);