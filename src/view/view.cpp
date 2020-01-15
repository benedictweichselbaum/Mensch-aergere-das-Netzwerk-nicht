#include "view.hpp"
#include <regex>

void DrawPlayground(sf::RenderWindow& window, std::vector<std::vector<sf::CircleShape>>& listOfCircles)
{
	for (int i = 0; i < listOfCircles.size(); ++i)
	{
		for (int j = 0; j < listOfCircles.at(i).size(); ++j)
		{
			window.draw(listOfCircles.at(i).at(j));
		}
	}
}

void DrawSidebar(sf::RenderWindow& window, Sidebar sidebar)
{
	DrawDice(window, sidebar.dice);
	DrawRectangle(window, sidebar.rollTheDiceButton);
	DrawRectangle(window, sidebar.startButton);
	DrawRectangle(window, sidebar.passButton);
	DrawRectangle(window, sidebar.SaveAndCloseButton);
	DrawText(window, *sidebar.infotext);
	DrawText(window, *sidebar.passButtonText);
	DrawText(window, *sidebar.startButtonText);
	DrawText(window, *sidebar.rollTheDiceButtonText);
	DrawText(window, *sidebar.SaveAndCloseButtonText);
	DrawMinigame(window, sidebar.minigame);
}

void DrawMinigame(sf::RenderWindow& window, Minigame minigame)
{
	window.draw(minigame.background);
	window.draw(*minigame.task);
	window.draw(*minigame.text);
	window.draw(minigame.newTaskButton);
	window.draw(*minigame.newTaskButtonText);
}

void DrawDice(sf::RenderWindow& window, Dice dice)
{
	window.draw(dice.diceShape);
	for (int i = 0; i < dice.listOfDiceDots.size(); ++i)
	{
		window.draw(dice.listOfDiceDots.at(i));
	}
}

void DrawPlayers(sf::RenderWindow& window, ViewPtr view)
{
	for (int i = 0; i < 4; ++i)
	{
		window.draw(view->Player1.Meeples.at(i).meeple);
		window.draw(view->Player2.Meeples.at(i).meeple);
		window.draw(view->Player3.Meeples.at(i).meeple);
		window.draw(view->Player4.Meeples.at(i).meeple);
	}
}

void DrawRectangle(sf::RenderWindow& window, sf::RectangleShape rectangle)
{
	window.draw(rectangle);
}

void DrawText(sf::RenderWindow& window, sf::Text text)
{
	window.draw(text);
}

void DrawLines(sf::RenderWindow& window, std::vector<sf::RectangleShape> lineList)
{
	for (int i = 0; i < lineList.size(); ++i)
	{
		DrawRectangle(window, lineList.at(i));
	}
}

void RunView(sf::RenderWindow& window, ViewPtr view)
{
	while (window.isOpen())
	{
		view->CheckForAnswer();
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonReleased:
				RunMouseButtonReleased(window, view);
				break;
			case sf::Event::MouseEntered:

				break;
			case sf::Event::TextEntered:
				RunMinigameInput(view->sidebar.minigame, event);
				break;
			case sf::Event::KeyPressed:
				RunMinigameInput(view->sidebar.minigame, event);
				break;
			}
			
		}

		window.clear();
		window.draw(view->background);
		DrawShowWhichPlayer(window, view);
		DrawLines(window, view->listOfLines);
		DrawPlayground(window, view->listOfCircles);
		DrawPlayers(window, view);
		DrawSidebar(window, view->sidebar);
		window.display();
	}
}

void DrawShowWhichPlayer(sf::RenderWindow& window, ViewPtr view)
{
	if (view->client->playerNumber >= 1 && view->client->playerNumber <= 4)
	{
		DrawRectangle(window, view->showsWhichPlayer[view->client->playerNumber - 1]);
	}
}

void RunMouseButtonReleased(sf::RenderWindow& window, ViewPtr view)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	if (view->sidebar.rollTheDiceButton.getGlobalBounds().contains(mousePosF))
	{
		view->CommunicateWithClient("D");
	}
	else if (view->sidebar.startButton.getGlobalBounds().contains(mousePosF))
	{
		view->CommunicateWithClient("start");
	}
	else if (view->sidebar.passButton.getGlobalBounds().contains(mousePosF))
	{
		view->CommunicateWithClient("P");
	}
	else if (view->sidebar.SaveAndCloseButton.getGlobalBounds().contains(mousePosF))
	{
		view->CommunicateWithClient("save");
	}
	else if (view->sidebar.minigame.newTaskButton.getGlobalBounds().contains(mousePosF))
	{
		view->sidebar.minigame.NewTask();
	}
	RunMouseButtonReleasedPlayerMeeples(window, view, mousePosF);
}

void RunMouseEntered(sf::RenderWindow& window, ViewPtr view)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	if (view->sidebar.rollTheDiceButton.getGlobalBounds().contains(mousePosF))
	{
		std::cout << "In Wuerfeln.";
	}
	RunMouseButtonReleasedPlayerMeeples(window, view, mousePosF);
}

