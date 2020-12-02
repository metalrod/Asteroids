#include "stdafx.h"
#include "Game.h"
#include "GameObjects.h"
#include <math.h>

Player::Player(std::string texturePath, const sf::Vector2f & pos)
	: GameObject(texturePath, pos)
	, m_cooldown(0.0f)
{
	shootingBuffer = new sf::SoundBuffer();
	if (!shootingBuffer->loadFromFile("Audio/Shooting.wav"))
	{
		//
	}
	shootingSound = new sf::Sound();
	shootingSound->setBuffer(*shootingBuffer);
    shootingSound->setVolume(10.f);

	dyingBuffer = new sf::SoundBuffer();
	if (!dyingBuffer->loadFromFile("Audio/Dying.wav"))
	{
		//
	}
	dyingSound = new sf::Sound();
	dyingSound->setBuffer(*dyingBuffer);
    dyingSound->setVolume(10.f);

	respawningBuffer = new sf::SoundBuffer();
	if (!respawningBuffer->loadFromFile("Audio/Respawn.wav"))
	{
		//
	}
	respawningSound = new sf::Sound();
	respawningSound->setBuffer(*respawningBuffer);
	respawningSound->play();
    respawningSound->setVolume(10.f);
}

void Player::invulSound(){
    respawningSound->play();
}

void Player::DeadSound(){
    dyingSound->play();
}

void Player::shootSound(){
    shootingSound->play();
}

bool Player::isFiring()
{
	return m_firing;
}
bool Player::isInvulnerable()
{
	return m_isInvulnerable;
}

void Player::Draw(sf::RenderWindow* window)
{
	GameObject::Draw(window);

	if (m_timer < 3.0f)
	{
		sf::CircleShape shape;
		shape.setRadius(60);
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineThickness(5 * m_timer / 3.0f + 1);
		sf::Color color = sf::Color::Blue;
		color.a = 255 * m_timer / 3.0f;
		shape.setOutlineColor(color);
		shape.setOrigin(60, 55);
		shape.setPosition(m_pos.x, m_pos.y);
		window->draw(shape);
	}

	if (m_timer < 3.0f)
	{
		float sinValue = sin(m_timer * 25); // Between -1 and 1
		sinValue += 1.25; // Between 0-2
		sinValue *= 0.25; // Between 0-1	
		float alpha = sinValue * 255; // 0 -255
		sf::Color shipColor = sf::Color::White;
		shipColor.a = alpha;
		m_sprite.setColor(shipColor);
	}
	else
	{
		m_sprite.setColor(sf::Color::White);
	}
}

void Player::Update(sf::RenderWindow * window, float dt)
{
	m_cooldown -= dt;
	m_timer += dt;

	GameObject::Update(window, dt);

	//cout << m_timer << endl;

	if (m_timer >= 3.0f)
	{
		m_isInvulnerable = false;
	}

	SetAccel(0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		SetAngle(GetAngle() + 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		SetAngle(GetAngle() - 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		SetAccel(400.0f);
		m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle + 90)) * 50, -cos(DEG_TO_RAD * (m_angle + 90)) * 50), sf::Color::Blue, 1, 0, -m_angle, 400, 0));
		m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle - 90)) * 50, -cos(DEG_TO_RAD * (m_angle - 90)) * 50), sf::Color::Blue, 1, 0, -m_angle, 400, 0));
	}
//NEW CODE HERE
    //DAMIT MAN AUCH IN RESPAWN SCHIEßEN kann 0 anstatt 3
	if (m_timer >= 0.0f)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			m_firing = true;
		}
		else
		{
			m_firing = false;
		}
	}

	if (m_firing && m_cooldown <= 0.0f)
	{
		// Wants to do a multishot here
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			m_cooldown = 1.0f;
			shootingSound->play();
			for (int i = 0; i < 3; i++)
			{
				Bullet* bullet = new Bullet(m_pos);
				bullet->SetAngle(m_angle + i * 15 - 15);
				bullet->SetVelocity(500);
				m_owner->AddObject(bullet);
			}
		}
		else
		{
			shootingSound->play();
			m_cooldown = 0.2f;
			Bullet* bullet = new Bullet(m_pos);
			bullet->SetAngle(m_angle);
			bullet->SetVelocity(500);
			m_owner->AddObject(bullet);
		}
	}
}

void Player::CollidedWith(GameObject* other)
{
	if (m_isInvulnerable == false)
	{
		EnemyBullet* enemybullet = dynamic_cast<EnemyBullet*>(other);
		if (enemybullet != nullptr)
		{
			Destroy();
			dyingSound->play();
			m_owner->RespawnPlayer();
			m_isInvulnerable = true;
			//m_owner->GameOver();
		}

		Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
		if (asteroid != nullptr)
		{
			Destroy();
			dyingSound->play();
			m_owner->RespawnPlayer();
			m_isInvulnerable = true;
			//m_owner->GameOver();
		}
	}
}

sf::Vector2f Player::currentPos()
{
	return m_sprite.getPosition();
}
float Player::getPRotation()
{
	return m_sprite.getRotation();
}

void Player::setInvulernable()
{
	m_isInvulnerable = true;
	m_timer = 0.0f;
}
