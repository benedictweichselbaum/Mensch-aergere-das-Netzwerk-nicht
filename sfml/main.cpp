#include<SFML/Graphics.hpp>
#include "main.h"
#include "config.h"
#include "view.h"

#define RADIUS	40.f
#define ABSTAND 90.f

int main()
{
	sf::RenderWindow window(sf::VideoMode(PLAYGROUNDINTERVAL * 11 + SIDEPANEL, PLAYGROUNDINTERVAL * 11), "Mensch �rgere dich nicht");
	View view = InitializeView();
	//view.sidebar.dice.Set(5);
	//view.setPositions("4000040000400004000000000000000000000000000000000000000000002360");
	view.setPositions("3100031000310003100000000000000000000000000000000000000000002360");
	//view.setPositions("4000030000200101100000000000020000400000000040000030000000002360");
	//view.setPositions("00000000000000000000040123300002300040010100003020400040000100202360");
	RunView(window, view);



	//sf::RenderWindow window(sf::VideoMode(990, 990), "Mensch �rgere dich nicht");

	//std::vector<std::vector<sf::CircleShape>> listOfCircles = FeldInitialisieren();

	//

	//while (window.isOpen())
	//{
	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//			window.close();
	//	}

	//	window.clear();
	//	DrawPlayground(window, listOfCircles);
	//	window.display();
	//}

	return 0;
}

//void DrawPlaygroundMAIN(sf::RenderWindow& window, std::vector<std::vector<sf::CircleShape>>& listOfCircles)
//{
//	for (int i = 0; i < listOfCircles.size(); ++i)
//	{
//		for (int j = 0; j < listOfCircles.at(i).size(); ++j)
//		{
//			window.draw(listOfCircles.at(i).at(j));
//		}
//	}
//}

