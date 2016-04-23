#include <iostream>
#include <string>
#include "BaseFunctions.h"
#include "Property.h"
#include "Player.h"
#include "RealEstate.h"
#include "RailRoad.h"
#include "Utility.h"
#include "Jail.h"
#include "ChanceCC.h"
#include "SpecialProperties.h"


using namespace std;


Property::Property(string Name, string Type){

    PropertyName=Name;
    propertyType=Type;
    if (Type=="realEstate")
        realEstate bo;
}