void RunMouseButtonReleasedPlayerMeeples(sf::RenderWindow& window, ViewPtr view, sf::Vector2f& mousePosF)
{
	for (int i = 0; i < 4; ++i)
	{
		RunMouseButtonReleasedPlayerMeeple(window, view, mousePosF, view->Player1.Meeples.at(i).meeple, 1, view->Player1.Meeples.at(i).numberForServer);
		RunMouseButtonReleasedPlayerMeeple(window, view, mousePosF, view->Player2.Meeples.at(i).meeple, 2, view->Player2.Meeples.at(i).numberForServer);
		RunMouseButtonReleasedPlayerMeeple(window, view, mousePosF, view->Player3.Meeples.at(i).meeple, 3, view->Player3.Meeples.at(i).numberForServer);
		RunMouseButtonReleasedPlayerMeeple(window, view, mousePosF, view->Player4.Meeples.at(i).meeple, 4, view->Player4.Meeples.at(i).numberForServer);
	}
}

void RunMouseButtonReleasedPlayerMeeple(sf::RenderWindow& window, ViewPtr view, sf::Vector2f& mousePosF, sf::CircleShape& meeple, int playerId, int meepleNr)
{
	if (meeple.getGlobalBounds().contains(mousePosF))
	{
		if (view->client->playerNumber == playerId)
		{
			view->CommunicateWithClient(std::to_string(meepleNr));
		}
	}
}

void RunMinigameInput(Minigame& minigame, sf::Event event)
{
	if (event.text.unicode == 108)
	{
		minigame.answer = "";
	}
	else if ((event.text.unicode == 122 || event.text.unicode == 114 ) && minigame.answer.size() != 0)
	{
		minigame.answer.pop_back();
	}

	if (event.KeyPressed == sf::Keyboard::BackSpace && minigame.answer.size() != 0) {
		minigame.answer.pop_back();
	}
	else if (event.text.unicode < 128) {
		minigame.answer.push_back((char)event.text.unicode);
	}
	minigame.answer = std::regex_replace(minigame.answer, std::regex(R"([\D])"), "");

	//int i = std::stoi(minigame.answer);
	const char* str = minigame.answer.c_str();
	int j;

	if (sscanf_s(str, "%d", &j) != EOF)
	{
		if (minigame.result == std::stoi(minigame.answer))
		{
			minigame.task->setFillColor(sf::Color::Green);
		}
		else
		{
			minigame.task->setFillColor(MINIGAMETEXTCOLOR);
		}
	}
	else
	{
		minigame.task->setFillColor(MINIGAMETEXTCOLOR);
	}
		
	minigame.task->setString(minigame.taskstring + minigame.answer);
}

std::vector<std::vector<sf::CircleShape>> CreateField()
{
	std::vector<std::vector<sf::CircleShape>> field;
	for (int i = 0; i < 11; i++)
	{
		std::vector<sf::CircleShape> row;
		for (int j = 0; j < 11; j++)
		{
			row.push_back(CreateCircle(j, i, sf::Color::Transparent));
		}
		field.push_back(row);
	} 

	return field;
}

void ColorizeCircle(sf::Color color, sf::CircleShape& circle)
{
	circle.setFillColor(color);
	circle.setOutlineColor(PLAYGROUNDCOLOROUTLINE);
	circle.setOutlineThickness(PLAYGROUNDTHICKNESSOUTLINE);
}

sf::CircleShape CreateCircle(float x, float y, sf::Color color)
{
	sf::CircleShape shape(PLAYGROUNDCIRCLESRADIUS);
	shape.setPosition(PLAYGROUNDINTERVAL * x + (PLAYGROUNDINTERVAL - PLAYGROUNDCIRCLESRADIUS * 2) / 2, PLAYGROUNDINTERVAL * y + (PLAYGROUNDINTERVAL - PLAYGROUNDCIRCLESRADIUS * 2) / 2);
	shape.setFillColor(color);
	return shape;
}

sf::CircleShape CreateDot(float x, float y, sf::Color farbe)
{
	sf::CircleShape shape(DICEDOTRADIUS);
	shape.setPosition(PLAYGROUNDINTERVAL * 11 + ((SIDEPANEL - DICESIZE) / 2) + DICEPADDING + (((((DICESIZE - (2 * DICEPADDING)) - 6 * DICEDOTRADIUS)/2) + (DICEDOTRADIUS * 2)) * x), DICEYPLACE + DICEPADDING + (((((DICESIZE - (2 * DICEPADDING)) - 6 * DICEDOTRADIUS) / 2) + (DICEDOTRADIUS * 2)) * y));
	return shape;
}

sf::RectangleShape CreateDiceShape(float x, float y, sf::Color color)
{
	sf::RectangleShape diceshape(sf::Vector2f(DICESIZE, DICESIZE));
	diceshape.setPosition(x, y);
	diceshape.setFillColor(color);
	return diceshape;
}

