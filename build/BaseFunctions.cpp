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
	updatePropertyGroupOwnerships(&players[0],&board[0]);
}

void throwDice(Player** players, Property** board, int* activePlayer, const int* numberOfPlayers, int* playthisTurn, int* bankruptInflictingPlayer){

	/* local variables */
	int diceThrowOutcome;
	bool doubleDice=false;
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
		diceThrowOutcome = rollTwoDice(&doubleDice);
		cout << "You rolled " << diceThrowOutcome << endl;

		if (doubleDice == true)
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
				if (doubleDice == true){
					
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
		cout << "You are now at " << board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyName() << endl;

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
							else{

								if (board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.getNumOfHouses() != 0)
									handoverMoney(&players[0], *activePlayer, board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyOwner(), board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.rent[board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.getNumOfHouses()]);
								else
									handoverMoney(&players[0], *activePlayer, board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyOwner(), 2*board[ players[*activePlayer]->getPlayerPosition() ]->realEstate.rent[0]);
							}
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
						
							auctionProperty(&players[0], &board[0], players[*activePlayer]->getPlayerPosition(), *numberOfPlayers);
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
						
							auctionProperty(&players[0], &board[0], players[*activePlayer]->getPlayerPosition(), *numberOfPlayers);
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
						
							auctionProperty(&players[0], &board[0], players[*activePlayer]->getPlayerPosition(), *numberOfPlayers);
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
	updatePropertyGroupOwnerships(&players[0],&board[0]);
}

void updatePlayerPosition(Player** players, int* activePlayer, int diceThrowOutcome){
	
	players[*activePlayer]->setPlayerPosition(players[*activePlayer]->getPlayerPosition() + diceThrowOutcome);
	if (players[*activePlayer]->getPlayerPosition() > 39){
		
		players[*activePlayer]->setPlayerPosition(players[*activePlayer]->getPlayerPosition() - 40);
		players[*activePlayer]->setMoneyInHand(players[*activePlayer]->getMoneyInHand() + 200);	
	}
}

void showTitleDeed(Property** board, int propertyPosition){
	
	cout << "TITLE DEED" << endl;
	cout << "Property Name: " << board[propertyPosition]->getPropertyName() << endl;
	cout << "Property Type: " << board[propertyPosition]->getPropertyType() << endl;
	cout << "Property Value: " << board[propertyPosition]->getPropertyValue() << endl;
	cout << "Property Rent:" << endl;
	cout << "Basic: " << board[propertyPosition]->realEstate.rent[0] << endl;
	cout << "With 1 house: " << board[propertyPosition]->realEstate.rent[1] << endl;
	cout << "With 2 houses: " << board[propertyPosition]->realEstate.rent[2] << endl;
	cout << "With 3 houses: " << board[propertyPosition]->realEstate.rent[3] << endl;
	cout << "With 4 houses: " << board[propertyPosition]->realEstate.rent[4] << endl;
	cout << "With hotel: " << board[propertyPosition]->realEstate.rent[5] << endl;


}

void buyProperty(Player** players, Property** board, int* activePlayer){
	// from buy option
	if (board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyValue() > players[*activePlayer]->getMoneyInHand() )
		cout << "You Do Not Have Enough Money TO Buy This Property." << endl;
	else{
		players[*activePlayer]->setMoneyInHand( players[*activePlayer]->getMoneyInHand() - board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyValue() );
		board[ players[*activePlayer]->getPlayerPosition() ]->setPropertyOwner(players[*activePlayer]->getPlayerNum());
		players[*activePlayer]->setNumPropertiesOwned(players[*activePlayer]->getNumPropertiesOwned() + 1);
		if (board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyType() == "Railroad")
			players[*activePlayer]->setNumRailroadsOwned(players[*activePlayer]->getNumRailroadsOwned() + 1);
		if (board[ players[*activePlayer]->getPlayerPosition() ]->getPropertyType() == "Utility")
			players[*activePlayer]->setNumUtilitiesOwned(players[*activePlayer]->getNumUtilitiesOwned() + 1);
		updatePropertyGroupOwnerships(&players[0],&board[0]);

	}

}


void buyProperty(Player** players, Property** board, int propertyPosition, int biddingPlayer, int highestBid){
		// from auction
		players[biddingPlayer]->setMoneyInHand( players[biddingPlayer]->getMoneyInHand() - highestBid );
		board[propertyPosition]->setPropertyOwner(players[biddingPlayer]->getPlayerNum());
		players[biddingPlayer]->setNumPropertiesOwned(players[biddingPlayer]->getNumPropertiesOwned() + 1);
		if (board[propertyPosition]->getPropertyType() == "Railroad")
			players[biddingPlayer]->setNumRailroadsOwned(players[biddingPlayer]->getNumRailroadsOwned() + 1);
		if (board[propertyPosition]->getPropertyType() == "Utility")
			players[biddingPlayer]->setNumUtilitiesOwned(players[biddingPlayer]->getNumUtilitiesOwned() + 1);
		updatePropertyGroupOwnerships(&players[0],&board[0]);

}

void auctionProperty(Player** players, Property** board, int propertyPosition, const int numberOfPlayers){
	
	// local variables
	int playersRemainingInTheAuction = numberOfPlayers; //random high initial count, no practical relevance
	int biddingPlayer;
	int inAuction[4] = {1,1,1,1};
	int bidOption;
	int highestBid = 0;
	int bidIn;
	// end
	for (int i=0;i<numberOfPlayers;i++){
		if (players[i]->getIsBankrupt() == true){
			inAuction[i] = 0;
			playersRemainingInTheAuction -= 1;
		}
		else
			biddingPlayer = players[i]->getPlayerNum();
	}
	for (int i=numberOfPlayers;i<4;i++)
			inAuction[i] = 0;

	while(playersRemainingInTheAuction > 1){
		
		if(inAuction[biddingPlayer]==1){
			cout << players[biddingPlayer]->getPlayerName() << " - It's your turn now." << endl;
			cout << "Enter 1 - To Bid;  anything else to Forfeit" << endl;
			cin >> bidOption;
			if (bidOption == 1){
				
				cout << "Highest Bid so far: " << highestBid << "\nEnter higher value to bid, anything else to forfeit" << endl;
				cin >> bidIn;
				if (bidIn > players[biddingPlayer]->getMoneyInHand()){
					cout << "You do not have that much money. Forfeiting." << endl;
					playersRemainingInTheAuction -= 1;
					inAuction[biddingPlayer] = 0;
					bidIn = -1;
				}
				if (bidIn > highestBid)
					highestBid = bidIn;
				else{
					playersRemainingInTheAuction -= 1;
					inAuction[biddingPlayer] = 0;
				}
			}
			else{
				playersRemainingInTheAuction -= 1;
				inAuction[biddingPlayer] = 0;
			}
		}
		biddingPlayer = changeActivePlayer(&players[0],biddingPlayer,numberOfPlayers);
	}
	for (int i=0;i<numberOfPlayers;i++){
		if (inAuction[i] == 1)
			biddingPlayer = i;	
	}
	buyProperty(&players[0], &board[0], propertyPosition, biddingPlayer, highestBid);

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
	updatePropertyGroupOwnerships(&players[0],&board[0]);
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
	updatePropertyGroupOwnerships(&players[0],&board[0]);
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
		cout << "Sorry. You cannot improve upon this Property." << endl;

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
	updatePropertyGroupOwnerships(&players[0],&board[0]);
}

void handoverAllProperties(Player** players, Property** board, int* activePlayer, int* bankruptInflictingPlayer){
	
	players[*bankruptInflictingPlayer]->setNumGetOutOfJailFreeCard(players[*bankruptInflictingPlayer]->getNumGetOutOfJailFreeCard()+players[*activePlayer]->getNumGetOutOfJailFreeCard());
	for(int i=0;i<39;i++){
		if (board[i]->getPropertyOwner() == players[*activePlayer]->getPlayerNum()){
			handoverProperty(&players[0], &board[0], i, *activePlayer, *bankruptInflictingPlayer);
		}
	}
	updatePropertyGroupOwnerships(&players[0],&board[0]);

}

void updatePropertyGroupOwnerships(Player** players, Property** board){
	// local variables
	// end

	// Brown Group
	if (board[1]->getPropertyOwner() == board[3]->getPropertyOwner() && board[1]->getPropertyOwner() >= 0){
		
		board[1]->setPropertyGroupOwned(true);
		board[3]->setPropertyGroupOwned(true);
	}
	else
	{
		board[1]->setPropertyGroupOwned(false);
		board[3]->setPropertyGroupOwned(false);
	}
		
	// Light Blue Group
	if (board[6]->getPropertyOwner() == board[8]->getPropertyOwner() && board[8]->getPropertyOwner() == board[9]->getPropertyOwner() && board[6]->getPropertyOwner() >= 0){
		
		board[6]->setPropertyGroupOwned(true);
		board[8]->setPropertyGroupOwned(true);
		board[9]->setPropertyGroupOwned(true);
	}
	else
	{
		board[6]->setPropertyGroupOwned(false);
		board[8]->setPropertyGroupOwned(false);
		board[9]->setPropertyGroupOwned(false);
	}

	// Pink Group

	if (board[11]->getPropertyOwner() == board[13]->getPropertyOwner() && board[13]->getPropertyOwner() == board[14]->getPropertyOwner() && board[11]->getPropertyOwner() >= 0){
		
		board[11]->setPropertyGroupOwned(true);
		board[13]->setPropertyGroupOwned(true);
		board[14]->setPropertyGroupOwned(true);
	}
	else
	{
		board[11]->setPropertyGroupOwned(false);
		board[13]->setPropertyGroupOwned(false);
		board[14]->setPropertyGroupOwned(false);
	}

	// Orange Group

	if (board[16]->getPropertyOwner() == board[18]->getPropertyOwner() && board[18]->getPropertyOwner() == board[19]->getPropertyOwner() && board[16]->getPropertyOwner() >= 0){
		
		board[16]->setPropertyGroupOwned(true);
		board[18]->setPropertyGroupOwned(true);
		board[19]->setPropertyGroupOwned(true);
	}
	else
	{
		board[16]->setPropertyGroupOwned(false);
		board[18]->setPropertyGroupOwned(false);
		board[19]->setPropertyGroupOwned(false);
	}

	// Red Group
	
	if (board[21]->getPropertyOwner() == board[23]->getPropertyOwner() && board[23]->getPropertyOwner() == board[24]->getPropertyOwner() && board[21]->getPropertyOwner() >= 0){
		
		board[21]->setPropertyGroupOwned(true);
		board[23]->setPropertyGroupOwned(true);
		board[24]->setPropertyGroupOwned(true);
	}
	else
	{
		board[21]->setPropertyGroupOwned(false);
		board[23]->setPropertyGroupOwned(false);
		board[24]->setPropertyGroupOwned(false);
	}
	// Yellow Group

	if (board[26]->getPropertyOwner() == board[27]->getPropertyOwner() && board[27]->getPropertyOwner() == board[29]->getPropertyOwner() && board[26]->getPropertyOwner() >= 0){
		
		board[26]->setPropertyGroupOwned(true);
		board[27]->setPropertyGroupOwned(true);
		board[29]->setPropertyGroupOwned(true);
	}
	else
	{
		board[26]->setPropertyGroupOwned(false);
		board[27]->setPropertyGroupOwned(false);
		board[29]->setPropertyGroupOwned(false);
	}

	// Green Group

	if (board[31]->getPropertyOwner() == board[32]->getPropertyOwner() && board[32]->getPropertyOwner() == board[34]->getPropertyOwner() && board[31]->getPropertyOwner() >= 0){
		
		board[31]->setPropertyGroupOwned(true);
		board[32]->setPropertyGroupOwned(true);
		board[34]->setPropertyGroupOwned(true);
	}
	else
	{
		board[31]->setPropertyGroupOwned(false);
		board[32]->setPropertyGroupOwned(false);
		board[34]->setPropertyGroupOwned(false);
	}

	// Dark Blue Group
	
	if (board[37]->getPropertyOwner() == board[39]->getPropertyOwner() && board[37]->getPropertyOwner() >= 0){
		
		board[37]->setPropertyGroupOwned(true);
		board[39]->setPropertyGroupOwned(true);
	}
	else
	{
		board[37]->setPropertyGroupOwned(false);
		board[39]->setPropertyGroupOwned(false);
	}

	// Railroads

	if (board[5]->getPropertyOwner() == board[15]->getPropertyOwner() && board[15]->getPropertyOwner() == board[25]->getPropertyOwner() && board[25]->getPropertyOwner() == board[35]->getPropertyOwner() && board[5]->getPropertyOwner() >= 0){
		
		board[5]->setPropertyGroupOwned(true);
		board[15]->setPropertyGroupOwned(true);
		board[25]->setPropertyGroupOwned(true);
		board[35]->setPropertyGroupOwned(true);
	}
	else
	{
		board[5]->setPropertyGroupOwned(false);
		board[15]->setPropertyGroupOwned(false);
		board[25]->setPropertyGroupOwned(false);
		board[35]->setPropertyGroupOwned(false);
	}

	// Utilities
	
	if (board[12]->getPropertyOwner() == board[28]->getPropertyOwner() && board[12]->getPropertyOwner() >= 0){
		
		board[12]->setPropertyGroupOwned(true);
		board[28]->setPropertyGroupOwned(true);
	}
	else
	{
		board[12]->setPropertyGroupOwned(false);
		board[28]->setPropertyGroupOwned(false);
	}

}

void displayTurnOptions(int* playthisTurn){

	cout << "\nYour Possible Actions:" << endl;
	if (*playthisTurn != 0)
		cout << "1 - Throw Dice" << endl;
	cout << "2 - Trade\n3 - Manage\n-1 - End Turn\n-999 - Quit Game" << endl;

}

void showAllPlayerDetails(Player** players, const int numberOfPlayers){
	
	for(int i=0;i<numberOfPlayers;i++){
		if(players[i]->getIsBankrupt() == false){
			cout << players[i]->getPlayerName() << "\nPosition: " << players[i]->getPlayerPosition() <<"  Money: " << players[i]->getMoneyInHand() << "Properties: " << players[i]->getNumPropertiesOwned() << endl;
		}
	}

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


