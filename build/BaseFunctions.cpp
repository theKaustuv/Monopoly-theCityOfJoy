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
	bool* doubleDice;
	*doubleDice = false;
	string useCardString;
	string useBailString;
	string landedPropertyType;
	int landedPropertyOwnership;
	int buyAuction = -1;
	bool buyAuctionFlag = false;
	int CCNumber;
	int chanceNumber;
	/* end */

	if (*playthisTurn == 0)
		cout << "You have no more Dice Rolls left for this turn.\nYou can trade, Manage your properties or End your turn" << endl;
	else{
		diceThrowOutcome = rollTwoDice(doubleDice);
		
		if (*doubleDice == true)
			(*playthisTurn) += 1;
		else
			(*playthisTurn) = 0;

		// setting player position
		if (*playthisTurn > 3){
			sendPlayerToJail(&players[0], activePlayer);
			(*playthisTurn) = 0;
		}
		else{

			if (players[*activePlayer]->getIsInJail() >= 0){
				players[*activePlayer]->setIsInJail(players[*activePlayer]->getIsInJail() + 1);
				if (*doubleDice == true){
					
					players[*activePlayer]->setIsInJail(-1);
					updatePlayerPosition(&players[0], activePlayer, diceThrowOutcome);
				}
				else{
					if (players[*activePlayer]->getNumGetOutOfJailFreeCard() > 0){
						cout << "Do you want to use a Get Out Of Jail Free Card to get out of Jail? (press 'y' for yes, anything else to skip)" << endl;
						getline(cin >> ws,useCardString);
						if (useCardString == "y"){
							players[*activePlayer]->setNumGetOutOfJailFreeCard(players[*activePlayer]->getNumGetOutOfJailFreeCard() - 1);
							players[*activePlayer]->setIsInJail(-1);
							updatePlayerPosition(&players[0], activePlayer, diceThrowOutcome);
						}
					}
					if (players[*activePlayer]->getIsInJail() >= 3){
						cout << players[*activePlayer]->getPlayerName() << " posted bail" << endl;
						players[*activePlayer]->setMoneyInHand(players[*activePlayer]->getMoneyInHand() - 50);
						players[*activePlayer]->setIsInJail(-1);
						updatePlayerPosition(&players[0], activePlayer, diceThrowOutcome);
					}
					else if (players[*activePlayer]->getIsInJail() >= 0){
						cout << "Do you want to post bail of 50 to get out of Jail? (press 'y' for yes, anything else to skip)" << endl;
						getline(cin >> ws,useBailString);
						if (useBailString == "y"){
							players[*activePlayer]->setMoneyInHand(players[*activePlayer]->getMoneyInHand() - 50);
							players[*activePlayer]->setIsInJail(-1);
							updatePlayerPosition(&players[0], activePlayer, diceThrowOutcome);
						}
					}

				}
			}
			else{
				updatePlayerPosition(&players[0], activePlayer, diceThrowOutcome);
			}

		}
		// end

		landedPropertyType = board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyType();
		landedPropertyOwnership = board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyOwner();
		if( board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyMortgage() == false ){
			if (landedPropertyType == "RealEstate"){
			
				if (landedPropertyOwnership == *activePlayer);
				else if (landedPropertyOwnership >= 0){

						if (board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyGroupOwned() == false)
							handoverMoney(&players[0], *activePlayer, board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyOwner(), board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.rent[0]);
						else{
							if (board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.getHotel() == true)
								handoverMoney(&players[0], *activePlayer, board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyOwner(), board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.rent[5]);
							else
								handoverMoney(&players[0], *activePlayer, board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyOwner(), board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.rent[board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.getNumOfHouses()]);
						}
				
				}
				else{
					showTitleDeed(&board[0], players[*activePlayer]->getPlayerPosition());

					while(buyAuctionFlag == false){

						cout << "Enter 1 to Buy Property, 2 to Trade with Other Players, 3 to Manage your own properties, 4 - to Pass for Auction:  ";
						cin >> buyAuction;
						if (buyAuction == 1){
					
							if (board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyValue() > players[*activePlayer]->getMoneyInHand()){
						
								cout << "You do not have enough money to buy this property. Passing for Auction!" << endl;
								buyAuction = 4;
							}
							else{
							
								buyProperty(&players[0], &board[0], activePlayer);
								buyAuctionFlag = true;
								cout << "Congratulations. You bought yourself this property." << endl;
							}
						}
						if (buyAuction == 2)
							tradeWithOtherPlayers(&players[0], &board[0], activePlayer, numberOfPlayers);
						else if (buyAuction == 3)
							manageProperties(&players[0], &board[0], activePlayer, numberOfPlayers);
						else if (buyAuction == 4){
						
							auctionProperty(&players[0], &board[0], players[*activePlayer]->getPlayerPosition());
							buyAuctionFlag = true;
						}

					}
				}
			
			}
			else if (landedPropertyType == "Railroad"){

				
				if (landedPropertyOwnership == *activePlayer);
				else if (landedPropertyOwnership >= 0){

					handoverMoney(&players[0], *activePlayer, board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyOwner(), board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.rent[players[ board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyOwner() ]->getNumRailroadsOwned() - 1]);
				}
				else{
					showTitleDeed(&board[0], players[*activePlayer]->getPlayerPosition());

					while(buyAuctionFlag == false){

						cout << "Enter 1 to Buy Property, 2 to Trade with Other Players, 3 to Manage your own properties, 4 - to Pass for Auction:  ";
						cin >> buyAuction;
						if (buyAuction == 1){
					
							if (board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyValue() > players[*activePlayer]->getMoneyInHand()){
						
								cout << "You do not have enough money to buy this property. Passing for Auction!" << endl;
								buyAuction = 4;
							}
							else{
							
								buyProperty(&players[0], &board[0], activePlayer);
								buyAuctionFlag = true;
								cout << "Congratulations. You bought yourself this property." << endl;
							}
						}
						if (buyAuction == 2)
							tradeWithOtherPlayers(&players[0], &board[0], activePlayer, numberOfPlayers);
						else if (buyAuction == 3)
							manageProperties(&players[0], &board[0], activePlayer, numberOfPlayers);
						else if (buyAuction == 4){
						
							auctionProperty(&players[0], &board[0], players[*activePlayer]->getPlayerPosition());
							buyAuctionFlag = true;
						}

					}
				}
			
		
			}
			else if (landedPropertyType == "Utility"){
			
					
				if (landedPropertyOwnership == *activePlayer);
				else if (landedPropertyOwnership >= 0){

					handoverMoney(&players[0], *activePlayer, board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyOwner(), (diceThrowOutcome)*( board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.rent[players[ board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyOwner() ]->getNumUtilitiesOwned() - 1] ));
				}
				else{
					showTitleDeed(&board[0], players[*activePlayer]->getPlayerPosition());

					while(buyAuctionFlag == false){

						cout << "Enter 1 to Buy Property, 2 to Trade with Other Players, 3 to Manage your own properties, 4 - to Pass for Auction:  ";
						cin >> buyAuction;
						if (buyAuction == 1){
					
							if (board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyValue() > players[*activePlayer]->getMoneyInHand()){
						
								cout << "You do not have enough money to buy this property. Passing for Auction!" << endl;
								buyAuction = 4;
							}
							else{
							
								buyProperty(&players[0], &board[0], activePlayer);
								buyAuctionFlag = true;
								cout << "Congratulations. You bought yourself this property." << endl;
							}
						}
						if (buyAuction == 2)
							tradeWithOtherPlayers(&players[0], &board[0], activePlayer, numberOfPlayers);
						else if (buyAuction == 3)
							manageProperties(&players[0], &board[0], activePlayer, numberOfPlayers);
						else if (buyAuction == 4){
						
							auctionProperty(&players[0], &board[0], players[*activePlayer]->getPlayerPosition());
							buyAuctionFlag = true;
						}

					}
				}
			
		
			}
			else if (landedPropertyType == "CC"){
				
				CCNumber = randomNum(1, 2, 1);

				if (CCNumber == 1){
					cout << "Congratulations. You won 200." << endl;
					players[*activePlayer]->setMoneyInHand(players[*activePlayer]->getMoneyInHand() + 200);
				}
				else if (CCNumber == 2){
					cout << "Congratulations. You lost 10." << endl;
					players[*activePlayer]->setMoneyInHand(players[*activePlayer]->getMoneyInHand() - 10);
				}
		
		
			}
			else if (landedPropertyType == "Chance"){
				chanceNumber = randomNum(1, 2, 1);

				if (chanceNumber == 1){
					cout << "OOPS. You lost 150." << endl;
					players[*activePlayer]->setMoneyInHand(players[*activePlayer]->getMoneyInHand() - 150);
				}
				else if (chanceNumber == 2){
					cout << "Congratulations. You won 50." << endl;
					players[*activePlayer]->setMoneyInHand(players[*activePlayer]->getMoneyInHand() + 50);
				}
		
			}
			else if (landedPropertyType == "MoneyProperties"){
				
				players[*activePlayer]->setMoneyInHand(players[*activePlayer]->getMoneyInHand() - board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.rent[0]);
			}
			else if (landedPropertyType == "SpecialProperties"){
				
				if ( board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyName() == "Free Parking");
				else if ( board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyName() == "Jail" );
				else if ( board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyName() == "Go To Jail" ){
					
					cout << "OOPS. You have been sent to Jail." << endl;
					sendPlayerToJail(&players[0], activePlayer);
				}
		
		
			}
		}

	
	}
}