std::vector<std::vector<sf::CircleShape>> InitializePlayground()
{
	std::vector<std::vector<sf::CircleShape>> listOfCircles = CreateField();

	//Gelbe Start
	ColorizeCircle(PLAYGROUNDCOLORPLAYER3, listOfCircles.at(0).at(0));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER3, listOfCircles.at(0).at(1));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER3, listOfCircles.at(1).at(0));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER3, listOfCircles.at(1).at(1));

	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(0).at(4));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(0).at(5));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER2, listOfCircles.at(0).at(6));

	//Gr�ne Start
	ColorizeCircle(PLAYGROUNDCOLORPLAYER2, listOfCircles.at(0).at(9));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER2, listOfCircles.at(0).at(10));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER2, listOfCircles.at(1).at(9));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER2, listOfCircles.at(1).at(10));

	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(1).at(4));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER2, listOfCircles.at(1).at(5));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(1).at(6));

	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(2).at(4));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER2, listOfCircles.at(2).at(5));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(2).at(6));

	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(3).at(4));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER2, listOfCircles.at(3).at(5));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(3).at(6));

	ColorizeCircle(PLAYGROUNDCOLORPLAYER3, listOfCircles.at(4).at(0));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(4).at(1));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(4).at(2));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(4).at(3));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(4).at(4));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER2, listOfCircles.at(4).at(5));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(4).at(6));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(4).at(7));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(4).at(8));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(4).at(9));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(4).at(10));

	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(5).at(0));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER3, listOfCircles.at(5).at(1));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER3, listOfCircles.at(5).at(2));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER3, listOfCircles.at(5).at(3));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER3, listOfCircles.at(5).at(4));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER4, listOfCircles.at(5).at(6));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER4, listOfCircles.at(5).at(7));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER4, listOfCircles.at(5).at(8));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER4, listOfCircles.at(5).at(9));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(5).at(10));

	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(6).at(0));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(6).at(1));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(6).at(2));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(6).at(3));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(6).at(4));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER1, listOfCircles.at(6).at(5));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(6).at(6));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(6).at(7));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(6).at(8));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(6).at(9));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER4, listOfCircles.at(6).at(10));

	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(7).at(4));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER1, listOfCircles.at(7).at(5));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(7).at(6));

	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(8).at(4));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER1, listOfCircles.at(8).at(5));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(8).at(6));

	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(9).at(4));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER1, listOfCircles.at(9).at(5));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(9).at(6));

	ColorizeCircle(PLAYGROUNDCOLORPLAYER1, listOfCircles.at(10).at(4));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(10).at(5));
	ColorizeCircle(PLAYGROUNDCOLORNORMALSHAPE, listOfCircles.at(10).at(6));

	ColorizeCircle(PLAYGROUNDCOLORPLAYER1, listOfCircles.at(9).at(0));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER1, listOfCircles.at(9).at(1));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER1, listOfCircles.at(10).at(0));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER1, listOfCircles.at(10).at(1));

	ColorizeCircle(PLAYGROUNDCOLORPLAYER4, listOfCircles.at(9).at(9));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER4, listOfCircles.at(9).at(10));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER4, listOfCircles.at(10).at(9));
	ColorizeCircle(PLAYGROUNDCOLORPLAYER4, listOfCircles.at(10).at(10));

	return listOfCircles;
}

std::vector<sf::RectangleShape> InitializeListOfLines()
{
	std::vector<sf::RectangleShape> listOfLines;
	listOfLines.push_back(CreateRectangleLine(true, 4, 0, 2));
	listOfLines.push_back(CreateRectangleLine(true, 0, 4, 4));
	listOfLines.push_back(CreateRectangleLine(true, 6, 4, 4));
	listOfLines.push_back(CreateRectangleLine(true, 0, 6, 4));
	listOfLines.push_back(CreateRectangleLine(true, 6, 6, 4));
	listOfLines.push_back(CreateRectangleLine(true, 4, 10, 2));

	listOfLines.push_back(CreateRectangleLine(false, 0, 4, 2));
	listOfLines.push_back(CreateRectangleLine(false, 4, 0, 4));
	listOfLines.push_back(CreateRectangleLine(false, 4, 6, 4));
	listOfLines.push_back(CreateRectangleLine(false, 6, 0, 4));
	listOfLines.push_back(CreateRectangleLine(false, 6, 6, 4));
	listOfLines.push_back(CreateRectangleLine(false, 10, 4, 2));

	//Striche hinter den H�usern
	//listOfLines.push_back(CreateRectangleLine(true, 0, 5, 4));
	//listOfLines.push_back(CreateRectangleLine(true, 6, 5, 4));
	//listOfLines.push_back(CreateRectangleLine(false, 5, 0, 4));
	//listOfLines.push_back(CreateRectangleLine(false, 5, 6, 4));
	return listOfLines;
}

std::vector<sf::RectangleShape> InitializeShowsWhichPlayer()
{
	std::vector<sf::RectangleShape> list;
	list.push_back(CreateShowsWhichPlayer(9, 0));
	list.push_back(CreateShowsWhichPlayer(9, 9));
	list.push_back(CreateShowsWhichPlayer(0, 9));
	list.push_back(CreateShowsWhichPlayer(0, 0));
	return list;
}

