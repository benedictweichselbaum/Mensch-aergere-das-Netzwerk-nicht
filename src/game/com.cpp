#include "com.hpp"

void Com(int8_t* game, int8_t comnumber)
{
	if (game[61] < 7)
	{
		game[61] = game[61] + 67 + (game[60] * game[60] * game[60]); //Einfach, damit ein anderer Seed genutzt werden kann -> bei öfteren Würfeln in gleicher Sekunde unterschiedlicher Würfel
	}
	else 
	{
		game[61] = game[61] + 23 + (game[60] * game[60] * game[60]);
	}
	int8_t rolledNumber = RollTheDice(game[61]);
	std::cout << std::endl << "Gewuerfelt:" << (int)rolledNumber << std::endl;
	int8_t playerNumber = game[60];
	int8_t inStartHouse = HowManyMeeplesInStartHouse(game, playerNumber);
	// Alle 4 Im Haus -> 3* Würfeln und ziehen
	if (inStartHouse == 4)
	{
		std::cout << "Im StRTHAUS ALLE SIND" << std::endl;
		int8_t threetimes = 3;
		while (threetimes > 1 && rolledNumber != 6)
		{
			--threetimes;
			game[61] = game[61] % 7 + threetimes * playerNumber;
			rolledNumber = RollTheDice(game[61]);
			std::cout << "Gewürfelt:" << (int)rolledNumber << std::endl;
		}
		if (rolledNumber == 6)
		{
			if (game[GetIndexOfStartPoint(playerNumber)] != 0)
			{
				MeepleKickedOut(game, game[GetIndexOfStartPoint(playerNumber)]);
			}
			game[GetIndexOfStartPoint(playerNumber)] = playerNumber;
			game[(playerNumber - 1) * 5] = 3;
		}
	}
	//1-3 im Starthaus und 6 gewürfelt -> Rauskommen
	else if (inStartHouse > 0 && rolledNumber == 6)
	{
		if (game[GetIndexOfStartPoint(playerNumber)] == playerNumber)
		{
			HasToPlattenPutzen(game, playerNumber, rolledNumber);
		}
		else
		{
			if (game[GetIndexOfStartPoint(playerNumber)] > 0) //Werfen
				MeepleKickedOut(game, game[GetIndexOfStartPoint(playerNumber)]);
			game[GetIndexOfStartPoint(playerNumber)] = playerNumber;
			--game[(playerNumber - 1) * 5];
		}
		std::cout << "Com " << (int)playerNumber << " aus dem Starthaus gekommen." << std::endl;

	}
	// 1-3 im Starthaus und auf Startfeld -> Platte Putzen
	else if (inStartHouse > 0 && game[GetIndexOfStartPoint(playerNumber)] == playerNumber)
	{
		HasToPlattenPutzen(game, playerNumber, rolledNumber);
	}
	else
	{
		switch (comnumber)
		{
		case 1:
			Com1(game, rolledNumber, playerNumber);
			break;
		default:
			Com1(game, rolledNumber, playerNumber);
			break;
		}

	}

	if (rolledNumber == 6)
	{
		Com(game, comnumber);
	}
	else
	{
		game[61] = rolledNumber;
	}
}

void Com1(int8_t* game, int8_t rolledNumber, int8_t playerNumber)
{
	for (int8_t i = 4; i >= 1; --i)
	{
		int8_t p = ProgressOfMeeple(game, playerNumber, i);
		if (IsMovePossible(game, playerNumber, p, rolledNumber))
		{
			std::cout << "Ziehe:" << std::endl;
			Move(game, playerNumber, p, rolledNumber);
			std::cout << "Com " << (int)playerNumber << " mit Figur " << (int)i << " " << (int)rolledNumber << " Felder gezogen." << std::endl;
			return;
		}
	}
	std::cout << "Kein Zug möglich" << std::endl;
}


int8_t HowManyMeeplesInStartHouse(int8_t* game, int8_t playerNumber)
{
	return game[(playerNumber - 1) * 5];
}

int8_t RollTheDice(int8_t seedhelper)
{
	/*std::mt19937 rng(time(NULL));
	std::uniform_int_distribution<int8_t> gen(1, 6);
	return gen(rng);*/
	srand(time(NULL) + seedhelper);
	//return (rand() % dicemod * (rand() / 11) + diceadd + 37) % 6 + 1; //Wenn in einer Sekunde öfter gewürfelt wurde, kam sonst die gleiche Würfelzahl raus
	return rand() % 6 + 1;
}