//std::vector<std::vector<sf::CircleShape>> FeldErstellen()//--
//{
//	std::vector<std::vector<sf::CircleShape>> feld;
//	for (int i = 0; i < 11; i++)
//	{
//		std::vector<sf::CircleShape> reihe;
//		for (int j = 0; j < 11; j++)
//		{
//			reihe.push_back(ErstelleKreis(j, i, sf::Color::Black));
//		}
//		feld.push_back(reihe);
//	}
//
//	return feld;
//}
//
//std::vector<std::vector<sf::CircleShape>> FeldInitialisieren()//--
//{
//	std::vector<std::vector<sf::CircleShape>> listOfCircles = FeldErstellen();
//
//	//Gelbe Start
//	KreisF�rben(sf::Color::Yellow, listOfCircles.at(0).at(0));
//	KreisF�rben(sf::Color::Yellow, listOfCircles.at(0).at(1));
//	KreisF�rben(sf::Color::Yellow, listOfCircles.at(1).at(0));
//	KreisF�rben(sf::Color::Yellow, listOfCircles.at(1).at(1));
//
//	KreisF�rben(sf::Color::White, listOfCircles.at(0).at(4));
//	KreisF�rben(sf::Color::White, listOfCircles.at(0).at(5));
//	KreisF�rben(sf::Color::Green, listOfCircles.at(0).at(6));
//
//	//Gr�ne Start
//	KreisF�rben(sf::Color::Green, listOfCircles.at(0).at(9));
//	KreisF�rben(sf::Color::Green, listOfCircles.at(0).at(10));
//	KreisF�rben(sf::Color::Green, listOfCircles.at(1).at(9));
//	KreisF�rben(sf::Color::Green, listOfCircles.at(1).at(10));
//
//	KreisF�rben(sf::Color::White, listOfCircles.at(1).at(4));
//	KreisF�rben(sf::Color::Green, listOfCircles.at(1).at(5));
//	KreisF�rben(sf::Color::White, listOfCircles.at(1).at(6));
//
//	KreisF�rben(sf::Color::White, listOfCircles.at(2).at(4));
//	KreisF�rben(sf::Color::Green, listOfCircles.at(2).at(5));
//	KreisF�rben(sf::Color::White, listOfCircles.at(2).at(6));
//
//	KreisF�rben(sf::Color::White, listOfCircles.at(3).at(4));
//	KreisF�rben(sf::Color::Green, listOfCircles.at(3).at(5));
//	KreisF�rben(sf::Color::White, listOfCircles.at(3).at(6));
//
//	KreisF�rben(sf::Color::Yellow, listOfCircles.at(4).at(0));
//	KreisF�rben(sf::Color::White, listOfCircles.at(4).at(1));
//	KreisF�rben(sf::Color::White, listOfCircles.at(4).at(2));
//	KreisF�rben(sf::Color::White, listOfCircles.at(4).at(3));
//	KreisF�rben(sf::Color::White, listOfCircles.at(4).at(4));
//	KreisF�rben(sf::Color::Green, listOfCircles.at(4).at(5));
//	KreisF�rben(sf::Color::White, listOfCircles.at(4).at(6));
//	KreisF�rben(sf::Color::White, listOfCircles.at(4).at(7));
//	KreisF�rben(sf::Color::White, listOfCircles.at(4).at(8));
//	KreisF�rben(sf::Color::White, listOfCircles.at(4).at(9));
//	KreisF�rben(sf::Color::White, listOfCircles.at(4).at(10));
//
//	KreisF�rben(sf::Color::White, listOfCircles.at(5).at(0));
//	KreisF�rben(sf::Color::Yellow, listOfCircles.at(5).at(1));
//	KreisF�rben(sf::Color::Yellow, listOfCircles.at(5).at(2));
//	KreisF�rben(sf::Color::Yellow, listOfCircles.at(5).at(3));
//	KreisF�rben(sf::Color::Yellow, listOfCircles.at(5).at(4));
//	KreisF�rben(sf::Color::Red, listOfCircles.at(5).at(6));
//	KreisF�rben(sf::Color::Red, listOfCircles.at(5).at(7));
//	KreisF�rben(sf::Color::Red, listOfCircles.at(5).at(8));
//	KreisF�rben(sf::Color::Red, listOfCircles.at(5).at(9));
//	KreisF�rben(sf::Color::White, listOfCircles.at(5).at(10));
//
//	KreisF�rben(sf::Color::White, listOfCircles.at(6).at(0));
//	KreisF�rben(sf::Color::White, listOfCircles.at(6).at(1));
//	KreisF�rben(sf::Color::White, listOfCircles.at(6).at(2));
//	KreisF�rben(sf::Color::White, listOfCircles.at(6).at(3));
//	KreisF�rben(sf::Color::White, listOfCircles.at(6).at(4));
//	KreisF�rben(sf::Color::Blue, listOfCircles.at(6).at(5));
//	KreisF�rben(sf::Color::White, listOfCircles.at(6).at(6));
//	KreisF�rben(sf::Color::White, listOfCircles.at(6).at(7));
//	KreisF�rben(sf::Color::White, listOfCircles.at(6).at(8));
//	KreisF�rben(sf::Color::White, listOfCircles.at(6).at(9));
//	KreisF�rben(sf::Color::Red, listOfCircles.at(6).at(10));
//
//	KreisF�rben(sf::Color::White, listOfCircles.at(7).at(4));
//	KreisF�rben(sf::Color::Blue, listOfCircles.at(7).at(5));
//	KreisF�rben(sf::Color::White, listOfCircles.at(7).at(6));
//
//	KreisF�rben(sf::Color::White, listOfCircles.at(8).at(4));
//	KreisF�rben(sf::Color::Blue, listOfCircles.at(8).at(5));
//	KreisF�rben(sf::Color::White, listOfCircles.at(8).at(6));
//
//	KreisF�rben(sf::Color::White, listOfCircles.at(9).at(4));
//	KreisF�rben(sf::Color::Blue, listOfCircles.at(9).at(5));
//	KreisF�rben(sf::Color::White, listOfCircles.at(9).at(6));
//
//	KreisF�rben(sf::Color::Blue, listOfCircles.at(10).at(4));
//	KreisF�rben(sf::Color::White, listOfCircles.at(10).at(5));
//	KreisF�rben(sf::Color::White, listOfCircles.at(10).at(6));
//
//	KreisF�rben(sf::Color::Blue, listOfCircles.at(9).at(0));
//	KreisF�rben(sf::Color::Blue, listOfCircles.at(9).at(1));
//	KreisF�rben(sf::Color::Blue, listOfCircles.at(10).at(0));
//	KreisF�rben(sf::Color::Blue, listOfCircles.at(10).at(1));
//
//	KreisF�rben(sf::Color::Red, listOfCircles.at(9).at(9));
//	KreisF�rben(sf::Color::Red, listOfCircles.at(9).at(10));
//	KreisF�rben(sf::Color::Red, listOfCircles.at(10).at(9));
//	KreisF�rben(sf::Color::Red, listOfCircles.at(10).at(10));
//
//
//
//	return listOfCircles;
//}
//
//void FeldZeichnen(std::vector<std::vector<sf::CircleShape>>& listOfCircles)//--
//{
//	for (int i = 0; i < listOfCircles.size(); ++i)
//	{
//		for (int j = 0; j < listOfCircles.at(i).size(); ++j)
//		{
//			//window.draw(listOfCircles.at(i).at(j));
//		}
//	}
//}
//
//void KreisF�rben(sf::Color farbe, sf::CircleShape& circle)//--
//{
//	circle.setFillColor(farbe);
//}
//
//sf::CircleShape ErstelleKreis(float x, float y, sf::Color farbe)//--
//{
//	sf::CircleShape shape(RADIUS);
//	shape.setPosition(ABSTAND * x + (ABSTAND - RADIUS * 2) / 2, ABSTAND * y + (ABSTAND - RADIUS * 2) / 2);
//	shape.setFillColor(sf::Color::Black);
//
//	return shape;
//}