sf::RectangleShape CreateShowsWhichPlayer(int x, int y)
{
	sf::RectangleShape shape(sf::Vector2f((PLAYGROUNDINTERVAL - SHOWSWHICHPLAYEROUTLINETHICKNESS) * 2, (PLAYGROUNDINTERVAL - SHOWSWHICHPLAYEROUTLINETHICKNESS) * 2));
	shape.setFillColor(SHOWSWHICHPLAYERCOLOR);
	shape.setOutlineThickness(SHOWSWHICHPLAYEROUTLINETHICKNESS);
	shape.setOutlineColor(SHOWSWHICHPLAYEROUTLINECOLOR);
	shape.setPosition(x * PLAYGROUNDINTERVAL, y * PLAYGROUNDINTERVAL);
	return shape;
}

sf::RectangleShape CreateRectangleLine(bool horizontal, int startx, int starty, int length)
{
	if (horizontal)
	{
		sf::RectangleShape line(sf::Vector2f(length * PLAYGROUNDINTERVAL, 0));
		line.setOutlineColor(PLAYGROUNDLINEOUTLINECOLOR);
		line.setOutlineThickness(PLAYGROUNDLINEOUTLINETHICKNESS);
		line.setPosition(sf::Vector2f(startx * PLAYGROUNDINTERVAL + PLAYGROUNDINTERVAL / 2, starty * PLAYGROUNDINTERVAL + PLAYGROUNDINTERVAL / 2));
		return line;
	}
	else
	{
		sf::RectangleShape line(sf::Vector2f(0, length * PLAYGROUNDINTERVAL));
		line.setOutlineColor(PLAYGROUNDLINEOUTLINECOLOR);
		line.setOutlineThickness(PLAYGROUNDLINEOUTLINETHICKNESS);
		line.setPosition(sf::Vector2f(startx * PLAYGROUNDINTERVAL + PLAYGROUNDINTERVAL / 2, starty * PLAYGROUNDINTERVAL + PLAYGROUNDINTERVAL / 2));
		return line;
	}
}

Sidebar InitializeSidebar()
{
	Sidebar sidebar;
	sf::Font* Fontii = new sf::Font;
	Fontii->loadFromFile("schrift.ttf");
	sidebar.font = Fontii;
	sidebar.dice = InitializeDice();
	sidebar.rollTheDiceButton = InitRollTheDiceButton(sidebar);
	sidebar.SaveAndCloseButton = InitSaveAndCloseButton(sidebar);
	sidebar.startButton = InitTopButton(0, "Start", sidebar);
	sidebar.passButton = InitTopButton(1, "Weitergeben", sidebar);
	sf::Text* textbox = new sf::Text;
	textbox->setFont(*sidebar.font);
	textbox->setString("Herzlich Willkommen.");
	textbox->setPosition (PLAYGROUNDINTERVAL * 11 + TOPBUTTONMARGIN, 350.f); //TODO
	textbox->setCharacterSize(20);
	textbox->setOutlineColor(sf::Color::Green);
	sidebar.infotext = textbox;
	sidebar.infotext->setFillColor(sf::Color::White);
	sidebar.minigame = InitializeMinigame(sidebar);
	return sidebar;
}

Minigame InitializeMinigame(Sidebar& sidebar)
{
	Minigame minigame;
	minigame.answer = "";
	minigame.taskstring = "";
	sf::RectangleShape background(sf::Vector2f(SIDEPANEL - TOPBUTTONMARGIN * 6, PLAYGROUNDINTERVAL * 1.6));
	background.setFillColor(MINIGAMEBACKGROUNDCOLOR);
	background.setPosition(PLAYGROUNDINTERVAL * 11 + TOPBUTTONMARGIN * 3, PLAYGROUNDINTERVAL * 7);
	minigame.background = background;
	sf::Text* textbox = new sf::Text;
	textbox->setFont(*sidebar.font);
	textbox->setString("Minigame: Kleine Rechenaufgabe");
	textbox->setPosition(PLAYGROUNDINTERVAL * 11 + TOPBUTTONMARGIN * 6, PLAYGROUNDINTERVAL * 7 + TOPBUTTONMARGIN);
	textbox->setCharacterSize(TOPBUTTONTEXTSIZE);
	textbox->setFillColor(MINIGAMETEXTCOLOR);
	minigame.text = textbox;
	sf::Text* task = new sf::Text;
	task->setFont(*sidebar.font);
	task->setString(minigame.taskstring + minigame.answer);
	task->setPosition(PLAYGROUNDINTERVAL * 11 + TOPBUTTONMARGIN * 6, PLAYGROUNDINTERVAL * 7 + TOPBUTTONMARGIN + 2 * TOPBUTTONTEXTSIZE);
	task->setCharacterSize(TOPBUTTONTEXTSIZE);
	task->setFillColor(MINIGAMETEXTCOLOR);
	minigame.task = task;
	sf::RectangleShape newTaskButton(sf::Vector2f(SIDEPANEL - TOPBUTTONMARGIN * 12, ROLLTHEDICEBUTTONHEIGHT));
	newTaskButton.setFillColor(TOPBUTTONCOLOR);
	newTaskButton.setPosition(PLAYGROUNDINTERVAL * 11 + TOPBUTTONMARGIN * 6, PLAYGROUNDINTERVAL * 7 + 4 * TOPBUTTONTEXTSIZE);
	minigame.newTaskButton = newTaskButton;
	sf::Text* buttonText = new sf::Text;
	buttonText->setFont(*sidebar.font);
	buttonText->setString("Neue Aufgabe");
	buttonText->setPosition(PLAYGROUNDINTERVAL * 11 + TOPBUTTONMARGIN * 6 + MINIGAMEBUTTONLEFTPADDING, PLAYGROUNDINTERVAL * 7 + 4 * TOPBUTTONTEXTSIZE + ((ROLLTHEDICEBUTTONHEIGHT - TOPBUTTONTEXTSIZE) / 2.5));
	buttonText->setCharacterSize(TOPBUTTONTEXTSIZE);
	buttonText->setFillColor(MINIGAMETEXTCOLOR);
	minigame.newTaskButtonText = buttonText;
	return minigame;
}

