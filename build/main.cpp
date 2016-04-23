#include <iostream>
#include <string>
#include <limits>
#include "BaseFunctions.h"
#include "Property.h"
#include "Player.h"

using namespace std;

/* initialize game */
int rent[6] = {1,2,3,4,5,6};
Property Mediterrenean("Mediterrenean","RealEstate", 1, 60, rent );

/* end */

int main(){


cout << "Press ENTER to finish...";
cin.ignore( numeric_limits<streamsize>::max(), '\n' );

return 0;
}
