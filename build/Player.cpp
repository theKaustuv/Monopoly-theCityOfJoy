#include <iostream>
#include <string>
#include "BaseFunctions.h"
#include "Property.h"
#include "Player.h"
#include "RealEstate.h"

using namespace std;


Player::Player(string Name, int Number)
:
playerName(Name),
playerNum(Number),
playerPosition(0),
moneyInHand(1500),
numPropertiesOwned(0),
numPropertiesMortgaged(0),
numRailroadsOwned(0),
numUtilitiesOwned(0),
numGetOutOfJailFreeCard(0),
isInJail(-1),
isBankrupt(false)
{

}


//Getters and  Setters:

string Player::getPlayerName(){
	return playerName;
}

int Player::getPlayerNum(){
	return playerNum;
}

void Player::setPlayerPosition(int Pos){
	playerPosition = Pos;
}

int Player::getPlayerPosition(){
	return playerPosition;
}

void Player::setMoneyInHand(int Money){
	moneyInHand = Money;
}

int Player::getMoneyInHand(){
	return moneyInHand;
}

void Player::setNumPropertiesOwned(int n){
	numPropertiesOwned = n;
}

int Player::getNumPropertiesOwned(){
	return numPropertiesOwned;
}

void Player::setNumPropertiesMortgaged(int n){
	numPropertiesMortgaged = n;
}

int Player::getNumPropertiesMortgaged(){
	return numPropertiesMortgaged;
}

void Player::setNumRailroadsOwned(int n){
	numRailroadsOwned = n;
}

int Player::getNumRailroadsOwned(){
	return numRailroadsOwned;
}

void Player::setNumUtilitiesOwned(int n){
	numUtilitiesOwned = n;
}

int Player::getNumUtilitiesOwned(){
	return numUtilitiesOwned;
}

void Player::setNumGetOutOfJailFreeCard(int n){
	numGetOutOfJailFreeCard = n;
}

int Player::getNumGetOutOfJailFreeCard(){
	return numGetOutOfJailFreeCard;
}

void Player::setIsInJail(int n){
	isInJail = n;
}

int Player::getIsInJail(){
	return isInJail;
}

void Player::setIsBankrupt(bool val){
	isBankrupt = val;
}

bool Player::getIsBankrupt(){
	return isBankrupt;
}

// end