#include <iostream>
#include <string>
#include <limits>
#include "basicFunctions.h"
#include "Property.h"
#include "Player.h"
#include "realEstate.h"

using namespace std;

int main(){


Property Kolkata("Kolkata","realEstate");

cout << "Press ENTER to finish...";
cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

return 0;
}
