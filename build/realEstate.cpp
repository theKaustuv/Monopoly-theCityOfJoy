#include <iostream>
#include <string>
#include "basicFunctions.h"
#include "Property.h"
#include "Player.h"
#include "RealEstate.h"

using namespace std;

RealEstate::RealEstate(int r[6], int Pos)
:rent(r),
numOfHouses(0),
hotel(false),
improvementCost( ( (Pos/10)+1 )*50  )
{

}
