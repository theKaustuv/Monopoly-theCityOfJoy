#include <iostream>
#include <string>
#include "baseFunctions.h"
#include "Property.h"
#include "Player.h"
#include "RealEstate.h"

using namespace std;

RealEstate::RealEstate(int r0,int r1,int r2,int r3,int r4, int r5, int Pos)
:
numOfHouses(0),
hotel(false),
improvementCost( ( (Pos/10)+1 )*50  )
{
	rent[0] = r0;
	rent[1] = r1;
	rent[2] = r2;
	rent[3] = r3;
	rent[4] = r4;
	rent[5] = r5;

}
