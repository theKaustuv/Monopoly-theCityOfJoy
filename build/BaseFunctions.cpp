#include <iostream>
#include <cstdlib>
#include <ctime>
#include "BaseFunctions.h"

using namespace std;

int randomNum(int lowerLim,int upperLim, int numberOfNumbers){

    int result=0;
    srand(time(0));
    for (int i=0;i<numberOfNumbers;i++)
        result+=(rand()%(upperLim-lowerLim+1) + lowerLim);
    return result;
}

int rollDice(int numberOfDice){

    return randomNum(1,6,numberOfDice);
}