int8_t GetIndexOfStartPoint(int8_t playerNumber)
{
	switch (playerNumber)
	{
	case 1:
		return 40;
	case 2:
		return 20;
	case 3:
		return 50;
	case 4:
		return 30;
	}
}

void MeepleKickedOut(int8_t* game, int8_t player)
{
	++game[(player - 1) * 5];
}

void HasToPlattenPutzen(int8_t* game, int8_t playerNumber, int8_t rolledNumber)
{
	if (game[AddDiceAmountToPosition(GetIndexOfStartPoint(playerNumber), rolledNumber)] == playerNumber)
	{
		std::cout << "Com " << (int)playerNumber << " konnte nicht vom Startfeld ziehen. Wuerfelzahl: " << (int)rolledNumber << std::endl;
		return;
	}
	else if (game[AddDiceAmountToPosition(GetIndexOfStartPoint(playerNumber), rolledNumber)] > 0)
	{
		std::cout << "Com " << (int)playerNumber << " " << (int)rolledNumber << " Felder vom Startfeld gezogen." << std::endl;
		MeepleKickedOut(game, game[AddDiceAmountToPosition(GetIndexOfStartPoint(playerNumber), rolledNumber)]);
	}
	game[GetIndexOfStartPoint(playerNumber)] = 0;
	game[AddDiceAmountToPosition(GetIndexOfStartPoint(playerNumber), rolledNumber)] = playerNumber;
}

int8_t AddDiceAmountToPosition(int8_t position, int8_t dice)
{
	if (position + dice < 60)
		return position + dice;
	else
		return (position + dice - 40);
}

int8_t ProgressOfMeeple(int8_t* game, int8_t playerNumber, int8_t meepleNumber)
{
	int8_t currentMeeple = 1;
	currentMeeple += HowManyMeeplesInStartHouse(game, playerNumber);

	if (meepleNumber < currentMeeple)
		return 0;

	int it = GetIndexOfStartPoint(playerNumber);
	for (int i = 1; i <= 40; ++i)
	{
		if (game[it] == playerNumber)
		{
			if (meepleNumber == currentMeeple)
			{
				return i;
			}
			else
			{
				++currentMeeple;
			}
		}
		++it;
		if (it >= 60)
			it = 20;
	}
	for (int i = 1; i <= 4; ++i)
	{
		if (game[(playerNumber - 1) * 5 + i] == 1)
		{
			if (meepleNumber == currentMeeple)
			{
				return 40 + i;
			}
			else
			{
				++currentMeeple;
			}
		}
	}
	std::cout << "Das haette nicht passieren sollen. Anscheinend ist eine Figur vom Spielbrett gefallen. Es wurden " << (int)currentMeeple << "Figuren gefunden" << std::endl;
	return 99;
}

bool IsMovePossible(int8_t* game, int8_t playerNumber, int8_t progressOfMeeple, int8_t rolledNumber)
{
	if (progressOfMeeple + rolledNumber > 44)
		return false;
	if (GetIndexWithProgress(playerNumber, progressOfMeeple + rolledNumber) == playerNumber)
		return false;

	return true;
}

int8_t GetIndexWithProgress(int8_t playerNumber, int8_t progressOfMeeple)
{
	int8_t it = GetIndexOfStartPoint(playerNumber);
	for (int8_t i = 1; i <= 40; ++i)
	{
		if (i == progressOfMeeple)
		{
			return it;
		}
		++it;
		if (it >= 60)
			it = 20;
	}
	for (int8_t i = 1; i <= 4; ++i)
	{
		if (40 + i == progressOfMeeple)
			return (playerNumber - 1) * 5 + i;
	}
}

void Move(int8_t* game, int8_t playerNumber, int8_t progressOfMeeple, int8_t rolledNumber)
{
	game[GetIndexWithProgress(playerNumber, progressOfMeeple)] = 0;
	if (game[GetIndexWithProgress(playerNumber, progressOfMeeple + rolledNumber)] != 0)
	{
		MeepleKickedOut(game, game[GetIndexWithProgress(playerNumber, progressOfMeeple + rolledNumber)]);
	}
	if (GetIndexWithProgress(playerNumber, progressOfMeeple + rolledNumber) < 20) //Zieht ins Zielhaus
	{
		game[GetIndexWithProgress(playerNumber, progressOfMeeple + rolledNumber)] = 1;
	}
	else
	{
		game[GetIndexWithProgress(playerNumber, progressOfMeeple + rolledNumber)] = playerNumber;
	}
	
}
