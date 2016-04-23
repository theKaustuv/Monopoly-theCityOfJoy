#include <iostream>
#include <string>
#include "BaseFunctions.h"
#include "Property.h"
#include "Player.h"
#include "RealEstate.h"

using namespace std;

Property::Property(string Name,string Type, int Pos, int Value, int rent[6])
:propertyName(Name),
propertyType(Type),
propertyPosition(Pos),
propertyOwner(-1),
propertyValue(Value),
propertyMortgage(false)
{
    if (getPropertyType()=="RealEstate")
        RealEstate propertyAttributes(rent,Pos);
    else if (getPropertyType()=="Railroad")
        // do we really need Railroad class? we can just multiply 25*number of railroads owned
    else if (getPropertyType()=="Utility")
        //same as railroads
    else if (getPropertyType()=="ChanceCC")
        //draw a card randomly
    else if (getPropertyType()=="Jail")


}

Property::Property(string Name,string Type, int Pos, int Value)
:propertyName(Name),
propertyType(Type),
propertyPosition(Pos),
propertyOwner(-1),
propertyValue(Value),
propertyMortgage(false)
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
