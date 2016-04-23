#ifndef PLAYER_H
#define PLAYER_H
#include <string>

using namespace std;


class Player
{
    public:
        Player();

    private:
        const string playerName;
        const int playerNum;
        int playerPosition;
        int moneyInHand;
        int numRailroadsOwned;
        int numUtilitiesOwned;
        bool isInJail;
        int turnsInjail;
        bool isBankrupt;
};

#endif // PLAYER_H