ViewPtr InitializeView(ClientMadnPtr client_ptr)
{
	ViewPtr view = std::make_shared<View>();
	view->client = client_ptr;
	//view->client = clientMadn.GetClient();
	sf::RectangleShape backgroundshape(sf::Vector2f(PLAYGROUNDINTERVAL * 11 + SIDEPANEL, PLAYGROUNDINTERVAL * 11));
	backgroundshape.setFillColor(VIEWBACKGROUNDCOLOR);
	view->background = backgroundshape;
	view->listOfCircles = InitializePlayground();
	view->sidebar = InitializeSidebar();
	view->listOfLines = InitializeListOfLines();
	view->showsWhichPlayer = InitializeShowsWhichPlayer();
	view->Player1 = InitPlayer();
	view->Player2 = InitPlayer();
	view->Player3 = InitPlayer();
	view->Player4 = InitPlayer();
	InitializeAllPlayers(view);
	return view;
}

void InitializeAllPlayers(ViewPtr view)
{
	for (int i = 0; i < 4; ++i)
	{
		view->Player1.Meeples.at(i).meeple.setFillColor(MEEPLECOLOR1);
		view->Player1.Meeples.at(i).meeple.setOutlineColor(MEEPLEOUTLINECOLOR1);
		view->Player2.Meeples.at(i).meeple.setFillColor(MEEPLECOLOR2);
		view->Player2.Meeples.at(i).meeple.setOutlineColor(MEEPLEOUTLINECOLOR2);
		view->Player3.Meeples.at(i).meeple.setFillColor(MEEPLECOLOR3);
		view->Player3.Meeples.at(i).meeple.setOutlineColor(MEEPLEOUTLINECOLOR3);
		view->Player4.Meeples.at(i).meeple.setFillColor(MEEPLECOLOR4);
		view->Player4.Meeples.at(i).meeple.setOutlineColor(MEEPLEOUTLINECOLOR4);
	}
}

Dice InitializeDice()
{
	Dice dice;
	std::vector<sf::CircleShape> listOfDiceDots;
	for (int j = 0; j < 3; j = j + 2)
	{
		for (int i = 0; i < 3; ++i)
			listOfDiceDots.push_back(CreateDot(j, i, DICECOLOR));
	}
	listOfDiceDots.push_back(CreateDot(1, 1, DICECOLOR));
	dice.listOfDiceDots = listOfDiceDots;
	dice.diceShape = CreateDiceShape(PLAYGROUNDINTERVAL * 11 + (SIDEPANEL - DICESIZE) / 2, DICEYPLACE, DICECOLOR);

	return dice;
}

void Dice::Set(int number)
{
	Clear();
	if (number >= 4)
	{
		listOfDiceDots.at(0).setFillColor(DICEDOTCOLOR);
		listOfDiceDots.at(5).setFillColor(DICEDOTCOLOR);
	}
	if (number == 6)
	{
		listOfDiceDots.at(1).setFillColor(DICEDOTCOLOR);
		listOfDiceDots.at(4).setFillColor(DICEDOTCOLOR);
	}
	if (number >= 2)
	{
		listOfDiceDots.at(2).setFillColor(DICEDOTCOLOR);
		listOfDiceDots.at(3).setFillColor(DICEDOTCOLOR);
	}
	if (number%2 == 1) 
	{
		listOfDiceDots.at(6).setFillColor(DICEDOTCOLOR);
	}

}

void Dice::Clear()
{
	for (int i = 0; i < listOfDiceDots.size(); ++i)
	{
		listOfDiceDots.at(i).setFillColor(sf::Color::Transparent);
	}
}

Player InitPlayer()
{
	Player player;
	std::vector<Meeple> meeples;
	for (int i = 0; i < 4; ++i)
	{
		meeples.push_back(InitMeeple());
	}
	player.Meeples = meeples;
	return player;
}

