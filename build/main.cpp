#include <iostream>
#include <string>
#include <limits>
#include "BaseFunctions.h"
#include "Property.h"
#include "Player.h"

using namespace std;

/* initialize game */

Property Mediterrenean("Mediterrenean","RealEstate", 1, 60, /*rents:*/ 1,2,3,4,5,6 );

/* end */

int main(){

	cout << Mediterrenean.getPropertyName() << endl;

cout << "Press ENTER to finish...";
cin.ignore( numeric_limits<streamsize>::max(), '\n' );

return 0;
}
