#include <iostream>
#include <SFML/Graphics.hpp>
#include "main.h"
#include "view/config.h"
#include "view/view.h"

int main () {
    std::cout << "Some crazy shit happening!" << std::endl;
    //Some crazy code that does the magic.
    //Noch mehr Kommentare um das mit Git auszuprobieren
	TestView();
}

void TestView()
{
	sf::RenderWindow window(sf::VideoMode(PLAYGROUNDINTERVAL * 11 + SIDEPANEL, PLAYGROUNDINTERVAL * 11), "Mensch ärgere dich nicht");
	View view = InitializeView();
	window.setFramerateLimit(30);
	//view.setPositions("4000040000400004000000000000000000000000000000000000000000002360");
	view.setPositions("3100031000310003100000000000000000000000000000000000000000002360");
	//view.setPositions("4000030000200101100000000000020000400000000040000030000000002360");
	//view.setPositions("00000000000000000000040123300002300040010100003020400040000100202360");
	RunView(window, view);
}