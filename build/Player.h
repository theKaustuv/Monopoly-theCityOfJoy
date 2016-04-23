#ifndef PLAYER_H
#define PLAYER_H
#include <string>

using namespace std;


class Player
{
    public:
        Player();

    private:
        int numRailroadsOwned;
        int numUtilitiesOwned;
        bool isInJail;
        int turnsInjail;
};

#endif // PLAYER_H
