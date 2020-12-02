#include "GameStates.h"
#include "stdafx.h"

using namespace std;

GameState::GameState()
	: m_gameOver(false) {}

void GameState::Draw(sf::RenderWindow* window)
{
	if (m_gameOver)
	{
		window->draw(outcomeText);
	}
}

void GameState::AddState(GameState* state)
{
	state->SetOwner(this);
	m_gameStates.push_back(state);
}

void GameState::Update(sf::RenderWindow* window, float dt)
{
	for (int i = 0; i < m_gameStates.size(); i++)
	{
		GameState* current = m_gameStates[i];
		if (current->m_gameOver == true)
		{
			current->Draw(window);
		}
	}
}

void GameState::SetOwner(GameState* state) {}

winState::winState(int s, int x, int y) 
	: GameState::GameState(), Size(0), posX(0), posY(0)
{
	m_gameOver = true;
	Size = s; posX = x; posY = y;
	m_mainFont.loadFromFile("Fonts/kenpixel_high_square.ttf");
	outcomeText.setFont(m_mainFont);
	outcomeText.setString("GAME OVER! VICTORY!");
	outcomeText.setCharacterSize(Size);
	outcomeText.setPosition(posX, posY);
}

void winState::SetOwner(GameState* owner)
{
	m_owner = owner;
}

loseState::loseState(int s, int x, int y) 
	: GameState::GameState(), Size(0), posX(0), posY(0)
{
	m_gameOver = true;
	Size = s; posX = x; posY = y;
	m_mainFont.loadFromFile("Fonts/kenpixel_high_square.ttf");
	outcomeText.setFont(m_mainFont);
	outcomeText.setString("GAME OVER! YOU LOSE!");
	outcomeText.setCharacterSize(Size);
	outcomeText.setPosition(posX, posY);
	outcomeText.setOrigin(outcomeText.getLocalBounds().width / 2, outcomeText.getLocalBounds().height / 2);
}

void loseState::SetOwner(GameState* owner)
{
	m_owner = owner;
}

levelState::levelState(int s, int x, int y)
	: GameState::GameState(), Size(0), posX(0), posY(0)
{
	Size = s; posX = x; posY = y;
	Score = 0;
	Level = 1;
	m_mainFont.loadFromFile("Fonts/kenpixel_high_square.ttf");
	scoreText.setFont(m_mainFont);
	scoreText.setString("Score: ");
	scoreText.setCharacterSize(Size);
	scoreText.setPosition(posX, posY);
	textScore.setFont(m_mainFont);
	textScore.setString(to_string(Score));
	textScore.setCharacterSize(Size);
	textScore.setPosition(posX + 100, posY);

	levelText.setFont(m_mainFont);
	levelText.setString("Level: ");
	levelText.setCharacterSize(Size);
	levelText.setPosition(posX + 1100, posY);
	textLevel.setFont(m_mainFont);
	textLevel.setString(to_string(Level));
	textLevel.setCharacterSize(Size);
	textLevel.setPosition(posX + 1200, posY);
}

void levelState::SetOwner(GameState* owner)
{
	m_owner = owner;
}

void levelState::Update(sf::RenderWindow* window, float dt, int s, int l)
{
	GameState::Update(window, dt);
	Score += s;
	Level = l;
	textScore.setString(to_string(Score));
	textLevel.setString(to_string(Level));
	window->draw(scoreText);
	window->draw(textScore);
	window->draw(levelText);
	window->draw(textLevel);
}

int levelState::getCurrentScore()
{
	return Score;
}