#include "stdafx.h"
#include "Game.h"
#include "GameObjects.h"
#include <random>
#include <ctime>
#include <cassert>
#include <iostream>

using namespace std;

mt19937 enemyGenerator(time(0));
uniform_int_distribution<int> randomWayPointX(0, 1280);
uniform_int_distribution<int> randomWayPointY(0, 720);

Enemy::Enemy(std::string texturePath, const sf::Vector2f & pos)
	: GameObject(texturePath, pos), playerRotation(0.0f), target(0, 0), velocity(0, 0), angle(0.0f)
	, isPlayerFiring(false), isPlayerSpawning(false), waypointAquired(false), waypointAchieved(false)
    , wayPoint(0, 0), m_firing(false), m_cooldown(0.0f), m_gotchha(false)
{

}

void Enemy::Draw(sf::RenderWindow* window)
{
	GameObject::Draw(window);

	if (m_timer < 3.0f)
	{
		sf::CircleShape shape;
		shape.setRadius(60);
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineThickness(5 * m_timer / 3.0f + 1);
		sf::Color color = sf::Color::Red;
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

void Enemy::Update(sf::RenderWindow * window, float dt)
{
	m_timer += dt;
	m_cooldown += dt;

	GameObject::Update(window, dt);
	Brain(dt);

}

void Enemy::CollidedWith(GameObject* other)
{
	Bullet* bullet = dynamic_cast<Bullet*>(other);
	if (bullet != nullptr)
	{
        //NEW CODE HERE
        //Bisschen Kohle für den Abschuss eines Gegners
        m_owner->updateScore(250);
        //NEW CODE HERE
		Destroy();
		//dyingSound->play();
		//m_owner->RespawnPlayer();
		//m_owner->GameOver();
	}
}

void Enemy::trackPlayer(sf::Vector2f pos, float rotation)
{
	playerLocation = pos;
	playerRotation = rotation;
}

float Enemy::toDegree(float radian)
{
	return 180.f / 3.141592 * radian;
}

float Enemy::length(sf::Vector2f vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f Enemy::unitVector(sf::Vector2f vector)
{
	assert(vector != sf::Vector2f(0.0f, 0.0f));
	return vector / length(vector);
}
sf::Vector2f Enemy::direction(sf::Sprite& follower, sf::Vector2f target)
{
	return unitVector(sf::Vector2f(target - follower.getPosition()));
}

void Enemy::firingPlayer(bool isFiring)
{
	isPlayerFiring = isFiring;
}

void Enemy::spawningPlayer(bool isSpawning)
{
	isPlayerSpawning = isSpawning;
}

void Enemy::Brain(float dt)
{
	if (isPlayerSpawning)
	{
		Wait(dt);
        m_gotchha = false;
	}
	if (!isPlayerSpawning)
	{
		Track(dt);
	}
}

void Enemy::Wait(float dt)
{
	if (!waypointAquired)
	{
		wayPoint = sf::Vector2f(randomWayPointX(enemyGenerator), randomWayPointY(enemyGenerator));
		//cout << wayPoint.x << " " << wayPoint.y << endl;
		waypointAquired = true;
	}

	else if (waypointAquired)
	{
		Wander(dt);
	}
}

void Enemy::Track(float dt)
{
	target = direction(m_sprite, playerLocation);
	m_angle = toDegree(atan2(target.x, -target.y));

	//cout << "Chasing" << endl;
	//cout << m_sprite.getRotation() << endl;
	//cout << m_sprite.getPosition().x << m_sprite.getPosition().y << endl;
    
    float dist_player = length(playerLocation - m_sprite.getPosition());
    
    if(m_gotchha){
        SetVelocity(0);
        if (m_angle < m_angle_set-10 ){
            SetAngle(m_angle-70);
            SetVelocity(150);
            fly();
        } else if (m_angle > m_angle_set+10) {
            SetAngle(m_angle+70);
            SetVelocity(150);
            fly();
        } else {
            SetVelocity(0);
            m_gotchha = false;
        }
    }else {
        if (dist_player <= 175 && dist_player > 150) {
            m_gotchha = true;
            //m_tracktimer = dt;
        } else if (dist_player <= 150){
            SetAngle(m_angle-180);
            SetAccel(25.f);
            fly();
        } else {
            if (!isPlayerSpawning) {
                SetAccel(dist_player-100);
                fly();
            } else {
                SetAccel(25.f);
                fly();
            }
        }
        
    }
    
	if (m_cooldown >= 1.5f)
	{
		m_firing = isPlayerFiring;
	}

	if (m_firing)
	{
		Attack(dt);
		m_cooldown = 0.0f;
	}
}

void Enemy::Attack(float dt)
{
	//shootingSound->play();
	m_cooldown = 0.2f;
	EnemyBullet* bullet = new EnemyBullet(m_pos);
	bullet->SetAngle(m_angle);
	bullet->SetVelocity(500);
	m_owner->AddObject(bullet);

	m_firing = false;
}

void Enemy::Wander(float dt)
{
	if (!waypointAchieved)
	{
		target = direction(m_sprite, wayPoint);
		m_angle = toDegree(atan2(target.x, -target.y));

		float dist = length(wayPoint - m_sprite.getPosition());
		if (dist < 50)
		{
			//cout << "Gotcha!" << endl;
			waypointAchieved = true;
		}
		else
		{
			//cout << "Chasing" << endl;
			//cout << m_sprite.getRotation() << endl;
			//cout << m_sprite.getPosition().x << m_sprite.getPosition().y << endl;
			SetAngle(m_angle);
			SetAccel(25.0f);
			m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle + 90)) * 50, -cos(DEG_TO_RAD * (m_angle + 90)) * 50), sf::Color::Red, 1, 0, -m_angle, 400, 0));
			m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle - 90)) * 50, -cos(DEG_TO_RAD * (m_angle - 90)) * 50), sf::Color::Red, 1, 0, -m_angle, 400, 0));
		}
	}
	else if (waypointAchieved)
	{
		waypointAquired = false;
		waypointAchieved = false;
	}

}

void Enemy::setAngleSet(float set){
    m_angle_set = set;
}
void Enemy::fly(){
    m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle + 90)) * 50, -cos(DEG_TO_RAD * (m_angle + 90)) * 50), sf::Color::Green, 1, 0, -m_angle, 400, 0));
    m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle - 90)) * 50, -cos(DEG_TO_RAD * (m_angle - 90)) * 50), sf::Color::Green, 1, 0, -m_angle, 400, 0));
}