Meeple InitMeeple()
{
	Meeple meeple;
	meeple.meeple = sf::CircleShape(MEEPLERADIUS);
	meeple.meeple.setOutlineThickness(MEEPLEOUTLINETHICKNESS);
	return meeple;
}

sf::RectangleShape InitRollTheDiceButton(Sidebar& sidebar)
{
	sf::RectangleShape rollTheDiceButton(sf::Vector2f(ROLLTHEDICEBUTTONWIDTH, ROLLTHEDICEBUTTONHEIGHT));
	rollTheDiceButton.setFillColor(ROLLTHEDICEBUTTONCOLOR);
	rollTheDiceButton.setPosition(PLAYGROUNDINTERVAL * 11 + (SIDEPANEL - ROLLTHEDICEBUTTONWIDTH) / 2, DICEYPLACE + DICESIZE + ROLLTHEDICEBUTTONMARGINTOP);
	sf::Text* textbox = new sf::Text;
	textbox->setFont(*sidebar.font);
	textbox->setString("Wuerfeln");
	textbox->setPosition(PLAYGROUNDINTERVAL * 11 + (SIDEPANEL - ROLLTHEDICEBUTTONWIDTH) / 2 + ROLLTHEDICEBUTTONLEFTPADDING, DICEYPLACE + DICESIZE + ROLLTHEDICEBUTTONMARGINTOP + ((ROLLTHEDICEBUTTONHEIGHT - TOPBUTTONTEXTSIZE) / 2.5));
	textbox->setCharacterSize(TOPBUTTONTEXTSIZE);
	textbox->setFillColor(TOPBUTTONTEXTCOLOR);
	sidebar.rollTheDiceButtonText = textbox;
	return rollTheDiceButton;
}

sf::RectangleShape InitSaveAndCloseButton(Sidebar& sidebar)
{
	sf::RectangleShape SaveAndCloseButton(sf::Vector2f(ROLLTHEDICEBUTTONWIDTH, ROLLTHEDICEBUTTONHEIGHT));
	SaveAndCloseButton.setFillColor(ROLLTHEDICEBUTTONCOLOR);
	SaveAndCloseButton.setPosition(PLAYGROUNDINTERVAL * 11 + (SIDEPANEL - ROLLTHEDICEBUTTONWIDTH) / 2, PLAYGROUNDINTERVAL * 11 - ROLLTHEDICEBUTTONMARGINTOP / 2 - ROLLTHEDICEBUTTONHEIGHT);
	sf::Text* textbox = new sf::Text;
	textbox->setFont(*sidebar.font);
	textbox->setString("Speicheranfrage senden");
	textbox->setPosition(PLAYGROUNDINTERVAL * 11 + (SIDEPANEL - ROLLTHEDICEBUTTONWIDTH) / 2 + SAVEBUTTONLEFTPADDING, PLAYGROUNDINTERVAL * 11 - ROLLTHEDICEBUTTONMARGINTOP / 2 - ROLLTHEDICEBUTTONHEIGHT + ((ROLLTHEDICEBUTTONHEIGHT - TOPBUTTONTEXTSIZE) / 2.5));
	textbox->setCharacterSize(TOPBUTTONTEXTSIZE);
	textbox->setFillColor(TOPBUTTONTEXTCOLOR);
	sidebar.SaveAndCloseButtonText = textbox;
	return SaveAndCloseButton;
}

sf::RectangleShape InitTopButton(int x, std::string text, Sidebar& sidebar)
{
	sf::RectangleShape button(sf::Vector2f(SIDEPANEL / 2 - (2 * TOPBUTTONMARGIN), TOPBUTTONHEIGHT));
	button.setFillColor(TOPBUTTONCOLOR);
	button.setPosition(PLAYGROUNDINTERVAL * 11 + TOPBUTTONMARGIN + (x * SIDEPANEL / 2), TOPBUTTONMARGIN);
	
	sf::Text* textbox = new sf::Text;
	textbox->setFont(*sidebar.font);
	textbox->setString(text);
	textbox->setCharacterSize(TOPBUTTONTEXTSIZE);
	textbox->setFillColor(TOPBUTTONTEXTCOLOR);
	if (x == 1)
	{
		textbox->setPosition(PLAYGROUNDINTERVAL * 11 + TOPBUTTONMARGIN + (x * SIDEPANEL / 2) + TOPBUTTONLEFTLEFTPADDING, TOPBUTTONMARGIN + ((TOPBUTTONHEIGHT - TOPBUTTONTEXTSIZE) / 2.5));
		sidebar.startButtonText = textbox;
	}
	else {
		textbox->setPosition(PLAYGROUNDINTERVAL * 11 + TOPBUTTONMARGIN + (x * SIDEPANEL / 2) + TOPBUTTONRIGHTLEFTPADDING, TOPBUTTONMARGIN + ((TOPBUTTONHEIGHT - TOPBUTTONTEXTSIZE) / 2.5));
		sidebar.passButtonText = textbox;
	}
	return button;
}

