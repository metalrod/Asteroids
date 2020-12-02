#include "stdafx.h"
#include "Game.h"
#include "GameObjects.h"

Bullet::Bullet(const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/laserBlue08.png", pos)
	, m_timeAlive(0.0f)
{
	m_sprite.setScale(0.5, 0.5);
	m_collisionRadius = 20.0f;
}

void Bullet::Update(sf::RenderWindow * window, float dt)
{
	if (m_timeAlive > 3.0f)
	{
		Destroy();
	}

	GameObject::Update(window, dt);
	m_timeAlive += dt;
}

void Bullet::CollidedWith(GameObject* other)
{
	Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
	if (asteroid != nullptr)
	{
		if (dynamic_cast<LargeAsteroid*>(other) != NULL)
		{
			m_owner->updateScore(100);
		}
		else if (dynamic_cast<MedAsteroid*>(other) != NULL)
		{
			m_owner->updateScore(50);
		}
		else if (dynamic_cast<SmallAsteroid*>(other) != NULL)
		{
			m_owner->updateScore(25);
		}

		Destroy();
		other->Destroy();
	}
}

EnemyBullet::EnemyBullet(const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/laserRed10.png", pos)
	, m_timeAlive(0.0f)
{
	m_sprite.setScale(0.5, 0.5);
	m_collisionRadius = 20.0f;
}



void EnemyBullet::Update(sf::RenderWindow * window, float dt)
{
	if (m_timeAlive > 3.0f)
	{
		Destroy();
	}

	GameObject::Update(window, dt);
	m_timeAlive += dt;
}

void EnemyBullet::CollidedWith(GameObject* other)
{
	Player* player = dynamic_cast<Player*>(other);
	if (player != nullptr)
	{
		Destroy();
		if (dynamic_cast<Player*>(other)->isInvulnerable() == false)
		{
			other->Destroy();
		}
	}
}