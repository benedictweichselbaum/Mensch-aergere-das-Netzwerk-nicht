#pragma once

//void DrawPlaygroundMAIN(sf::RenderWindow& window, std::vector<std::vector<sf::CircleShape>>& listOfCircles);

std::vector<std::vector<sf::CircleShape>> FeldErstellen();

std::vector<std::vector<sf::CircleShape>> FeldInitialisieren();

void FeldZeichnen(std::vector<std::vector<sf::CircleShape>>& listOfCircles);

void KreisFärben(sf::Color farbe, sf::CircleShape& circle);

sf::CircleShape ErstelleKreis(float x, float y, sf::Color farbe);
