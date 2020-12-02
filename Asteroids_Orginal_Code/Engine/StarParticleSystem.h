#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <ctime>

using namespace std;

class Star
{
public:
	Star(sf::Uint16 xPosition, sf::Uint16 yPosition);
	sf::Uint16 getPositionX();
	sf::Uint16 getPositionY();
	void setPositionX(sf::Uint16 xPosition);
	void setPositionY(sf::Uint16 yPosition);
	void addPositionY(sf::Uint16 yPosition);
private:
	sf::Uint16 xPos;
	sf::Uint16 yPos;
};

class Starfield
{
public:
	Starfield(sf::RenderWindow* window);
	void Update(sf::RenderWindow* window, float dt);
	void Draw(sf::Texture& p_Texture);

protected:
	int maxSmallStars;
	int maxMediumStars;
	int maxLargeStars;

	sf::Uint16 x_Size;
	sf::Uint16 y_Size;

	sf::Uint16 smallSize;
	sf::Uint16 mediumSize;
	sf::Uint16 largeSize;

	vector<Star> smallStars;
	vector<Star> mediumStars;
	vector<Star> largeStars;

	sf::Image smallStarImage;
	sf::Image mediumStarImage;
	sf::Image largeStarImage;

	mt19937 starGenerator;
	mt19937 starGeneratorY;
	uniform_int_distribution<int> starDistributionX;
	uniform_int_distribution<int> starDistributionY;
};