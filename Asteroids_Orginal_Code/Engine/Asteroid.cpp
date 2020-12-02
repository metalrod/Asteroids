#include "stdafx.h"
#include "Game.h"
#include "GameObjects.h"
#include <random>
#include <ctime>

using namespace std;

mt19937 randomGenerator(time(0));
uniform_int_distribution<int> randomAngle(0, 360);
uniform_int_distribution<int> random(1, 2);
uniform_int_distribution<int> randomRpositive(45, 90);
uniform_int_distribution<int> randomRnegative(-90, -45);

Asteroid::Asteroid(std::string texturePath, const sf::Vector2f& pos)
	: GameObject(texturePath, pos)
{
	int randomA = randomAngle(randomGenerator);
	m_collisionRadius = 50.0f;
	SetAngle(randomA);
	if (random(randomGenerator) < 2)
	{
		rotation = true;
	}
	else
	{
		rotation = false;
	}

	blowUpBuffer = new sf::SoundBuffer();
	if (!blowUpBuffer->loadFromFile("Audio/BlowUp.wav"))
	{
		//
	}
	blowUpSound = new sf::Sound();
	blowUpSound->setBuffer(*blowUpBuffer);
}

MedAsteroid::MedAsteroid(const sf::Vector2f& pos)
	: Asteroid("Sprites/PNG/Meteors/meteorBrown_med1.png", pos)
{
	m_collisionRadius = 30.0f;
}

void MedAsteroid::Destroy()
{
	Asteroid::Destroy();
	blowUpSound->play();
	for (int i = 0; i < 2; i++)
	{
		SmallAsteroid* small = new SmallAsteroid(m_pos);
		small->SetAngle(randomAngle(randomGenerator));
		small->SetVelocity(50);
		m_owner->AddObject(small);
	}
}

LargeAsteroid::LargeAsteroid(const sf::Vector2f& pos)
	: Asteroid("Sprites/PNG/Meteors/meteorBrown_big1.png", pos)
{

}

void LargeAsteroid::Destroy()
{
	Asteroid::Destroy();
	blowUpSound->play();
	for (int i = 0; i < 3; i++)
	{
		MedAsteroid* medium = new MedAsteroid(m_pos);
		medium->SetAngle(randomAngle(randomGenerator));
		medium->SetVelocity(75);
		m_owner->AddObject(medium);
	}
}

SmallAsteroid::SmallAsteroid(const sf::Vector2f& pos)
	: Asteroid("Sprites/PNG/Meteors/meteorBrown_small2.png", pos)
{
	m_collisionRadius = 15;
}

void SmallAsteroid::Destroy()
{
	Asteroid::Destroy();
	blowUpSound->play();
}

void Asteroid::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);
	if (rotation)
	{
		SetAngle(GetAngle() + randomRpositive(randomGenerator) * dt);
	}
	else
	{
		SetAngle(GetAngle() + randomRnegative(randomGenerator) * dt);
	}
}