#pragma once
#include <SFML/Graphics.hpp>

class GameState
{
public:
	GameState();
	virtual void Draw(sf::RenderWindow* window);
	void AddState(GameState* state);
	virtual void Update(sf::RenderWindow* window, float dt);
	virtual void SetOwner(GameState* state);
protected:
	bool m_gameOver;
	sf::Text outcomeText;
	sf::Font m_mainFont;
	std::vector<GameState*> m_gameStates;
	sf::Text scoreText;
	sf::Text textScore;
	sf::Text levelText;
	sf::Text textLevel;
};

class winState : public GameState
{
public:
	winState(int s, int x, int y);
	void SetOwner(GameState* owner);
private:
	int Size, posX, posY;
	GameState* m_owner;
};

class loseState : public GameState
{
public:
	loseState(int s, int x, int y);
	void SetOwner(GameState* owner);
private:
	int Size, posX, posY;
	GameState* m_owner;
};

class levelState : public GameState
{
public:
	levelState(int s, int x, int y);
	void SetOwner(GameState* owner);
	virtual void Update(sf::RenderWindow* window, float dt, int s, int l);
	int getCurrentScore();
private:
	int Size, posX, posY;
	int Score;
	int Level;
	GameState* m_owner;
};