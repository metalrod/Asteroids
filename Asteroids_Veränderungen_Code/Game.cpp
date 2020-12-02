#include "Game.h"
#include "GameObjects.h"
#include <iostream>
#include <random>
#include <ctime>
#include <sstream>

using namespace std;

mt19937 myGenerator(time(0));
// NEW CODE HERE
uniform_int_distribution<int> randomLocationX(0, 1000);
uniform_int_distribution<int> randomLocationY(0, 600);
// NEW CODE HERE
uniform_int_distribution<int> randomSize(1, 5);
uniform_int_distribution<int> randomLifetime(1, 5);

Game::Game() : m_timeUntilRespawn(2.0f)
	, m_livesRemaining(4)
	, m_scoreTracker(0)
	, m_levelTracker(5)
	, m_proceedLevel(false)
	, m_hotFix(false)
{
	gameMusic = new sf::Music();
	if (!gameMusic->openFromFile("Audio/Song.wav"))
	{
		// Not Working
	}
    //NEW CODE HERE
    //Mir war die Musik beim Arbeiten zu laut - ehemals 50
	gameMusic->setVolume(10);
    //NEW CODE HERE
	gameMusic->play();
	gameMusic->setLoop(true);
}
void Game::Draw(sf::RenderWindow * window)
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		current->Draw(window);
	}
	for (int i = 0; i < m_livesRemaining; i++)
	{
		sf::Texture shipTexture;
		shipTexture.loadFromFile("Sprites/PNG/playerShip2_red.png");

		sf::Sprite shipSprite;
		shipSprite.setTexture(shipTexture);
		shipSprite.setScale(0.5, 0.5);

		sf::Color shipColor;
		shipColor = sf::Color::White;
		shipColor.a = 120;
		shipSprite.setColor(shipColor);

		shipSprite.setPosition(25 + 50 * i, 50);
		window->draw(shipSprite);
	}
}

void Game::Update(sf::RenderWindow * window, float dt)
{
	if (m_timeUntilRespawn > 0)
	{
		m_timeUntilRespawn -= dt;
		if (m_timeUntilRespawn <= 0)
		{
            //NEW CODE HERE
            //Damit der Spieler nicht immer in der Mitte beginnt
			Player* player = new Player("Sprites/PNG/playerShip2_red.png", sf::Vector2f(randomLocationX(myGenerator), randomLocationY(myGenerator)));
            //NEW CODE HERE
			AddObject(player);
		}
	}

	if (m_hotFix == false)
	{
        // NEW CODE HERE
        // Mehere Gegner und zufällige Position
        // Alte Position 300, 150
        //Enemy* enemy = new Enemy("Sprites/PNG/playerShip2_blue.png", sf::Vector2f(300, 150));
        //AddObject(enemy);
		m_hotFix = true;
        float spot = (360.f / m_levelTracker);
        for (float i=0; i<m_levelTracker; i++) {
            std::stringstream ss;
            //ss << "Sprites/PNG/playerShip" << i+1 << "_red.png";
            //Enemy* enemy = new Enemy(ss.str(), sf::Vector2f(randomLocationX(myGenerator), randomLocationY(myGenerator)));
            Enemy* enemy = new Enemy("Sprites/PNG/playerShip2_blue.png", sf::Vector2f(randomLocationX(myGenerator), randomLocationY(myGenerator)));
            if ((-180 + spot + (spot*i)) == 180 ) {
                enemy->setAngleSet(-179);
            } else {
                enemy->setAngleSet(-180 + spot + (spot*i));
                
            }
            
            AddObject(enemy);
        }
        // NEW CODE HERE
       // PowerUp droppen
        Bonus* bonus = new Bonus("", sf::Vector2f(randomLocationX(myGenerator), randomLocationY(myGenerator)));
        AddObject(bonus);
    
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		current->Update(window, dt);
	}

	// Do our Collision test here
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		for (int j = 0; j < m_gameObjects.size(); j++)
		{
			GameObject* other = m_gameObjects[j];
			if (current->IsCollidingWith(other))
			{
				current->CollidedWith(other);
			}
		}
	}

	for (int i = m_gameObjects.size() - 1; i >=0; i--)
	{
		GameObject* current = m_gameObjects[i];
		if (current->IsDestroyed())
		{
			delete current;
			m_gameObjects.erase(m_gameObjects.begin() + i);
		}
	}

	getPlayerLocation();

	if (!getProceed())
	{
		for (int i = m_gameObjects.size() - 1; i >= 0; i--)
		{
			GameObject* current = m_gameObjects[i];

			if (dynamic_cast<Player*>(current) != NULL && m_gameObjects.size() == 1)
			{
				dynamic_cast<Player*>(current)->setInvulernable();
				m_proceedLevel = true;
				m_levelTracker += 1;
			}
		}
	}
}

