// Engine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameObjects.h"
#include "GameStates.h"
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include "StarParticleSystem.h"
#include "HighScore.h"

using namespace std;
bool fullscreen = false;

int main()
{
	scoreBoard gameScoreBoard;

	mt19937 randomGenerator(time(0));
	uniform_int_distribution<int> randomLocationX(0, 1000);
	uniform_int_distribution<int> randomLocationY(0, 600);
	uniform_int_distribution<int> randomSize(1, 2);
	uniform_int_distribution<int> randomSpeed(100, 300);

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Welcome to SDVA 213!");		
	sf::RenderWindow ingameScoreBoard;
	Game game;
	GameState currentState;
	sf::Image starImage;
	starImage.create(window.getSize().x +4, window.getSize().y, sf::Color::Black);
	sf::Texture starTexture;
	starTexture.loadFromImage(starImage);
	starTexture.setSmooth(false);
	sf::Sprite starSprite;
	starSprite.setTexture(starTexture);
	starSprite.setPosition(0, 0);
	Starfield backgroundStars(&window);

	for (int i = 0; i < 5; i++)
	{
			LargeAsteroid* newAsteroid = new LargeAsteroid(sf::Vector2f(randomLocationX(randomGenerator), randomLocationY(randomGenerator)));
			newAsteroid->SetVelocity(randomSpeed(randomGenerator));
			game.AddObject(newAsteroid);
	}
	game.RespawnPlayer();
	loseState* possibleState = new loseState(50, 1280 / 2, 720 / 2);
	currentState.AddState(possibleState);
	levelState* currentLevel = new levelState(30, 25, 10);
	currentState.AddState(currentLevel);
	
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Return &&
					event.key.alt)
				{					
					window.close();
					fullscreen = !fullscreen;
					window.create(fullscreen ? sf::VideoMode(1920,1080) : sf::VideoMode(1280, 720), "Welcome to SDVA 213!", fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar);
				}
				if (event.key.code == sf::Keyboard::Tab)
				{
					ingameScoreBoard.create(sf::VideoMode(600, 300), "The Score to Beat!", sf::Style::Default);
					while (ingameScoreBoard.isOpen())
					{
						sf::Event event;
						while (ingameScoreBoard.pollEvent(event))
						{
							if (event.type == sf::Event::KeyReleased)
							{
								if (event.key.code == sf::Keyboard::Tab)
								{
									ingameScoreBoard.close();
									cout << "Testing" << endl;
								}
							}
						}
						ingameScoreBoard.clear(sf::Color(0, 0, 0));
						gameScoreBoard.inGameBoard(&ingameScoreBoard, currentLevel->getCurrentScore());
						ingameScoreBoard.display();
					}
				}
			}
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
			starTexture.loadFromImage(starImage);
			backgroundStars.Draw(starTexture);
			window.clear(sf::Color(0,0,0));
			window.draw(starSprite);
			game.Update(&window, dt.asSeconds());
			game.Draw(&window);
			if (game.isGameOver())
			{
				gameScoreBoard.saveHighScore(currentLevel->getCurrentScore());
				currentState.Update(&window, dt.asSeconds());
			}
			if (!game.isGameOver())
			{
				currentLevel->Update(&window, dt.asSeconds(), game.getScore(), game.getLevel());
				game.resetScoreTracker();
				if (game.getProceed())
				{
					for (int i = 0; i < 5 + game.getLevel(); i++)
					{
						LargeAsteroid* newAsteroid = new LargeAsteroid(sf::Vector2f(randomLocationX(randomGenerator), randomLocationY(randomGenerator)));
						newAsteroid->SetVelocity(randomSpeed(randomGenerator));
						game.AddObject(newAsteroid);
						cout << game.getLevel() << endl;
					}
					game.updateProceed();
					game.setAIRespawn();
				}
			}
			window.display();
			backgroundStars.Update(&window, dt.asSeconds());
	}
	return 0;
}
