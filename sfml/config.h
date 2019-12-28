#pragma once

//Größe der Kreise auf dem Spielfeld INTERVAL ist der Abstand der Kreismittelpunkte (Pixel)
#define PLAYGROUNDCIRCLESRADIUS	30.f
#define PLAYGROUNDINTERVAL 90.f
#define VIEWBACKGROUNDCOLOR sf::Color::Color(100,100,100,200)

//Breite des Seitenpanels (Pixel)
#define SIDEPANEL 400.f

//Farbe der farbigen Kreise auf dem Spielfeld
#define PLAYGROUNDCOLORPLAYER1 sf::Color::Color(249,255,189,255) //Gelb
#define PLAYGROUNDCOLORPLAYER2 sf::Color::Color(126,142,231,255) // Blau
#define PLAYGROUNDCOLORPLAYER3 sf::Color::Color(255,122,122,255) //Rot
#define PLAYGROUNDCOLORPLAYER4 sf::Color::Color(137,255,160,255) //Grün

#define PLAYGROUNDCOLORNORMALSHAPE sf::Color::White
#define PLAYGROUNDCOLOROUTLINE sf::Color::Black
#define PLAYGROUNDTHICKNESSOUTLINE 5.f

//Farbe der Spielfiguren
#define MEEPLECOLOR1 sf::Color::Color(255, 244, 31, 255) //(243,255,107,255)
#define MEEPLECOLOR2 sf::Color::Color(61,90,255,255)
#define MEEPLECOLOR3 sf::Color::Color(255,33,26,255)
#define MEEPLECOLOR4 sf::Color::Color(32,173,0,255)
#define MEEPLEOUTLINECOLOR1 sf::Color::Color(255,218,31,255)  //(234,255,5,255)
#define MEEPLEOUTLINECOLOR2 sf::Color::Color(0,37,250,255)
#define MEEPLEOUTLINECOLOR3 sf::Color::Color(209,7,0,255)
#define MEEPLEOUTLINECOLOR4 sf::Color::Color(23,128,0,255)

#define DICECOLOR sf::Color::Yellow
#define DICEDOTCOLOR sf::Color::Black

#define DICESIZE 100.f
#define DICEYPLACE 100.f //Mir ist grad kein besserer Name eingefallen (Abstand des Würfels von der oberen Fensterkante)

#define DICEDOTRADIUS 5.f
#define DICEPADDING 10.f // Abstand von Würfelrand zu Punkten

#define MEEPLERADIUS 12.f
#define MEEPLEOUTLINETHICKNESS 7.f