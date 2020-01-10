#ifndef H_VIEW
#define H_VIEW

#include <memory>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "config.hpp"
#include "../clientSide/client/madn_client.hpp"



//Erst NewPlayground erstellen und mit RunView (in Thread!) laufen lassen.

struct Meeple
{
	sf::CircleShape meeple;
private:
	bool clickable;
};

struct Player
{
	std::vector<Meeple> Meeples;
};

struct PlaygroundField
{
	sf::RenderWindow window;
	std::vector<std::vector<sf::CircleShape>> listOfCircles;
};

struct Dice
{
	void Set(int number);
	sf::RectangleShape diceShape;
	std::vector<sf::CircleShape> listOfDiceDots;
	void Clear();
};

struct Sidebar
{
	Dice dice;
	sf::RectangleShape rollTheDiceButton;
	sf::Text infotext;
};

struct View
{
	std::vector<std::vector<sf::CircleShape>> listOfCircles;
	std::vector<sf::RectangleShape> listOfLines;
	Sidebar sidebar;
	sf::RectangleShape background;
	Player Player1;
	Player Player2;
	Player Player3;
	Player Player4;
	void setPositions(std::string Coords);
	void setPositionOfMeeple(int x, int y, int playerNr, int& player1set, int& player2set, int& player3set, int& player4set);
	void CommunicateWithClient(std::string message);

	ClientMadnPtr client;

};

using ViewPtr = std::shared_ptr<View>;

void DrawPlayground(sf::RenderWindow& window, std::vector<std::vector<sf::CircleShape>>& listOfCircles);
void DrawSidebar(sf::RenderWindow& window, Sidebar sidebar);
void DrawDice(sf::RenderWindow& window, Dice dice);
void DrawPlayers(sf::RenderWindow& window, ViewPtr view);
void DrawRectangle(sf::RenderWindow& window, sf::RectangleShape rectangle);
void DrawLines(sf::RenderWindow& window, std::vector<sf::RectangleShape> lineList);

void RunView(sf::RenderWindow& window, ViewPtr view);
void RunMouseButtonReleased(sf::RenderWindow& window, ViewPtr view);

void RunMouseButtonReleasedPlayerMeeples(sf::RenderWindow& window, ViewPtr view, sf::Vector2f& mousePosF);

void RunMouseButtonReleasedPlayerMeeple(sf::RenderWindow& window, ViewPtr view, sf::Vector2f& mousePosF, sf::CircleShape& meeple, int playerId, int meepleNr);

std::vector<std::vector<sf::CircleShape>> CreateField();

void ColorizeCircle(sf::Color color, sf::CircleShape& circle);

sf::CircleShape CreateCircle(float x, float y, sf::Color farbe);
sf::CircleShape CreateDot(float x, float y, sf::Color farbe);
sf::RectangleShape CreateDiceShape(float x, float y, sf::Color farbe);

std::vector<std::vector<sf::CircleShape>> InitializePlayground();

std::vector<sf::RectangleShape> InitializeListOfLines(); //Background of Path (under the Circles)

sf::RectangleShape CreateRectangleLine(bool horizontal, int startx, int starty, int length);

Sidebar InitializeSidebar();

ViewPtr InitializeView(ClientMadnPtr client_ptr);

void InitializeAllPlayers(ViewPtr view);

Dice InitializeDice();

Player InitPlayer();

Meeple InitMeeple();

sf::RectangleShape InitRollTheDiceButton();

//	void setPositions(ViewPtr view, std::string Coords);

#endif