#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

class scoreBoard
{
public:
	scoreBoard();
	void saveHighScore(int score);
	void inGameBoard(sf::RenderWindow* window, int current);

private:
	ofstream outputSheet;
	ifstream inputSheet;
	sf::Font scoreBoardFont;
	sf::Text scoreBoardText;
	sf::Text scoreBoardTextTwo;
	sf::Text hScoreText;
	sf::Text cScoreText;
	int yourScore;
	int theHighScore;
};