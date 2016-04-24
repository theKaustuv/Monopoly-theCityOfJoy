#include <iostream>
#include <string>
#include "BaseFunctions.h"
#include "Property.h"
#include "Player.h"
#include "RealEstate.h"

using namespace std;

Property::Property(string Name,string Type, int Pos, int Value, int r0,int r1,int r2,int r3,int r4, int r5)
:propertyName(Name),
propertyType(Type),
propertyPosition(Pos),
propertyOwner(-1),
propertyValue(Value),
propertyMortgage(false),
realEstate(r0, r1, r2, r3, r4, r5,Pos)
{
    
}

Property::Property(string Name,string Type, int Pos, int Value, int r0,int r1,int r2,int r3)
:propertyName(Name),
propertyType(Type),
propertyPosition(Pos),
propertyOwner(-1),
propertyValue(Value),
propertyMortgage(false),
realEstate(r0, r1, r2, r3, /*these values will never be used :*/ -1, -1, 100)
{

}

Property::Property(string Name,string Type, int Pos, int Value, int r0,int r1)
:propertyName(Name),
propertyType(Type),
propertyPosition(Pos),
propertyOwner(-1),
propertyValue(Value),
propertyMortgage(false),
realEstate(r0, r1,/*these values will never be used :*/ -1, -1, -1, -1, 100)
{

}

Property::Property(string Name,string Type, int Pos, int baseRent)
:propertyName(Name),
propertyType(Type),
propertyPosition(Pos),
propertyOwner(-1),
propertyValue(-1),
propertyMortgage(false),
realEstate(baseRent, /*these values will never be used :*/ -1, -1, -1, -1, -1, 100)
{

}

Property::Property(string Name,string Type, int Pos)
:propertyName(Name),
propertyType(Type),
propertyPosition(Pos),
propertyOwner(-1),
propertyValue(-1),
propertyMortgage(false),
realEstate(/*these values will never be used :*/ -1, -1, -1, -1, -1, -1, 100)
{

}

void Property::setPropertyOwner(int playerNum){

    propertyOwner=playerNum;
}

int Property::getPropertyOwner(){

    return propertyOwner;
}

void Property::setPropertyMortgage(bool Val){

    propertyMortgage=Val;
}

bool Property::getPropertyMortgage(){

    return propertyMortgage;
}

int Property::getPropertyValue(){

    return propertyValue;
}

int Property::getPropertyPosition(){

    return propertyPosition;
}

string Property::getPropertyName(){

    return propertyName;
}

string Property::getPropertyType(){

    return propertyType;
}
