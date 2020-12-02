#include "HighScore.h"
#include <SFML/Graphics.hpp>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

scoreBoard::scoreBoard()
	: yourScore(0), theHighScore(0)
{
	scoreBoardFont.loadFromFile("Fonts/kenpixel_high_square.ttf");
	scoreBoardText.setFont(scoreBoardFont);
	scoreBoardText.setString("THE HIGH SCORE IS: ");
	scoreBoardText.setCharacterSize(20);
	scoreBoardText.setOrigin(scoreBoardText.getLocalBounds().width / 2, scoreBoardText.getLocalBounds().height / 2);
	
	scoreBoardTextTwo.setFont(scoreBoardFont);
	scoreBoardTextTwo.setString("YOUR SCORE IS: ");
	scoreBoardTextTwo.setCharacterSize(20);
	scoreBoardTextTwo.setOrigin(scoreBoardTextTwo.getLocalBounds().width / 2, scoreBoardTextTwo.getLocalBounds().height / 2);
}

void scoreBoard::saveHighScore(int score)
{
	yourScore = score;
	ifstream inputSheet("scoreSheet.bin", ios::binary);
	inputSheet.read((char*)&(theHighScore), sizeof(theHighScore));

	if (yourScore > theHighScore)
	{
		ofstream outputSheet("scoreSheet.bin", ios::binary);
		outputSheet.write((char*)&(yourScore), sizeof(yourScore));
		outputSheet.close();
		inputSheet.close();
	}
	else if (theHighScore > yourScore)
	{
		inputSheet.close();
	}
}

void scoreBoard::inGameBoard(sf::RenderWindow* window, int current)
{
	scoreBoardText.setPosition(window->getSize().x / 2, 0);
	scoreBoardTextTwo.setPosition(window->getSize().x / 2, window->getSize().y / 2);
	yourScore = current;
	ifstream inputSheet("scoreSheet.bin", ios::binary);
	inputSheet.read((char*)&(theHighScore), sizeof(theHighScore));
	cout << theHighScore << endl;
	hScoreText.setFont(scoreBoardFont);
	hScoreText.setString(to_string(theHighScore));
	hScoreText.setCharacterSize(20);
	hScoreText.setOrigin(hScoreText.getLocalBounds().width / 2, hScoreText.getLocalBounds().height / 2);
	hScoreText.setPosition(window->getSize().x / 2, 50);

	cScoreText.setFont(scoreBoardFont);
	cScoreText.setString(to_string(yourScore));
	cScoreText.setCharacterSize(20);
	cScoreText.setOrigin(cScoreText.getLocalBounds().width / 2, cScoreText.getLocalBounds().height / 2);
	cScoreText.setPosition(window->getSize().x / 2, 170);

	window->draw(scoreBoardText);
	window->draw(scoreBoardTextTwo);
	window->draw(hScoreText);
	window->draw(cScoreText);
	inputSheet.close();
}