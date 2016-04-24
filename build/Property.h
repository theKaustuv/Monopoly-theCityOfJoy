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
        string getPropertyName();
        string getPropertyType();
        int getPropertyPosition();
		void setPropertyOwner(int playerNum);
        int getPropertyOwner();
		int getPropertyValue();
        void setPropertyMortgage(bool Val);
        bool getPropertyMortgage();
                
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