int Game::getLevel()
{
	return m_levelTracker;
}

void Game::setLive(int value){
    m_livesRemaining = value;
}

int Game::getLive(){
    return m_livesRemaining;
}

bool Game::getProceed()
{
	return m_proceedLevel;
}

void Game::updateProceed()
{
	m_proceedLevel = false;
}

void Game::AddObject(GameObject * object)
{
	object->SetOwner(this);
	m_gameObjects.push_back(object);
}

void Game::RespawnPlayer()
{
	if (m_livesRemaining > 0)
	{
		m_livesRemaining--;
		m_timeUntilRespawn = 2.0f;
	}
	else
	{
		GameOver();
	}
}

void Game::updateScore(int s)
{
	m_scoreTracker += s;
}

int Game::getScore()
{
	return m_scoreTracker;
}

void Game::resetScoreTracker()
{
	m_scoreTracker = 0;
}

void Game::getPlayerLocation()
{
	bool foundPlayer = false;
	for (int i = m_gameObjects.size() - 1; i >= 0; i--)
	{
		GameObject* current = m_gameObjects[i];
		if (dynamic_cast<Player*>(current) != NULL)
		{
			foundPlayer = true;
			for (int j = m_gameObjects.size() - 1; j >= 0; j--)
			{
				GameObject* alsoCurrent = m_gameObjects[j];
				if (dynamic_cast<Enemy*>(alsoCurrent) != NULL)
				{
					dynamic_cast<Enemy*>(alsoCurrent)->trackPlayer(dynamic_cast<Player*>(current)->currentPos(), dynamic_cast<Player*>(current)->getPRotation());
					dynamic_cast<Enemy*>(alsoCurrent)->firingPlayer(dynamic_cast<Player*>(current)->isFiring());
					dynamic_cast<Enemy*>(alsoCurrent)->spawningPlayer(dynamic_cast<Player*>(current)->isInvulnerable());
				}
			}
		}
	}

	if (!foundPlayer)
	{
		for (int i = m_gameObjects.size() - 1; i >= 0; i--)
		{
			GameObject* alsoCurrent = m_gameObjects[i];
			if (dynamic_cast<Enemy*>(alsoCurrent) != NULL)
			{
				dynamic_cast<Enemy*>(alsoCurrent)->spawningPlayer(true);
			}
		}
	}
}

ParticleSystem::ParticleSystem(float count, sf::Vector2f pos, sf::Color color, float size, int spread, float angle, float speed, int speedSpread)
	: GameObject("", pos)
{
	for (int i = 0; i < count; i++)
	{
		Particle newParticle;
		newParticle.m_pos = pos;
		newParticle.m_color = color;
		newParticle.m_size = size + randomSize(myGenerator) / 2;
		newParticle.m_lifetime = 1 + (randomLifetime(myGenerator) / 10.0f);
		newParticle.m_initialLifeTime = newParticle.m_lifetime;
		float randAngle = angle + (spread > 0 ? (rand() % spread) : 0);
		randAngle *= DEG_TO_RAD;
		float particleSpeed = speed + (speedSpread > 0 ? (rand() % speedSpread) : 0);
		newParticle.m_vel = sf::Vector2f(sin(randAngle) * particleSpeed, cos(randAngle) * particleSpeed);
		newParticle.m_accel = sf::Vector2f(0.5f * -sin(randAngle) * particleSpeed, -0.5f * cos(randAngle) * particleSpeed);

		m_particles.push_back(newParticle);
	}
}

void ParticleSystem::Draw(sf::RenderWindow* window)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		Particle& particle = m_particles[i];

		m_rectShape.setPosition(particle.m_pos);
		m_rectShape.setFillColor(particle.m_color);
		m_rectShape.setRadius(particle.m_size);
		m_rectShape.setPointCount(6);
		window->draw(m_rectShape);
	}
}

void ParticleSystem::Update(sf::RenderWindow* window, float dt)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		Particle& particle = m_particles[i];
		particle.m_vel += particle.m_accel * dt;
		particle.m_pos += particle.m_vel * dt;
		particle.m_lifetime -= dt;
		particle.m_color.a = 255 * particle.m_lifetime / particle.m_initialLifeTime;

		if (particle.m_lifetime <= 0.0f)
		{
			m_particles.erase(m_particles.begin() + i);
			i--;
		}
	}

	if (m_particles.size() <= 0)
	{
		Destroy();
	}
}

void Game::setAIRespawn()
{
	m_hotFix = false;
}
