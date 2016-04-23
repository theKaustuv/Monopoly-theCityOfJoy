#ifndef PROPERTY_H
#define PROPERTY_H
#include <string>

using namespace std;

class Property
{
    public:
        Property(string Name,string Type);

    private:
        string PropertyName;
        string propertyType;
        int propertyOwner;

};

#endif // PROPERTY_H
