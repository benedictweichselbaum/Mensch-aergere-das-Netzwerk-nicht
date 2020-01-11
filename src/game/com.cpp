#include "Com.hpp"

void Com(int* game, int comnumber)
{
	int rolledNumber = RollTheDice();
	int playerNumber = game[60];
	int inStartHouse = HowManyMeeplesInStartHouse(game, playerNumber);
	// Alle 4 Im Haus -> 3* Würfeln und ziehen
	if (inStartHouse == 4)
	{
		int threetimes = 3;
		while (threetimes > 1 && rolledNumber != 6)
		{
			--threetimes;
			rolledNumber = RollTheDice();
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
		}
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
		Com(game, comnumber);
}

void Com1(int* game, int rolledNumber, int playerNumber)
{
	for (int i = 1; i <= 4; ++i)
	{
		int p = ProgressOfMeeple(game, playerNumber, i);
		if (IsMovePossible(game, playerNumber, p, rolledNumber))
		{
			Move(game, playerNumber, p, rolledNumber);
			return;
		}
	}
}


int HowManyMeeplesInStartHouse(int* game, int playerNumber)
{
	return game[(playerNumber - 1) * 5];
}

int RollTheDice()
{
	std::mt19937 rng(time(NULL));
	std::uniform_int_distribution<int8_t> gen(1, 6);
	return gen(rng);
}

int GetIndexOfStartPoint(int playerNumber)
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

void MeepleKickedOut(int* game, int player)
{
	++game[(player - 1) * 5];
}

void HasToPlattenPutzen(int* game, int playerNumber, int rolledNumber)
{
	if (game[AddDiceAmountToPosition(GetIndexOfStartPoint(playerNumber), rolledNumber)] == playerNumber)
		return;
	else if (game[AddDiceAmountToPosition(GetIndexOfStartPoint(playerNumber), rolledNumber)] > 0)
		MeepleKickedOut(game, game[AddDiceAmountToPosition(GetIndexOfStartPoint(playerNumber), rolledNumber)]);
	game[AddDiceAmountToPosition(GetIndexOfStartPoint(playerNumber), rolledNumber)] = playerNumber;
}

int AddDiceAmountToPosition(int position, int dice)
{
	if (position + dice < 60)
		return position + dice;
	else
		return (position + dice - 40);
}

int ProgressOfMeeple(int* game, int playerNumber, int meepleNumber)
{
	int currentMeeple = 1;
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
	std::cout << "Das haette nicht passieren sollen.";
	return 99;
}

bool IsMovePossible(int* game, int playerNumber, int progressOfMeeple, int rolledNumber)
{
	if (progressOfMeeple + rolledNumber > 44)
		return false;
	if (GetIndexWithProgress(playerNumber, progressOfMeeple + rolledNumber) == playerNumber)
		return false;

}

int GetIndexWithProgress(int playerNumber, int progressOfMeeple)
{
	int it = GetIndexOfStartPoint(playerNumber);
	for (int i = 1; i <= 40; ++i)
	{
		if (i == progressOfMeeple)
		{
			return it;
		}
		++it;
		if (it >= 60)
			it = 20;
	}
	for (int i = 1; i <= 4; ++i)
	{
		if (40 + i == progressOfMeeple)
			return (playerNumber - 1) * 5 + i;
	}
}

void Move(int* game, int playerNumber, int progressOfMeeple, int rolledNumber)
{
	game[GetIndexWithProgress(playerNumber, progressOfMeeple)] = 0;
	game[GetIndexWithProgress(playerNumber, progressOfMeeple + rolledNumber)] = playerNumber;
}