void updatePlayerPosition(Player** players, int* activePlayer, int diceThrowOutcome){
	//todo

}

void showTitleDeed(Property** board, int propertyPosition){
	//todo

}

void buyProperty(Player** players, Property** board, int* activePlayer){
	//todo

}

void auctionProperty(Player** players, Property** board, int propertyPosition){
	//todo

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
	else
		cout << "Sorry. The trade could not be completed." << endl;
	// end

}

void manageProperties(Player** players, Property** board, int* activePlayer, const int* numberOfPlayers){

	// local variables
	int propertyToManage = -1;
	int managingOption = -1;
	// end

	displayManageableProperties(&players[0], &board[0], *activePlayer);
	
	while(propertyToManage == -1){
		cout << "Enter Valid Corresponding Number to the property you want to manage: (-999 to exit)" << endl;
		cin >> propertyToManage;
		if (propertyToManage == -999)
			break;
		else if (propertyToManage < 0 || propertyToManage > 39)
			propertyToManage = 0;
		if(board[propertyToManage]->getPropertyOwner() != players[*activePlayer]->getPlayerNum())
			propertyToManage = -1;
	}
	if (propertyToManage != -999){

		managingOption = chooseManagingOptionForGivenProperty(&board[0], propertyToManage);
		if (managingOption == 1 || managingOption == 2)
			mortgageUnmortgageProperty(&players[0], &board[0], *activePlayer, propertyToManage, ( managingOption - 1 ) );
		else if (managingOption == 3)
			improveProperty(&players[0], &board[0], *activePlayer, propertyToManage);
	
	}
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

int chooseManagingOptionForGivenProperty(Property** board, int propertyToManage){

	// local variables
	int managingOption = -1;
	// end

	cout << board[propertyToManage]->getPropertyName() << "  Type: " << board[propertyToManage]->getPropertyType() <<"  Value: " << board[propertyToManage]->getPropertyValue() << endl;
	if (board[propertyToManage]->getPropertyMortgage() == false)
		cout << "1 - Mortgage Available for " << 0.5*( board[propertyToManage]->getPropertyValue() ) << endl;
	else
		cout << "2 - Unmortgage Available for " << 0.6*( board[propertyToManage]->getPropertyValue() ) << endl;
	if ( (board[propertyToManage]->getPropertyGroupOwned() == true) && (board[propertyToManage]->getPropertyType() == "RealEstate") )
		cout << "3 - Improve Property" << endl;

	cout << "Enter Valid Corresponding Number to Manage Property or enter any other number to exit: ";
	cin >> managingOption;
	
	if (managingOption == 1 && board[propertyToManage]->getPropertyMortgage() == false);
	else if (managingOption == 2 && board[propertyToManage]->getPropertyMortgage() == true);
	else if ( managingOption == 3 && (board[propertyToManage]->getPropertyGroupOwned() == true) && (board[propertyToManage]->getPropertyType() == "RealEstate") );
	else
		managingOption = -1;
	return managingOption;
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

void displayManageableProperties(Player** players, Property** board, int activePlayer){

	cout << players[activePlayer]->getPlayerName() << "'s Properties:" << endl;
	for (int i=0;i<39;i++){
		if (board[i]->getPropertyOwner() == players[activePlayer]->getPlayerNum()){
			cout << "Number: " << board[i]->getPropertyPosition() << "  Property: " << board[i]->getPropertyName() << "  Type: " << board[i]->getPropertyType() <<"  Value: " << board[i]->getPropertyValue() << endl;
			if (board[i]->getPropertyMortgage() == false)
				cout << "Mortgage Available\t";
			else
				cout << "Unmortgage Available\t";
			if ( (board[i]->getPropertyGroupOwned() == true) && (board[i]->getPropertyType() == "RealEstate") )
				cout << "Property Improvement Available" << endl;
		}
	
	}
	
}

void mortgageUnmortgageProperty(Player** players, Property** board, int activePlayer, int propertyToManage, int mortgageUnmortgageOption /* 0 - Mortgage 1- Unmortgage */){

	//local variables

	// end
	if (mortgageUnmortgageOption == 0 && board[propertyToManage]->getPropertyMortgage() == false){

		if (board[propertyToManage]->realEstate.getHotel() == true){
			board[propertyToManage]->realEstate.setHotel(false);
			players[activePlayer]->setMoneyInHand( players[activePlayer]->getMoneyInHand() + 0.7*board[propertyToManage]->realEstate.getImprovementCost() );
		}

		
		players[activePlayer]->setMoneyInHand( players[activePlayer]->getMoneyInHand() + 0.7*board[propertyToManage]->realEstate.getNumOfHouses()*board[propertyToManage]->realEstate.getImprovementCost() );
		board[propertyToManage]->realEstate.setNumOfHouses(0);

		board[propertyToManage]->setPropertyMortgage(true);
		players[activePlayer]->setNumPropertiesMortgaged(players[activePlayer]->getNumPropertiesMortgaged()+1);
		players[activePlayer]->setMoneyInHand( players[activePlayer]->getMoneyInHand() + 0.5*board[propertyToManage]->getPropertyValue() );
	
		cout << "Mortgaging Done Successfully." << endl;
	}
	else if (mortgageUnmortgageOption == 1 && board[propertyToManage]->getPropertyMortgage() == true){

		if (players[activePlayer]->getMoneyInHand() >= 0.6*board[propertyToManage]->getPropertyValue()){

			board[propertyToManage]->setPropertyMortgage(false);
			
			players[activePlayer]->setNumPropertiesMortgaged(players[activePlayer]->getNumPropertiesMortgaged()-1);
			players[activePlayer]->setMoneyInHand( players[activePlayer]->getMoneyInHand() - 0.6*board[propertyToManage]->getPropertyValue() );
			cout << "Unmortgaging Done Successfully." << endl;
		}
		else
			cout << "Unmortgaging Unsuccessfull. Player does not have enough money." << endl;
	
	}
	else
		cout << "Sorry.Request could not be completed." << endl;

}

void improveProperty(Player** players, Property** board, int activePlayer, int propertyToManage){

	// local variables
	int buildDestroyOption = -1;
	int buildDestroyAmount = -1;
	int maxBuildDestroyAmount = -1;
	// end;
	if (board[propertyToManage]->getPropertyOwner() == players[activePlayer]->getPlayerNum() && board[propertyToManage]->getPropertyMortgage() == false && board[propertyToManage]->getPropertyGroupOwned() == true && board[propertyToManage]->getPropertyType() == "RealEstate"){
		
		while(buildDestroyOption == -1){

			cout << "Enter 1 - Build On Property   2 - Remove from Property   -999 to exit" << endl;
			cin >> buildDestroyOption;
			if (buildDestroyOption == -999);
			else if (buildDestroyOption == 1){
				// build on property
				if (board[propertyToManage]->realEstate.getHotel() == true)
					cout << "Improvements are at a max for this property." << endl;
				else{

					maxBuildDestroyAmount = 5 - board[propertyToManage]->realEstate.getNumOfHouses();
					cout << "Enter the number of houses/hotels to build: (max " << maxBuildDestroyAmount  << ") Enter anything else to make no change" << endl;
					cin >> buildDestroyAmount;
					if (buildDestroyAmount < 1 || buildDestroyAmount > maxBuildDestroyAmount)
						buildDestroyAmount = 0;
					if (players[activePlayer]->getMoneyInHand() >= buildDestroyAmount*board[propertyToManage]->realEstate.getImprovementCost()){

						board[propertyToManage]->realEstate.setNumOfHouses(board[propertyToManage]->realEstate.getNumOfHouses() + buildDestroyAmount);
						if (board[propertyToManage]->realEstate.getNumOfHouses() > 4){
							board[propertyToManage]->realEstate.setNumOfHouses(4);
							board[propertyToManage]->realEstate.setHotel(true);
						}
						players[activePlayer]->setMoneyInHand(players[activePlayer]->getMoneyInHand() - buildDestroyAmount*board[propertyToManage]->realEstate.getImprovementCost());
					
					
					}
					else
						cout << "Player does not have required money." << endl;
				}
			
			}
			else if (buildDestroyOption == 2){
				// Remove from Property
				if (board[propertyToManage]->realEstate.getNumOfHouses() == 0)
					cout << "Improvements are already at a minimum for this property." << endl;
				else{

					maxBuildDestroyAmount = board[propertyToManage]->realEstate.getNumOfHouses();
					if (board[propertyToManage]->realEstate.getHotel() == true)
						maxBuildDestroyAmount += 1;
					cout << "Enter the number of houses/hotels to remove: (max " << maxBuildDestroyAmount  << ") Enter anything else to make no change" << endl;
					cin >> buildDestroyAmount;
					if (buildDestroyAmount < 1 || buildDestroyAmount > maxBuildDestroyAmount)
						buildDestroyAmount = 0;
					players[activePlayer]->setMoneyInHand(players[activePlayer]->getMoneyInHand() + 0.7*buildDestroyAmount*board[propertyToManage]->realEstate.getImprovementCost());
					if (buildDestroyAmount > 0 && board[propertyToManage]->realEstate.getHotel() == true){
						board[propertyToManage]->realEstate.setHotel(false);
						buildDestroyAmount -= 1;
					}
					board[propertyToManage]->realEstate.setNumOfHouses(board[propertyToManage]->realEstate.getNumOfHouses() - buildDestroyAmount);
	
					
				}
			
			}
			else
				buildDestroyOption = -1;
		}
	}
	else
		cout << "Sorry. This Property cannot be improved upon." << endl;

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


