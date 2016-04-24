#ifndef PLAYER_H
#define PLAYER_H
#include <string>

using namespace std;


class Player
{
    public:
        Player(string Name, int Number);
		string getPlayerName();
		int getPlayerNum();
		void setPlayerPosition(int Pos);
		int getPlayerPosition();
		void setMoneyInHand(int Money);
		int getMoneyInHand();
		void setNumPropertiesOwned(int n);
		int getNumPropertiesOwned();
		void setNumPropertiesMortgaged(int n);
		int getNumPropertiesMortgaged();
		void setNumRailroadsOwned(int n);
		int getNumRailroadsOwned();
		void setNumUtilitiesOwned(int n);
		int getNumUtilitiesOwned();
		void setNumGetOutOfJailFreeCard(int n);
		int getNumGetOutOfJailFreeCard();
		void setIsInJail(int n);
		int getIsInJail();
		void setIsBankrupt(bool val);
		bool getIsBankrupt();


    private:
        const string playerName;
        const int playerNum;
        int playerPosition;
        int moneyInHand;
		int numPropertiesOwned;
		int numPropertiesMortgaged;
        int numRailroadsOwned;
        int numUtilitiesOwned;
		int numGetOutOfJailFreeCard;
        int isInJail;
        bool isBankrupt;
};

#endif // PLAYER_H
