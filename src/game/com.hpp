#pragma once
#include <iostream>
#include <random>
#include <time.h>

void Com(int8_t* game,int8_t comnumber);

void Com1(int8_t* game,int8_t rolledNumber,int8_t playerNumber);

int8_t HowManyMeeplesInStartHouse(int8_t* game,int8_t playerNumber);

int8_t RollTheDice();

int8_t GetIndexOfStartPoint(int8_t playerNumber);

void MeepleKickedOut(int8_t* game,int8_t player);

void HasToPlattenPutzen(int8_t* game,int8_t playerNumber,int8_t rolledNumber);

int8_t AddDiceAmountToPosition(int8_t position,int8_t dice);

int8_t ProgressOfMeeple(int8_t* game,int8_t playerNumber,int8_t meepleNumber);

bool IsMovePossible(int8_t* game,int8_t playerNumber,int8_t progressOfMeeple,int8_t rolledNumber);

int8_t GetIndexWithProgress(int8_t playerNumber,int8_t progressOfMeeple);

void Move(int8_t* game,int8_t playerNumber,int8_t progressOfMeeple,int8_t rolledNumber);
