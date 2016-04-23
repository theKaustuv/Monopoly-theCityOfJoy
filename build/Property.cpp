#include <iostream>
#include <string>
#include "basicFunctions.h"
#include "Property.h"
#include "Player.h"
#include "realEstate.h"

using namespace std;


Property::Property(string Name, string Type){

    PropertyName=Name;
    propertyType=Type;
    if (Type=="realEstate")
        realEstate bo;
}
