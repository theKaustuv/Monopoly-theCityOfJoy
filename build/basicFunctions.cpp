//#include <iostream>
//#include <string>
#include <cstdlib>
#include <ctime>
#include "basicFunctions.h"
//#include "Property.h"
//#include "Player.h"
//#include "RealEstate.h"
//#include "RailRoad.h"
//#include "Utility.h"
//#include "Jail.h"
//#include "ChanceCC.h"
//#include "SpecialProperties.h"

//using namespace std;

int randomNum(int lowerLim,int upperLim){

    srand(time(0));
    return (rand()/(upperLim-lowerLim+1) + lowerLim);
}

int rollDice(){

    return randomNum(1,6);
}
