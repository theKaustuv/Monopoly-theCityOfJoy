#ifndef BASEFUNCTIONS_H_INCLUDED
#define BASEFUNCTIONS_H_INCLUDED

#include <iostream>
#include <string>
#include "Property.h"
#include "Player.h"
#include "RealEstate.h"

using namespace std;


void turn(Player** players, Property** board,int* activePlayer,const int* numberOfPlayers,bool* forcedExit,int* playersRemaining);

// functions in 'turn()'

void displayTurnOptions(int* playthisTurn);


void sendPlayerToJail(Player** players, int* activePlayer);

void updatePlayerPosition(Player** players, int* activePlayer, int diceThrowOutcome);
//todo
void showTitleDeed(Property** board, int propertyPosition);
//todo
void buyProperty(Player** players, Property** board, int* activePlayer);
//todo
void auctionProperty(Player** players, Property** board, int propertyPosition);
// todo
void throwDice(Player** players, Property** board, int* activePlayer, const int* numberOfPlayers, int* playthisTurn, int* bankruptInflictingPlayer);

int chooseTradingPlayer(Player** players, int activePlayer, const int numberOfPlayers);
void displayTradingProperties(Player** players, Property** board, int activePlayer, int tradingPlayer);
void handoverMoney(Player** players, int givingPlayer, int receivingPlayer, int amount);
void tradeWithOtherPlayers(Player** players, Property** board, int* activePlayer, const int* numberOfPlayers);

void displayManageableProperties(Player** players, Property** board, int activePlayer);
int chooseManagingOptionForGivenProperty(Property** board, int propertyToManage);
void mortgageUnmortgageProperty(Player** players, Property** board, int activePlayer, int propertyToManage, int mortgageUnmortgageOption /* 0 - Mortgage 1- Unmortgage */);
void improveProperty(Player** players, Property** board, int activePlayer, int propertyToManage);
void manageProperties(Player** players, Property** board, int* activePlayer, const int* numberOfPlayers);

void handoverProperty(Player** players, Property** board, int propertyPos, int givingPlayer, int receivingPlayer);
void handoverAllProperties(Player** players, Property** board, int* activePlayer, int* bankruptInflictingPlayer);

//end

int changeActivePlayer(Player** players,int activePlayer,int numberOfPlayers);
int rollDice(int numberOfDice);
int rollTwoDice(bool* doubleDice);
int chooseStartingPlayer(int numberOfPlayers);
bool areYouSure(string s);
int randomNum(int lowerLim,int upperLim, int numberOfNumbers);




#endif // BASEFUNCTIONS_H_INCLUDED
