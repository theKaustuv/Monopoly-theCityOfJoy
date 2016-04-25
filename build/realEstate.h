#ifndef REALESTATE_H
#define REALESTATE_H


class RealEstate
{
    public:
        RealEstate(int r0,int r1,int r2,int r3,int r4, int r5, int Pos);
		void setNumOfHouses(int n);
		int getNumOfHouses();
		void setHotel(bool val);
		bool getHotel();
		int getImprovementCost();

    private:
        int rent[6];
        int numOfHouses;
        bool hotel;
        const int improvementCost;
};

#endif // REALESTATE_H
