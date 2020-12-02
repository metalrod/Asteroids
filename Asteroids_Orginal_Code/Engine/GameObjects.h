#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#define DEG_TO_RAD (0.0174532925f)

class Game;
class GameState;

class GameObject
{
public:
	GameObject(std::string texturePath, const sf::Vector2f& pos);
	virtual void Draw(sf::RenderWindow* window);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void LimitVelocity(float dt);
	virtual void ApplyDrag(float dt);
	virtual void WentOffEdgeOfScreen(bool offTopOfScreen) {};
	virtual void CollidedWith(GameObject* other) {};

	bool IsCollidingWith(GameObject* other);
	void SetAngle(float angle);
	float GetAngle();
	
	void SetPos(const sf::Vector2f& pos);
	sf::Vector2f GetPos() { return m_pos; }
	void SetAccel(float amount);
	void SetVelocity(float amount);
	void SetOwner(Game* owner);
	virtual void Destroy();
	bool IsDestroyed();

protected:
	sf::Vector2f m_pos;
	sf::Vector2f m_vel;
	sf::Vector2f m_accel;

	sf::Sprite m_sprite;
	sf::Texture m_texture;

	Game* m_owner;
	float m_angle;
	float m_collisionRadius;
	bool m_destroyed;
};

class Bullet : public GameObject
{
public:
	Bullet(const sf::Vector2f& pos);	
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void ApplyDrag(float dt) {};
	virtual void CollidedWith(GameObject* other);
	float m_timeAlive;
};

class Asteroid : public GameObject
{
public:
	Asteroid(std::string texturePath, const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void ApplyDrag(float dt) 
	{
		// DO nothing
	}
	sf::SoundBuffer* blowUpBuffer;
	sf::Sound* blowUpSound;
private:
	bool rotation;

};

class SmallAsteroid : public Asteroid
{
public:
	SmallAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
};

class MedAsteroid : public Asteroid
{
public:
	MedAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
};

class LargeAsteroid : public Asteroid
{
public:
	LargeAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
};


class Player : public GameObject
{
public:
	Player(std::string texturePath, const sf::Vector2f& pos);
	virtual void Draw(sf::RenderWindow* window);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void CollidedWith(GameObject* other);
	sf::Vector2f currentPos();
	float getPRotation();
	bool isFiring();
	bool isInvulnerable();
	void setInvulernable();
private:
	bool m_firing;
	float m_cooldown;
	float m_timer = 0.0f;
	bool m_isInvulnerable = true;
	sf::SoundBuffer* shootingBuffer;
	sf::Sound* shootingSound;
	sf::SoundBuffer* dyingBuffer;
	sf::Sound* dyingSound;
	sf::SoundBuffer* respawningBuffer;
	sf::Sound* respawningSound;
};

class Enemy : public GameObject
{
public:
	Enemy(std::string texturePath, const sf::Vector2f& pos);
	virtual void Draw(sf::RenderWindow* window);
	virtual void Update(sf::RenderWindow* window, float dt);
	virtual void CollidedWith(GameObject* other);
	void trackPlayer(sf::Vector2f pos, float rotation);
	float toDegree(float radian);
	float length(sf::Vector2f vector);
	sf::Vector2f unitVector(sf::Vector2f vector);
	sf::Vector2f direction(sf::Sprite& follower, sf::Vector2f target);
	void firingPlayer(bool isFiring);
	void spawningPlayer(bool isSpawning);

	void Brain(float dt);
	void Wait(float dt);
	void Track(float dt);
	void Attack(float dt);
	void Wander(float dt);

private:
	float m_timer = 0.0f;
	bool m_firing;
	float m_cooldown;
	sf::Vector2f playerLocation;
	sf::Vector2f target;
	sf::Vector2f velocity;
	sf::Vector2f wayPoint;
	float angle;
	float playerRotation;
	bool isPlayerFiring;
	bool isPlayerSpawning;
	bool waypointAquired;
	bool waypointAchieved;
};

class EnemyBullet : public GameObject
{
public:
	EnemyBullet(const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void ApplyDrag(float dt) {};
	virtual void CollidedWith(GameObject* other);
	float m_timeAlive;
};