void View::setPositions(std::string Coords)
{
	int player1set = 0;
	int player2set = 0;
	int player3set = 0;
	int player4set = 0;

	int player1numberForServer = 0;
	int player2numberForServer = 0;
	int player3numberForServer = 0;
	int player4numberForServer = 0;
	switch (Coords.at(0))
	{
	case '4':
		Player1.Meeples.at(player1set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * 9 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player1.Meeples.at(player1set).numberForServer = 0;
		++player1set;
	case '3':
		Player1.Meeples.at(player1set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * 10 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player1.Meeples.at(player1set).numberForServer = 0;
		++player1set;
	case '2':
		Player1.Meeples.at(player1set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * 9 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player1.Meeples.at(player1set).numberForServer = 0;
		++player1set;
	case '1':
		Player1.Meeples.at(player1set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * 10 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player1.Meeples.at(player1set).numberForServer = 0;
		++player1set;
		break;
	}
	switch (Coords.at(5))
	{
	case '4':
		Player2.Meeples.at(player2set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * 9 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * 9 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player2.Meeples.at(player2set).numberForServer = 0;
		++player2set;
	case '3':
		Player2.Meeples.at(player2set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * 10 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * 9 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player2.Meeples.at(player2set).numberForServer = 0;
		++player2set;
	case '2':
		Player2.Meeples.at(player2set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * 9 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * 10 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player2.Meeples.at(player2set).numberForServer = 0;
		++player2set;
	case '1':
		Player2.Meeples.at(player2set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * 10 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * 10 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player2.Meeples.at(player2set).numberForServer = 0;
		++player2set;
		break;
	}
	switch (Coords.at(10))
	{
	case '4':
		Player3.Meeples.at(player3set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * 9 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player3.Meeples.at(player3set).numberForServer = 0;
		++player3set;
	case '3':
		Player3.Meeples.at(player3set).meeple.setPosition(sf::Vector2f((PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * 9 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player3.Meeples.at(player3set).numberForServer = 0;
		++player3set;
	case '2':
		Player3.Meeples.at(player3set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * 10 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player3.Meeples.at(player3set).numberForServer = 0;
		++player3set;
	case '1':
		Player3.Meeples.at(player3set).meeple.setPosition(sf::Vector2f((PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * 10 + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player3.Meeples.at(player3set).numberForServer = 0;
		++player3set;
		break;
	}
	switch (Coords.at(15))
	{
	case '4':
		Player4.Meeples.at(player4set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player4.Meeples.at(player4set).numberForServer = 0;
		++player4set;
	case '3':
		Player4.Meeples.at(player4set).meeple.setPosition(sf::Vector2f((PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player4.Meeples.at(player4set).numberForServer = 0;
		++player4set;
	case '2':
		Player4.Meeples.at(player4set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player4.Meeples.at(player4set).numberForServer = 0;
		++player4set;
	case '1':
		Player4.Meeples.at(player4set).meeple.setPosition(sf::Vector2f((PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player4.Meeples.at(player4set).numberForServer = 0;
		++player4set;
		break;
	}

	player1numberForServer = 1;
  	player2numberForServer = 1;
  	player3numberForServer = 1;
  	player4numberForServer = 1;

	setPositionOfMeeple(6, 0, Coords.at(20), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(6, 1, Coords.at(21), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(6, 2, Coords.at(22), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(6, 3, Coords.at(23), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(6, 4, Coords.at(24), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(7, 4, Coords.at(25), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(8, 4, Coords.at(26), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(9, 4, Coords.at(27), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(10, 4, Coords.at(28), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(10, 5, Coords.at(29), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(10, 6, Coords.at(30), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(9, 6, Coords.at(31), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(8, 6, Coords.at(32), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(7, 6, Coords.at(33), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(6, 6, Coords.at(34), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(6, 7, Coords.at(35), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(6, 8, Coords.at(36), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(6, 9, Coords.at(37), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(6, 10, Coords.at(38), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(5, 10, Coords.at(39), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(4, 10, Coords.at(40), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(4, 9, Coords.at(41), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(4, 8, Coords.at(42), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(4, 7, Coords.at(43), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(4, 6, Coords.at(44), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(3, 6, Coords.at(45), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(2, 6, Coords.at(46), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(1, 6, Coords.at(47), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(0, 6, Coords.at(48), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(0, 5, Coords.at(49), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(0, 4, Coords.at(50), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(1, 4, Coords.at(51), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(2, 4, Coords.at(52), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(3, 4, Coords.at(53), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(4, 4, Coords.at(54), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(4, 3, Coords.at(55), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(4, 2, Coords.at(56), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(4, 1, Coords.at(57), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(4, 0, Coords.at(58), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	setPositionOfMeeple(5, 0, Coords.at(59), player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);

	if (Coords.at(11) == '1')
		setPositionOfMeeple(5, 9, '3', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(12) == '1')
		setPositionOfMeeple(5, 8, '3', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(13) == '1')
		setPositionOfMeeple(5, 7, '3', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(14) == '1')
		setPositionOfMeeple(5, 6, '3', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);

	if (Coords.at(1) == '1')
		setPositionOfMeeple(5, 1, '1', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(2) == '1')
		setPositionOfMeeple(5, 2, '1', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(3) == '1')
		setPositionOfMeeple(5, 3, '1', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(4) == '1')
		setPositionOfMeeple(5, 4, '1', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);

	if (Coords.at(16) == '1')
		setPositionOfMeeple(1, 5, '4', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(17) == '1')
		setPositionOfMeeple(2, 5, '4', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(18) == '1')
		setPositionOfMeeple(3, 5, '4', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(19) == '1')
		setPositionOfMeeple(4, 5, '4', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);

	if (Coords.at(6) == '1')
		setPositionOfMeeple(9, 5, '2', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(7) == '1')
		setPositionOfMeeple(8, 5, '2', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(8) == '1')
		setPositionOfMeeple(7, 5, '2', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);
	if (Coords.at(9) == '1')
		setPositionOfMeeple(6, 5, '2', player1set, player2set, player3set, player4set, player1numberForServer, player2numberForServer, player3numberForServer, player4numberForServer);

	char dicenumber = Coords.at(61);
	sidebar.dice.Set(atoi(&dicenumber));

	char activePlayer = Coords.at(60);
	if (std::atoi(&activePlayer) == client->playerNumber)
	{
		SetErrorMessage("Du bist an der Reihe.");
	}

	char won = Coords.at(62);
	if (std::atoi(&won) > 0)
	{
		if (std::atoi(&won) == client->playerNumber)
		{
			SetErrorMessage("Herzlichen Glueckwunsch!\nDu hast gewonnen.");
		}
		else
		{
			std::string colour = "";
			switch (std::atoi(&won))
			{
			case 1:
				colour = "gelb";
				break;
			case 2:
				colour = "blau";
				break;
			case 3:
				colour = "rot";
				break;
			case 4:
				colour = "gruen";
				break;
			default:
				colour = "unsichtbar";
				break;
			}
			std::string message("Spieler " + won);
			SetErrorMessage(message + " (" + colour + ") hat gewonnen!");
			//?client->aktiv = false;
		}
	}
}

void View::setPositionOfMeeple(int x, int y, int playerNr, int& player1set, int& player2set, int& player3set, int& player4set,int& player1numberForServer,int& player2numberForServer,int& player3numberForServer,int& player4numberForServer)
{
	switch (playerNr)
	{
	case '1':
		Player1.Meeples.at(player1set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * x + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * y + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player1.Meeples.at(player1set).numberForServer = player1numberForServer;
		++player1numberForServer;
		++player1set;
		break;
	case '2':
		Player2.Meeples.at(player2set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * x + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * y + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player2.Meeples.at(player2set).numberForServer = player2numberForServer;
		++player2numberForServer;
		++player2set;
		break;
	case '3':
		Player3.Meeples.at(player3set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * x + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * y + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player3.Meeples.at(player3set).numberForServer = player3numberForServer;
		++player3numberForServer;
		++player3set;
		break;
	case '4':
		Player4.Meeples.at(player4set).meeple.setPosition(sf::Vector2f(PLAYGROUNDINTERVAL * x + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2, PLAYGROUNDINTERVAL * y + (PLAYGROUNDINTERVAL - MEEPLERADIUS * 2) / 2));
		Player4.Meeples.at(player4set).numberForServer = player4numberForServer;
		++player4numberForServer;
		++player4set;
		break;
	}
}

void View::CommunicateWithClient(std::string message)
{
	if (client->statusRunningHandler == 0)
	{
		std::string completemessage = std::to_string(client->playerNumber) + message;
		client->message = completemessage;
		client->statusRunningHandler = 1;
	}
	else
	{
		switch (client->statusRunningHandler)
		{
		case 1:
			SetErrorMessage("Warte, bis die vorherige Nachricht versendet wurde.");
			break;
		case 2:
			SetErrorMessage("Nur ein Zug auf einmal.");
			break;
		case 4:
			SetErrorMessage("Du konntest noch keinem Spieler\n zugeordnet werden.");
			break;
		default:
			SetErrorMessage("Unerwarteter Fehler.");
			break;
		}
	}
}

void View::CheckForAnswer()
{
	if (client->statusRunningHandler == 2)
	{
		std::string answer = client->answer;
		if (answer.size() > 60)
		{
			setPositions(answer);
		}
		else //TODO: andere Fehlermeldungen schön Ausgeben
		{
			SetErrorMessage(answer);
		}
		client->statusRunningHandler = 0;
	}
}

void View::SetErrorMessage(std::string message)
{
	sidebar.infotext->setString(message);
}

void Minigame::NewTask()
{
	srand((time(NULL) % 3 + 46846165165) * 7 + 5544564);
	for (int i = 0; i < 15; ++i)
		rand();
	int first = rand() % 8000;
	int second = rand() % 5000;
	if (first > second && rand() % 3 != 0)
	{
		result = first - second;
		taskstring = std::to_string(first) + " - " + std::to_string(second) + " = ";
	}
	else
	{
		result = first + second;
		taskstring = std::to_string(first) + " + " + std::to_string(second) + " = ";
	}
	task->setString(taskstring);
	task->setFillColor(MINIGAMETEXTCOLOR);
}
