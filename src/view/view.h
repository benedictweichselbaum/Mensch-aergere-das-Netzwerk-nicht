#pragma once

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

	ClientMadn client;
};

//PlaygroundField NewPlayground();

//	void DrawPlayground(PlaygroundField playgroundField);

//void RunView(PlaygroundField playgroundField);

//std::vector<std::vector<sf::CircleShape>> CreatePlayground();

//std::vector<std::vector<sf::CircleShape>> InitPlayground();

//void CreatePlayground(std::vector<std::vector<sf::CircleShape>>& listOfCircles);

void DrawPlayground(sf::RenderWindow& window, std::vector<std::vector<sf::CircleShape>>& listOfCircles);
void DrawSidebar(sf::RenderWindow& window, Sidebar sidebar);
void DrawDice(sf::RenderWindow& window, Dice dice);
void DrawPlayers(sf::RenderWindow& window, View view);
void DrawRectangle(sf::RenderWindow& window, sf::RectangleShape rectangle);
void DrawLines(sf::RenderWindow& window, std::vector<sf::RectangleShape> lineList);

void RunView(sf::RenderWindow& window, View& view);
void RunMouseButtonReleased(sf::RenderWindow& window, View& view);

void RunMouseButtonReleasedPlayerMeeples(sf::RenderWindow& window, View& view, sf::Vector2f& mousePosF);

void RunMouseButtonReleasedPlayerMeeple(sf::RenderWindow& window, View& view, sf::Vector2f& mousePosF, sf::CircleShape& meeple, int playerId, int meepleNr);

std::vector<std::vector<sf::CircleShape>> CreateField();

void ColorizeCircle(sf::Color color, sf::CircleShape& circle);

sf::CircleShape CreateCircle(float x, float y, sf::Color farbe);
sf::CircleShape CreateDot(float x, float y, sf::Color farbe);
sf::RectangleShape CreateDiceShape(float x, float y, sf::Color farbe);

std::vector<std::vector<sf::CircleShape>> InitializePlayground();

std::vector<sf::RectangleShape> InitializeListOfLines(); //Background of Path (under the Circles)

sf::RectangleShape CreateRectangleLine(bool horizontal, int startx, int starty, int length);

Sidebar InitializeSidebar();

View InitializeView();

void InitializeAllPlayers(View& view);

Dice InitializeDice();

Player InitPlayer();

Meeple InitMeeple();

sf::RectangleShape InitRollTheDiceButton();

//	void setPositions(View& view, std::string Coords);
