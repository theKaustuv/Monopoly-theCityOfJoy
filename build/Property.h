#ifndef PROPERTY_H
#define PROPERTY_H
#include <iostream>
#include <string>
#include "realEstate.h"

using namespace std;

class Property
{
    public:
        Property(string Name,string Type, int Pos, int Value, int r0,int r1,int r2,int r3,int r4, int r5);
        Property(string Name,string Type, int Pos, int Value, int r0,int r1,int r2,int r3);
        Property(string Name,string Type, int Pos, int Value, int r0,int r1);
		Property(string Name,string Type, int Pos, int baseRent);
		Property(string Name,string Type, int Pos);
        void setPropertyOwner(int playerNum);
        int getPropertyOwner();
        void setPropertyMortgage(bool Val);
        bool getPropertyMortgage();
        int getPropertyValue();
        int getPropertyPosition();
        string getPropertyName();
        string getPropertyType();
    private:
        const string propertyName;
        const string propertyType;
        const int propertyPosition;
        int propertyOwner;
        const int propertyValue;
        bool propertyMortgage;
		RealEstate realEstate;
};

#endif // PROPERTY_H
