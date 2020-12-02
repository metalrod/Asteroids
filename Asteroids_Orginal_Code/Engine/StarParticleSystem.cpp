#include "StarParticleSystem.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>

Star::Star(sf::Uint16 xPosition, sf::Uint16 yPosition)
{
	xPos = xPosition;
	yPos = yPosition;
}

sf::Uint16 Star::getPositionX()
{
	return xPos;
}

sf::Uint16 Star::getPositionY()
{
	return yPos;
}

void Star::setPositionX(sf::Uint16 xPosition)
{
	xPos = xPosition;
}

void Star::setPositionY(sf::Uint16 yPosition)
{
	yPos = yPosition;
}

void Star::addPositionY(sf::Uint16 yPosition)
{
	yPos += yPosition;
}

Starfield::Starfield(sf::RenderWindow* window)
	: maxSmallStars(0), maxMediumStars(0), maxLargeStars(0), x_Size(window->getSize().x), y_Size(window->getSize().y)
	, smallSize(1), mediumSize(2), largeSize(4)
{
	smallStarImage.create(smallSize, smallSize, sf::Color::White);
	mediumStarImage.create(mediumSize, mediumSize, sf::Color::White);
	largeStarImage.create(largeSize, largeSize, sf::Color::White);

	starGenerator.seed(time(0));
	starGeneratorY.seed(time(0)+24);
	starDistributionX = uniform_int_distribution<int>(0, x_Size);
	starDistributionY = uniform_int_distribution<int>(0, y_Size);

	maxSmallStars = (x_Size / 80) * (y_Size / 8);
	maxMediumStars = (x_Size / 240) * (y_Size / 24);
	maxLargeStars = (x_Size / 720) * (y_Size / 72);

	while (smallStars.size() <= maxSmallStars)
	{
		smallStars.push_back(Star(starDistributionX(starGenerator), starDistributionY(starGeneratorY)));
	}

	while (mediumStars.size() <= maxMediumStars)
	{
		mediumStars.push_back(Star(starDistributionX(starGenerator), starDistributionY(starGeneratorY)));
	}

	while (largeStars.size() <= maxLargeStars)
	{
		largeStars.push_back(Star(starDistributionX(starGenerator), starDistributionY(starGeneratorY)));
	}
}

void Starfield::Update(sf::RenderWindow* window, float dt)
{
	for_each(smallStars.begin(), smallStars.end(), [&](Star& p_Star) {
		p_Star.addPositionY(1);
	});

	for_each(mediumStars.begin(), mediumStars.end(), [&](Star& p_Star) {
		p_Star.addPositionY(2);
	});

	for_each(largeStars.begin(), largeStars.end(), [&](Star& p_Star) {
		p_Star.addPositionY(4);
	});

	smallStars.erase(remove_if(smallStars.begin(), smallStars.end(), [&](Star& p_Star) {
		return (p_Star.getPositionY() > y_Size);
	}), smallStars.end());

	mediumStars.erase(remove_if(mediumStars.begin(), mediumStars.end(), [&](Star& p_Star) {
		return (p_Star.getPositionY() > y_Size);
	}), mediumStars.end());

	largeStars.erase(remove_if(largeStars.begin(), largeStars.end(), [&](Star& p_Star) {
		return (p_Star.getPositionY() > y_Size);
	}), largeStars.end());

	while ((int)smallStars.size() <= maxSmallStars)
	{
		smallStars.push_back(Star(starDistributionX(starGenerator), 0));
	}

	while ((int)mediumStars.size() <= maxMediumStars)
	{
		mediumStars.push_back(Star(starDistributionX(starGenerator), 0));
	}

	while ((int)largeStars.size() <= maxLargeStars)
	{
		largeStars.push_back(Star(starDistributionX(starGenerator), 0));
	}
}

void Starfield::Draw(sf::Texture& p_Texture)
{
	for (vector<Star>::iterator it = smallStars.begin(); it != smallStars.end(); ++it)
	{
		if (it->getPositionY() < 720)
		{
			p_Texture.update(smallStarImage, it->getPositionX(), it->getPositionY());
		}
	}

	for (vector<Star>::iterator it = mediumStars.begin(); it != mediumStars.end(); ++it)
	{
		if (it->getPositionY() < 719)
		{
			p_Texture.update(mediumStarImage, it->getPositionX(), it->getPositionY());
		}
	}

	for (vector<Star>::iterator it = largeStars.begin(); it != largeStars.end(); ++it)
	{
		if (it->getPositionY() < 717)
		{
			p_Texture.update(largeStarImage, it->getPositionX(), it->getPositionY());
		}
	}
}