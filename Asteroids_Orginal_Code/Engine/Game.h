#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObjects.h"

class Game
{
public:
	Game();
	void Draw(sf::RenderWindow* window);
	void Update(sf::RenderWindow * window, float dt);
	void AddObject(GameObject* object);
	void RespawnPlayer();
	void GameOver() { m_gameOver = true; }
	bool isGameOver() { return m_gameOver; }
	void updateScore(int s);
	int getScore();
	void resetScoreTracker();
	int getLevel();
	bool getProceed();
	void updateProceed();
	void getPlayerLocation();
	void setAIRespawn();
private:
	std::vector<GameObject*> m_gameObjects;
	int m_livesRemaining;
	float m_timeUntilRespawn;
	bool m_gameOver;
	sf::Music* gameMusic;
	int m_scoreTracker;
	int m_levelTracker;
	bool m_proceedLevel;
	bool m_hotFix;
};

class Particle
{
public:
	Particle() : m_pos(0.0f, 0.0f), m_vel(0.0f, 0.0f), m_accel(0.0f, 0.0f) {}
	sf::Vector2f m_pos, m_vel, m_accel;
	sf::Color m_color;
	float m_size, m_lifetime, m_initialLifeTime;
};

class ParticleSystem : public GameObject
{
public:
	ParticleSystem(float count, sf::Vector2f pos, sf::Color color, float size, int spread, float angle, float speed, int speedSpread);
	void Draw(sf::RenderWindow* window);
	void Update(sf::RenderWindow* window, float dt);
	std::vector<Particle> m_particles;
	sf::CircleShape m_rectShape;
};