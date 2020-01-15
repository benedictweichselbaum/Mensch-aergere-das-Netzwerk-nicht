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
	int numberForServer;
private:
	//bool clickable;
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

struct Minigame
{
	int result;
	std::string answer;
	std::string taskstring;
	sf::RectangleShape background;
	sf::Text* text;
	sf::Text* task;
	sf::RectangleShape newTaskButton;
	sf::Text* newTaskButtonText;
	void NewTask();
};

struct Sidebar
{
	Dice dice;
	sf::RectangleShape startButton;
	sf::Text* startButtonText;
	sf::RectangleShape passButton;
	sf::Text* passButtonText;
	sf::RectangleShape rollTheDiceButton;
	sf::Text* rollTheDiceButtonText;
	sf::RectangleShape SaveAndCloseButton;
	sf::Text* SaveAndCloseButtonText;
	sf::Text* infotext;
	sf::Font* font;
	Minigame minigame;
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
	std::vector<sf::RectangleShape> showsWhichPlayer;
	void setPositions(std::string Coords);
	void setPositionOfMeeple(int x, int y, int playerNr, int& player1set, int& player2set, int& player3set, int& player4set, int& player1numberForServer, int& player2numberForServer, int& player3numberForServer, int& player4numberForServer);
	void CommunicateWithClient(std::string message); 
	void SetErrorMessage(std::string message);
	void CheckForAnswer();

	ClientMadnPtr client;
};

using ViewPtr = std::shared_ptr<View>;

void DrawPlayground(sf::RenderWindow& window, std::vector<std::vector<sf::CircleShape>>& listOfCircles);
void DrawSidebar(sf::RenderWindow& window, Sidebar sidebar);
void DrawMinigame(sf::RenderWindow& window, Minigame minigame);
void DrawDice(sf::RenderWindow& window, Dice dice);
void DrawPlayers(sf::RenderWindow& window, ViewPtr view);
void DrawRectangle(sf::RenderWindow& window, sf::RectangleShape rectangle);
void DrawLines(sf::RenderWindow& window, std::vector<sf::RectangleShape> lineList);
void DrawText(sf::RenderWindow& window, sf::Text text);
void DrawShowWhichPlayer(sf::RenderWindow& window, ViewPtr view);

void RunView(sf::RenderWindow& window, ViewPtr view);
void RunMouseButtonReleased(sf::RenderWindow& window, ViewPtr view);

void RunMouseButtonReleasedPlayerMeeples(sf::RenderWindow& window, ViewPtr view, sf::Vector2f& mousePosF);

void RunMouseButtonReleasedPlayerMeeple(sf::RenderWindow& window, ViewPtr view, sf::Vector2f& mousePosF, sf::CircleShape& meeple, int playerId, int meepleNr);

void RunMinigameInput(Minigame& minigame, sf::Event event);

std::vector<std::vector<sf::CircleShape>> CreateField();

void ColorizeCircle(sf::Color color, sf::CircleShape& circle);

sf::CircleShape CreateCircle(float x, float y, sf::Color farbe);
sf::CircleShape CreateDot(float x, float y, sf::Color farbe);
sf::RectangleShape CreateDiceShape(float x, float y, sf::Color farbe);

std::vector<std::vector<sf::CircleShape>> InitializePlayground();

std::vector<sf::RectangleShape> InitializeListOfLines(); // Pfade im Hintergrund(unter den Kreisen)

std::vector<sf::RectangleShape> InitializeShowsWhichPlayer(); // Hervorhebung des Starthauses des Spielers

sf::RectangleShape CreateShowsWhichPlayer(int x, int y);

sf::RectangleShape CreateRectangleLine(bool horizontal, int startx, int starty, int length);

Sidebar InitializeSidebar();

Minigame InitializeMinigame(Sidebar& sidebar);

ViewPtr InitializeView(ClientMadnPtr client_ptr);

void InitializeAllPlayers(ViewPtr view);

Dice InitializeDice();

Player InitPlayer();

Meeple InitMeeple();

sf::RectangleShape InitRollTheDiceButton(Sidebar& sidebar);

sf::RectangleShape InitSaveAndCloseButton(Sidebar& sidebar);

sf::RectangleShape InitTopButton(int x, std::string text, Sidebar& sidebar);

//	void setPositions(ViewPtr view, std::string Coords);

#endif