#ifndef BASEFUNCTIONS_H_INCLUDED
#define BASEFUNCTIONS_H_INCLUDED

#include <iostream>
#include <string>
#include "Property.h"
#include "Player.h"
#include "RealEstate.h"

using namespace std;


void turn(Player** players, Property** board, int activePlayer);
int changeActivePlayer(Player** players,int activePlayer,int numberOfPlayers);
int rollDice(int numberOfDice);
int chooseStartingPlayer(int numberOfPlayers);
int randomNum(int lowerLim,int upperLim, int numberOfNumbers);




#endif // BASEFUNCTIONS_H_INCLUDED
