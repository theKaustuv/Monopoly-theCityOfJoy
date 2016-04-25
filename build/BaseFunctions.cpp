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
	while(option!=-1 && option!= -999){
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
//local variables
	int tradingPlayer = -1;
	int tradeMoneyGet = -1;
	int tradeMoneyGive = -1;
	int tradeJailFreeCardGet = -1;
	int tradeJailFreeCardGive = -1;
	int tradeProperty = -1;
	int propertyGet[40] = {0};
	int propertyGive[40] = {0};
	bool acceptProposal = false;
	string tempString;
//end
	cout << "Choose Player to trade with:" << endl;
	tradingPlayer = chooseTradingPlayer(&players[0], *activePlayer, *numberOfPlayers);
	displayTradingProperties(&players[0], &board[0], *activePlayer, tradingPlayer);
	cout << "Please Formulate Your Proposal:" << endl;
	// receiving properties
	cout << "Enter Property numbers you want to receive: (Hit -1 to finish)" << endl;
	do
	{
		cin >> tradeProperty;
		if (tradeProperty == -1);
		else if (tradeProperty < 0 || tradeProperty > 39)
			cout << "Please Enter a valid number." << endl;
		else if (board[tradeProperty]->getPropertyOwner() == players[tradingPlayer]->getPlayerNum())
			propertyGet[tradeProperty] = 1;
		else
			cout << "Sorry, the trading player does not posses this property. Please enter the next option." << endl;
	} while (tradeProperty != -1);
	// end
	// receiving money
	cout << "Enter Money you want to receive:" << endl;
	while (tradeMoneyGet == -1)
	{
		cin >> tradeMoneyGet;
		if (tradeMoneyGet >= 0 && tradeMoneyGet <= players[tradingPlayer]->getMoneyInHand());
		else if (tradeMoneyGet > players[tradingPlayer]->getMoneyInHand()){
			cout << "The trading player does not have this amount of money.Enter a valid amount." << endl;
			tradeMoneyGet = -1;		
		}
		else if (tradeMoneyGet < 0){
			cout << "Enter a valid amount." << endl;
			tradeMoneyGet = -1;		
		}
	}
	//end
	// receiving Get Outta Jail Free Card
	cout << "Enter Number of Get Out Of Jail Free Cards you want to receive:" << endl;
	while (tradeJailFreeCardGet == -1)
	{
		cin >> tradeJailFreeCardGet;
		if (tradeJailFreeCardGet >= 0 && tradeJailFreeCardGet <= players[tradingPlayer]->getNumGetOutOfJailFreeCard());
		else if (tradeJailFreeCardGet > players[tradingPlayer]->getNumGetOutOfJailFreeCard()){
			cout << "The trading player does not have this many Get Out Of Jail Free Cards.Enter a valid number." << endl;
			tradeJailFreeCardGet = -1;		
		}
		else if (tradeJailFreeCardGet < 0){
			cout << "Enter a valid number." << endl;
			tradeJailFreeCardGet = -1;		
		}
	}
	//end
	tradeProperty = -1;
	// giving properties
	cout << "Enter Property numbers you want to give: (Hit -1 to finish)" << endl;
	do
	{
		cin >> tradeProperty;
		if (tradeProperty == -1);
		else if (tradeProperty < 0 || tradeProperty > 39)
			cout << "Please Enter a valid number." << endl;
		else if (board[tradeProperty]->getPropertyOwner() == players[*activePlayer]->getPlayerNum())
			propertyGive[tradeProperty] = 1;
		else
			cout << "Sorry, you do not posses this property. Please enter the next option." << endl;
	} while (tradeProperty != -1);
	// end
	// giving money
	cout << "Enter Money you want to give:" << endl;
	while (tradeMoneyGive == -1)
	{
		cin >> tradeMoneyGive;
		if (tradeMoneyGive >= 0 && tradeMoneyGive <= players[*activePlayer]->getMoneyInHand());
		else if (tradeMoneyGive > players[*activePlayer]->getMoneyInHand()){
			cout << "You do not have this amount of money.Enter a valid amount." << endl;
			tradeMoneyGive = -1;		
		}
		else if (tradeMoneyGive < 0){
			cout << "Enter a valid amount." << endl;
			tradeMoneyGive = -1;		
		}
	}
	//end
	// giving Get Outta Jail Free Card
	cout << "Enter Number of Get Out Of Jail Free Cards you want to give:" << endl;
	while (tradeJailFreeCardGive == -1)
	{
		cin >> tradeJailFreeCardGive;
		if (tradeJailFreeCardGive >= 0 && tradeJailFreeCardGive <= players[*activePlayer]->getNumGetOutOfJailFreeCard());
		else if (tradeJailFreeCardGive > players[*activePlayer]->getNumGetOutOfJailFreeCard()){
			cout << "You do not have this many Get Out Of Jail Free Cards.Enter a valid number." << endl;
			tradeJailFreeCardGive = -1;		
		}
		else if (tradeJailFreeCardGive < 0){
			cout << "Enter a valid number." << endl;
			tradeJailFreeCardGive = -1;		
		}
	}
	//end
	// Confirmation
	cout << "Do You " << players[tradingPlayer]->getPlayerName() << " accept the proposal? (y/n)" << endl;
	getline(cin >> ws,tempString);
	if (tempString == "y")
		acceptProposal = areYouSure("");
	// end
	// trading occurrs
	if (acceptProposal == true){
		
		players[*activePlayer]->setNumGetOutOfJailFreeCard(players[*activePlayer]->getNumGetOutOfJailFreeCard() + tradeJailFreeCardGet - tradeJailFreeCardGive);
		players[tradingPlayer]->setNumGetOutOfJailFreeCard(players[tradingPlayer]->getNumGetOutOfJailFreeCard() - tradeJailFreeCardGet + tradeJailFreeCardGive);
		handoverMoney(&players[0], *activePlayer, tradingPlayer, (tradeMoneyGive - tradeMoneyGet) );
		for (int i=0;i<39;i++){
			if (propertyGet[i] == 1)
				handoverProperty(&players[0], &board[0], i, tradingPlayer, *activePlayer);
			else if (propertyGive[i] == 1)
				handoverProperty(&players[0], &board[0], i, *activePlayer, tradingPlayer);
		}
	
	
	}
	// end

}

