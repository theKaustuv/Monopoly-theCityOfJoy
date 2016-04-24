#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "BaseFunctions.h"
#include "Property.h"
#include "Player.h"
#include "RealEstate.h"

using namespace std;

void turn(Player** players, Property** board, int activePlayer){

}

int changeActivePlayer(Player** players, int activePlayer,int numberOfPlayers){
	if (activePlayer == (numberOfPlayers-1) )
		activePlayer=0;
	else
		activePlayer+=1;;

	if ( players[activePlayer]->getIsBankrupt() == true )
		return changeActivePlayer(players,activePlayer,numberOfPlayers);
	else
		return activePlayer;
}

int rollDice(int numberOfDice){

    return randomNum(1,6,numberOfDice);
}

int chooseStartingPlayer(int numberOfPlayers){
	return randomNum(0, (numberOfPlayers-1) ,1); // activePlayer takes 0 to numberOfPlayers - 1 value only
}

int randomNum(int lowerLim,int upperLim, int numberOfRandomNumbersToBeSummedAndReturned){

    int result=0;
    srand(time(0));
    for (int i=0;i<numberOfRandomNumbersToBeSummedAndReturned;i++)
        result+=(rand()%(upperLim-lowerLim+1) + lowerLim);
    return result;
}


