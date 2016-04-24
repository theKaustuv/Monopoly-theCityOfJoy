#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "BaseFunctions.h"
#include "Property.h"
#include "Player.h"
#include "RealEstate.h"

using namespace std;

void turn(Player** players, Property** board,int* activePlayer,const int* numberOfPlayers,bool* forcedExit,int* playersRemaining){
	// local variables to the function: 'turn()'
	int bankruptInflictingPlayer=-1;
	int option=0;
	int playthisTurn=1; //number of turns played. if 0: cant play again, if >3: go to jail (3 doubles in same turn)
	bool assuranceVariable=false;
	// end
	// continually ask the player for what he wants to do
	while(option!=-1 || option!= -999){
		displayTurnOptions(&playthisTurn); //Prints out a list of options that can be availed by the player on turn, then asks him/her to choose one.
		if ( players[*activePlayer]->getMoneyInHand() >= 0)
			bankruptInflictingPlayer = -1;
		cin >> option;
		switch (option)
		{
		case 1:
			throwDice(&players[0], &board[0], activePlayer, numberOfPlayers, &playthisTurn, &bankruptInflictingPlayer);
			break;
		case 2:
			tradeWithOtherPlayers(&players[0], &board[0], activePlayer, numberOfPlayers);
			break;
		case 3:
			manageProperties(&players[0], &board[0], activePlayer, numberOfPlayers);
			break;
		case -1:
			if (playthisTurn!=0){
				cout << "You cannot end your turn yet. Throw the Dice." << endl;
				option=0;
			}
			break;
		case -999:
			assuranceVariable=areYouSure("you want to quit the game?");
			if (assuranceVariable==true)
				*forcedExit = true;
			else
				option=0;
			break;
		default:
			cout << "Please Enter a valid option." << endl;
			break;
		}
	}
	if (option==-1 /*ended his/her turn */ && *forcedExit == false /* not quit the game as a whole */ && players[*activePlayer]->getMoneyInHand() < 0 /* has negative balance */){
		players[*activePlayer]->setIsBankrupt();
		handoverAllProperties(&players[0], &board[0], activePlayer, &bankruptInflictingPlayer);
		(*playersRemaining) -= 1;
	}
		
}

void throwDice(Player** players, Property** board, int* activePlayer, const int* numberOfPlayers, int* playthisTurn, int* bankruptInflictingPlayer){

	/* local variables */
	int diceThrowOutcome;
	bool* doubleDice; *doubleDice = false;
	/* end */
	if (*playthisTurn == 0)
		cout << "You have no more Dice Rolls left for this turn.\nYou can trade, Manage your properties or End your turn" << endl;
	else{
		diceThrowOutcome = rollTwoDice(doubleDice);
		
		if (*doubleDice == true)
			(*playthisTurn) += 1;
		else
			(*playthisTurn) = 0;

		if (*playthisTurn > 3){
			sendPlayerToJail(&players[0], activePlayer);
			(*playthisTurn) = 0;
		}
		else{
			if (players[*activePlayer]->getIsInJail() != -1){
				if (*doubleDice == true){
				//gets outta jail
				
				}
			//todo
			
			}


			//todo

		}


	
	}
}

void tradeWithOtherPlayers(Player** players, Property** board, int* activePlayer, const int* numberOfPlayers){
//todo

}

void manageProperties(Player** players, Property** board, int* activePlayer, const int* numberOfPlayers){
//todo

}

void sendPlayerToJail(Player** players, int* activePlayer){

	players[*activePlayer]->setIsInJail(0);
	players[*activePlayer]->setPlayerPosition(10);

}


void handoverProperty(Player** players, Property** board, int propertyPos, int givingPlayer, int receivingPlayer){
	if (board[propertyPos]->getPropertyOwner() == players[givingPlayer]->getPlayerNum()){

		board[propertyPos]->setPropertyOwner( players[receivingPlayer]->getPlayerNum() );
		players[receivingPlayer]->setNumPropertiesOwned(players[receivingPlayer]->getNumPropertiesOwned() + 1);
		if (board[propertyPos]->getPropertyMortgage() == true)
				players[receivingPlayer]->setNumPropertiesMortgaged(players[receivingPlayer]->getNumPropertiesMortgaged() + 1);
		if (board[propertyPos]->getPropertyType() == "Railroad")
			players[receivingPlayer]->setNumRailroadsOwned(players[receivingPlayer]->getNumRailroadsOwned() + 1);
		else if (board[propertyPos]->getPropertyType() == "Utility")
			players[receivingPlayer]->setNumUtilitiesOwned(players[receivingPlayer]->getNumUtilitiesOwned() + 1);
		
	}
	else
		cout << "This property is not owned by the giving player at all." << endl;
}

void handoverAllProperties(Player** players, Property** board, int* activePlayer, int* bankruptInflictingPlayer){
	
	players[*bankruptInflictingPlayer]->setNumGetOutOfJailFreeCard(players[*bankruptInflictingPlayer]->getNumGetOutOfJailFreeCard()+players[*activePlayer]->getNumGetOutOfJailFreeCard());
	for(int i=0;i<39;i++){
		if (board[i]->getPropertyOwner() == players[*activePlayer]->getPlayerNum()){
			handoverProperty(&players[0], &board[0], i, *activePlayer, *bankruptInflictingPlayer);
		}
	}


}

void displayTurnOptions(int* playthisTurn){

	cout << "\nYour Possible Actions:" << endl;
	if (*playthisTurn != 0)
		cout << "1 - Throw Dice" << endl;
	cout << "2 - Trade\n3 - Manage\n-1 - End Turn\n-999 - Quit Game" << endl;

}

bool areYouSure(string s){
	string match;
	cout << "Are you sure " << s << " (enter 'yes' /anything else)" << endl; 
	cin >> match;
	if (match == "yes")
		return true;
	else
		return false;
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

int rollTwoDice(bool* doubleDice){
	int doubleCheck=0;
	*doubleDice = false;
	int singleResult=0;
    int result=0;
    srand(time(0));
    for (int i=0;i<2;i++){
		singleResult=( ( rand()%6 ) + 1);
		if (doubleCheck == singleResult)
			*doubleDice = true;
		else
			doubleCheck = singleResult;
		result+=singleResult;
	}
    return result;
}

int randomNum(int lowerLim,int upperLim, int numberOfRandomNumbersToBeSummedAndReturned){
	int result=0;
    srand(time(0));
    for (int i=0;i<numberOfRandomNumbersToBeSummedAndReturned;i++)
		result+=(rand()%(upperLim-lowerLim+1) + lowerLim);
    return result;
}


