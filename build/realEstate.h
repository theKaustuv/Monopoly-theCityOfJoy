#ifndef REALESTATE_H
#define REALESTATE_H


class RealEstate
{
    public:
        RealEstate(int r[6], int Pos);

    private:
        const int rent[6];
        int numOfHouses;
        bool hotel;
        const int improvementCost;
};

#endif // REALESTATE_H
