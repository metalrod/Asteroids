#include "stdafx.h"
#include "Game.h"
#include "GameObjects.h"
#include <math.h>

GameObject::GameObject(std::string texturePath, const sf::Vector2f & pos)
	: m_pos(pos)
	, m_destroyed(false)
	, m_collisionRadius(0.0f)
{
	// Load the texture
	// Assign the sprite
	if (texturePath != "")
	{
		m_texture.loadFromFile(texturePath);
		m_sprite.setTexture(m_texture);
		m_sprite.setOrigin(m_sprite.getTextureRect().width * 0.5f, m_sprite.getTextureRect().height * 0.5f);
	}
}
// NEW CODE HERE
void GameObject::reload(std::string texturePath){
    if (texturePath != "")
    {
        m_texture.loadFromFile(texturePath);
        m_sprite.setTexture(m_texture);
        m_sprite.setOrigin(m_sprite.getTextureRect().width * 0.5f, m_sprite.getTextureRect().height * 0.5f);
    }
}
// NEW CODE HERE

void GameObject::Destroy()
{
	m_destroyed = true;
}

bool GameObject::IsDestroyed()
{
	return m_destroyed;
}

void GameObject::Draw(sf::RenderWindow * window)
{
	window->draw(m_sprite);
}

void GameObject::Update(sf::RenderWindow * window, float dt)
{
	float decelScalar = 0.5f;
	m_vel += m_accel * dt;
	
	LimitVelocity(dt);
	ApplyDrag(dt);

	m_pos += m_vel * dt;
	if (m_pos.x < 0)
	{
		m_pos.x = window->getSize().x;
		WentOffEdgeOfScreen(false);
	}
	else if (m_pos.x > window->getSize().x)
	{
		m_pos.x = 0;
		WentOffEdgeOfScreen(false);
	}

	if (m_pos.y < 0)
	{
		m_pos.y = window->getSize().y;
		WentOffEdgeOfScreen(true);
	}
	else if (m_pos.y > window->getSize().y)
	{
		m_pos.y = 0;
		WentOffEdgeOfScreen(true);
	}
	m_sprite.setRotation(m_angle);
	m_sprite.setPosition(m_pos);
}

void GameObject::LimitVelocity(float dt)
{
	float speed = sqrt(m_vel.x * m_vel.x + m_vel.y  *m_vel.y);	

	if (speed <= 0.1)
	{
		return;
	}

	sf::Vector2f normalizedVel = sf::Vector2f(m_vel.x / speed, m_vel.y / speed);

	if (speed > 500)
	{
		speed = 500;
	}

	m_vel.x = normalizedVel.x * speed;
	m_vel.y = normalizedVel.y * speed;
}

void GameObject::SetAngle(float angle)
{
	m_angle = angle;
}

float GameObject::GetAngle()
{
	return m_angle;
}

void GameObject::SetPos(const sf::Vector2f & pos)
{
	m_pos = pos;
}

void GameObject::SetAccel(float amount)
{	
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle;
		m_accel = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_accel = sf::Vector2f(0, 0);
	}
    
}

void GameObject::SetVelocity(float amount)
{
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle;
		m_vel = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_vel = sf::Vector2f(0, 0);
	}
}
//NEW CODE HERE
sf::Vector2f GameObject::GetVelocity() {
    return m_vel;
}
//NEW CODE HERE

void GameObject::SetOwner(Game * owner)
{
	m_owner = owner;
}

void GameObject::ApplyDrag(float dt)
{
	if (m_accel.x == 0 && m_accel.y == 0)
	{
		float dragAmount = dt * 0.9f;
		m_vel.x -= dragAmount* m_vel.x;
		m_vel.y -= dragAmount * m_vel.y;
	}
}

bool GameObject::IsCollidingWith(GameObject* other)
{
	if ((this != other && (!IsDestroyed())) || (other->IsDestroyed()))
	{
		sf::Vector2f vectorFromMeToOther;
		vectorFromMeToOther = other->GetPos() - this->GetPos();
		float distance = sqrt(vectorFromMeToOther.x*vectorFromMeToOther.x + vectorFromMeToOther.y * vectorFromMeToOther.y);

		if (distance < m_collisionRadius + other->m_collisionRadius)
		{
			return true;
		}
	}

	return false;
}