void manageProperties(Player** players, Property** board, int* activePlayer, const int* numberOfPlayers){
//todo

}

int chooseTradingPlayer(Player** players, int activePlayer, const int numberOfPlayers){
	int tradingPlayer = -1;
	for (int i=0;i<numberOfPlayers;i++){
		if (players[i]->getPlayerNum() != activePlayer && players[i]->getIsBankrupt() == false)
			cout << players[i]->getPlayerNum() << " - " << players[i]->getPlayerName() << endl;
	}
	do{
		cout << "Enter Valid Corresponding Number for the Player to trade with: " << endl;
		cin >> tradingPlayer;
		if (tradingPlayer < 0 || tradingPlayer >= numberOfPlayers)
			tradingPlayer = activePlayer;
	}while(players[tradingPlayer]->getPlayerNum() == activePlayer || players[tradingPlayer]->getIsBankrupt() == true );
	return tradingPlayer;
	
}

void displayTradingProperties(Player** players, Property** board, int activePlayer, int tradingPlayer){
	int i;
	// for trading player
	cout << players[tradingPlayer]->getPlayerName() << "'s Properties:" << endl;
	for (i=0;i<39;i++){
		if (board[i]->getPropertyOwner() == players[tradingPlayer]->getPlayerNum())
			cout << "Number: " << board[i]->getPropertyPosition() << "  Property: " << board[i]->getPropertyName() << "  Type: " << board[i]->getPropertyType() <<"  Value: " << board[i]->getPropertyValue() << "  Mortgaged? " << board[i]->getPropertyMortgage() << endl; 
	
	}
	cout << players[tradingPlayer]->getPlayerName() << "'s Money In Hand: ";
	cout << players[tradingPlayer]->getMoneyInHand() << endl;
	
	cout << players[tradingPlayer]->getPlayerName() << "'s number of Get out of Jail Free Cards: ";
	cout << players[tradingPlayer]->getNumGetOutOfJailFreeCard() << endl;
	// end
	// for active player
	cout << players[activePlayer]->getPlayerName() << "'s Properties:" << endl;
	for (i=0;i<39;i++){
		if (board[i]->getPropertyOwner() == players[activePlayer]->getPlayerNum())
			cout << "Number: " << board[i]->getPropertyPosition() << "  Property: " << board[i]->getPropertyName() << "  Type: " << board[i]->getPropertyType() <<"  Value: " << board[i]->getPropertyValue() << "  Mortgaged? " << board[i]->getPropertyMortgage() << endl; 
	
	}
	cout << players[activePlayer]->getPlayerName() << "'s Money In Hand: ";
	cout << players[activePlayer]->getMoneyInHand() << endl;
	
	cout << players[activePlayer]->getPlayerName() << "'s number of Get out of Jail Free Cards: ";
	cout << players[activePlayer]->getNumGetOutOfJailFreeCard() << endl;
	// end;
	

}

void handoverMoney(Player** players, int givingPlayer, int receivingPlayer, int amount){

	if (amount == 0);
	else if (amount > 0 /* giving -> receiving */){
		if(amount <= players[givingPlayer]->getMoneyInHand()){
			players[givingPlayer]->setMoneyInHand(players[givingPlayer]->getMoneyInHand() - amount);
			players[receivingPlayer]->setMoneyInHand(players[receivingPlayer]->getMoneyInHand() + amount);
		}
		else
			cout << "Could not process request" << endl;
	}
	else if (amount < 0 /* giving <- receiving */){
		amount = (-amount);
		if(amount <= players[receivingPlayer]->getMoneyInHand()){
			players[givingPlayer]->setMoneyInHand(players[givingPlayer]->getMoneyInHand() + amount);
			players[receivingPlayer]->setMoneyInHand(players[receivingPlayer]->getMoneyInHand() - amount);
		}
		else
			cout << "Could not process request" << endl;
	}


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
	cout << "Are you sure " << s << " (enter 'yes' /anything else)\n"; 
	getline(cin >> ws,match);
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


