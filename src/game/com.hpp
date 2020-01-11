#pragma once
#include <iostream>
#include <random>
#include <time.h>

void Com(int* game, int comnumber);

void Com1(int* game, int rolledNumber, int playerNumber);

int HowManyMeeplesInStartHouse(int* game, int playerNumber);

int RollTheDice();

int GetIndexOfStartPoint(int playerNumber);

void MeepleKickedOut(int* game, int player);

void HasToPlattenPutzen(int* game, int playerNumber, int rolledNumber);

int AddDiceAmountToPosition(int position, int dice);

int ProgressOfMeeple(int* game, int playerNumber, int meepleNumber);

bool IsMovePossible(int* game, int playerNumber, int progressOfMeeple, int rolledNumber);

int GetIndexWithProgress(int playerNumber, int progressOfMeeple);

void Move(int* game, int playerNumber, int progressOfMeeple, int rolledNumber);
