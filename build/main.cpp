#include <iostream>
#include <string>
#include <limits>
#include "BaseFunctions.h"
#include "Property.h"
#include "Player.h"

using namespace std;


int main(){

/* initialize Board */

Property passGo					(/*name:*/ "PASS/GO",					/*type:*/ "MoneyProperties",		/*position:*/ 0,			/* base rents:*/ -200);
Property mediterraneanAvenue	(/*name:*/ "Mediterranean Avenue",		/*type:*/ "RealEstate",				/*position:*/ 1,	/*value:*/ 60,		/*rents:*/ 2,	 10,	 30,	 90,	160,	 250 );
Property cc1					(/*name:*/ "Community Chest",			/*type:*/ "CC",						/*position:*/ 2);
Property balticAvenue			(/*name:*/ "Baltic Avenue",				/*type:*/ "RealEstate",				/*position:*/ 3,	/*value:*/ 60,		/*rents:*/ 4,	 20,	 60,	180,	320,	 450 );
Property incomeTax				(/*name:*/ "Income Tax",				/*type:*/ "MoneyProperties",		/*position:*/ 4,			/* base rents:*/ 200);
Property readingRR				(/*name:*/ "Reading Railroad",			/*type:*/ "Railroad",				/*position:*/ 5,	/*value:*/ 200,		/*rents:*/ 25,	 50,	100,	200);
Property orientalAvenue			(/*name:*/ "Oriental Avenue",			/*type:*/ "RealEstate",				/*position:*/ 6,	/*value:*/ 100,		/*rents:*/ 6,	 30,	 90,	270,	400,	 550 );
Property chance1				(/*name:*/ "Chance",					/*type:*/ "Chance",					/*position:*/ 7); 
Property vermontAvenue			(/*name:*/ "Vermont Avenue",			/*type:*/ "RealEstate",				/*position:*/ 8,	/*value:*/ 100,		/*rents:*/ 6,	 30,	 90,	270,	400,	 550 );
Property connecticutAvenue		(/*name:*/ "Connecticut Avenue",		/*type:*/ "RealEstate",				/*position:*/ 9,	/*value:*/ 120,		/*rents:*/ 8,	 40,	100,	300,	450,	 600 );
Property jail					(/*name:*/ "Jail",						/*type:*/ "SpecialProperties",		/*position:*/ 10);
Property stCharlesPlace			(/*name:*/ "St. Charles Place",			/*type:*/ "RealEstate",				/*position:*/ 11,	/*value:*/ 140,		/*rents:*/ 10,	 50,	150,	450,	625,	 750 );
Property electricCompany		(/*name:*/ "Electric Company",			/*type:*/ "Utility",				/*position:*/ 12,	/*value:*/ 150,		/*rents:*/ 4,	 10);	
Property statesAvenue			(/*name:*/ "States Avenue",				/*type:*/ "RealEstate",				/*position:*/ 13,	/*value:*/ 140,		/*rents:*/ 10,	 50,	150,	450,	625,	 750 );
Property virginiaAvenue			(/*name:*/ "Virginia Avenue",			/*type:*/ "RealEstate",				/*position:*/ 14,	/*value:*/ 160,		/*rents:*/ 12,	 60,	180,	500,	700,	 900);
Property pennsylvaniaRR			(/*name:*/ "Pennsylvania Railroad",		/*type:*/ "Railroad",				/*position:*/ 15,	/*value:*/ 200,		/*rents:*/ 25,	 50,	100,	200);
Property stJamesPlace			(/*name:*/ "St. James Place",			/*type:*/ "RealEstate",				/*position:*/ 16,	/*value:*/ 180,		/*rents:*/ 14,	 70,	200,	550,	750,	 950 );
Property cc2					(/*name:*/ "Community Chest",			/*type:*/ "CC",						/*position:*/ 17);
Property tennesseeAvenue		(/*name:*/ "Tennessee Avenue",			/*type:*/ "RealEstate",				/*position:*/ 18,	/*value:*/ 180,		/*rents:*/ 14,	 70,	200,	550,	750,	 950 );
Property newYorkAvenue			(/*name:*/ "New York Avenue",			/*type:*/ "RealEstate",				/*position:*/ 19,	/*value:*/ 200,		/*rents:*/ 16,	 80,	220,	600,	800,	1000 );
Property freeParking			(/*name:*/ "Free Parking",				/*type:*/ "SpecialProperties",		/*position:*/ 20);
Property kentuckyAvenue			(/*name:*/ "Kentucky Avenue",			/*type:*/ "RealEstate",				/*position:*/ 21,	/*value:*/ 220,		/*rents:*/ 18,	 90,	250,	700,	875,	1050 );
Property chance2				(/*name:*/ "Chance",					/*type:*/ "Chance",					/*position:*/ 22);
Property indianaAvenue			(/*name:*/ "Indiana Avenue",			/*type:*/ "RealEstate",				/*position:*/ 23,	/*value:*/ 220,		/*rents:*/ 18,	 90,	250,	700,	875,	1050);
Property illinoisAvenue			(/*name:*/ "Illinois Avenue",			/*type:*/ "RealEstate",				/*position:*/ 24,	/*value:*/ 240,		/*rents:*/ 20,	100,	300,	750,	925,	1100);
Property bORR					(/*name:*/ "B. & O. Railroad",			/*type:*/ "Railroad",				/*position:*/ 25,	/*value:*/ 200,		/*rents:*/ 25,	 50,	100,	200);
Property atlanticAvenue			(/*name:*/ "Atlantic Avenue",			/*type:*/ "RealEstate",				/*position:*/ 26,	/*value:*/ 260,		/*rents:*/ 22,	110,	330,	800,	975,	1150);
Property ventnorAvenue			(/*name:*/ "Ventnor Avenue",			/*type:*/ "RealEstate",				/*position:*/ 27,	/*value:*/ 260,		/*rents:*/ 22,	110,	330,	800,	975,	1150);
Property waterWorks				(/*name:*/ "Water Works",				/*type:*/ "Utility",				/*position:*/ 28,	/*value:*/ 150,		/*rents:*/ 4,	 10);	
Property marvinGardens			(/*name:*/ "Marvin Gardens",			/*type:*/ "RealEstate",				/*position:*/ 29,	/*value:*/ 280,		/*rents:*/ 24,	120,	360,	850,	1025,	1200);
Property goToJail				(/*name:*/ "Go To Jail",				/*type:*/ "SpecialProperties",		/*position:*/ 30);
Property pacificAvenue			(/*name:*/ "Pacific Avenue",			/*type:*/ "RealEstate",				/*position:*/ 31,	/*value:*/ 300,		/*rents:*/ 26,	130,	390,	900,	1100,	1275);
Property northCarolinaAvenue	(/*name:*/ "North Carolina Avenue",		/*type:*/ "RealEstate",				/*position:*/ 32,	/*value:*/ 300,		/*rents:*/ 26,	130,	390,	900,	1100,	1275);
Property cc3					(/*name:*/ "Community Chest",			/*type:*/ "CC",						/*position:*/ 33);
Property pennsylvaniaAvenue		(/*name:*/ "Pennsylvania Avenue",		/*type:*/ "RealEstate",				/*position:*/ 34,	/*value:*/ 320,		/*rents:*/ 28,	150,	450,	1000,	1200,	1400);
Property shortLineRR			(/*name:*/ "Short Line",				/*type:*/ "Railroad",				/*position:*/ 35,	/*value:*/ 200,		/*rents:*/ 25,	 50,	100,	200);
Property chance3				(/*name:*/ "Chance",					/*type:*/ "Chance",					/*position:*/ 36);
Property parkPlace				(/*name:*/ "Park Place",				/*type:*/ "RealEstate",				/*position:*/ 37,	/*value:*/ 350,		/*rents:*/ 35,	175,	500,	1100,	1300,	1500);
Property luxuryTax				(/*name:*/ "Luxury Tax",				/*type:*/ "MoneyProperties",		/*position:*/ 38,			/* base rents:*/ 100);
Property boardwalk				(/*name:*/ "Boardwalk",					/*type:*/ "RealEstate",				/*position:*/ 39,	/*value:*/ 400,		/*rents:*/ 50,	200,	600,	1400,	1700,	2000);

Property* board[40] = {&passGo,&mediterraneanAvenue,&cc1,&balticAvenue,&incomeTax,&readingRR,&orientalAvenue,&chance1,&vermontAvenue,&connecticutAvenue,&jail,&stCharlesPlace,&electricCompany,&statesAvenue,&virginiaAvenue,&pennsylvaniaRR,&stJamesPlace,&cc2,&tennesseeAvenue,&newYorkAvenue,&freeParking,&kentuckyAvenue,&chance2,&indianaAvenue,&illinoisAvenue,&bORR,&atlanticAvenue,&ventnorAvenue,&waterWorks,&marvinGardens,&goToJail,&pacificAvenue,&northCarolinaAvenue,&cc3,&pennsylvaniaAvenue,&shortLineRR,&chance3,&parkPlace,&luxuryTax,&boardwalk};


/* end */

/* variables */
int temporaryIntegerValue;
int	playersRemaining; // 1 to numberOfPlayers in value
int	activePlayer=0; // 0 to numberOfPlayers-1 in value
bool forcedExit = false;
int i;
string name[4] = {};
/* end */

/* initialize players */
cout << "Enter Number of Players (2 to 4 players only):";
cin >> temporaryIntegerValue;
const int numberOfPlayers = temporaryIntegerValue; // 2 to 4 in value
for(i=0;i<numberOfPlayers;i++){
	cout << "Enter Player " << i+1 << " Name: ";
	getline(cin >> ws,name[i]);
}

Player player1(name[0],0);Player player2(name[1],1);Player player3(name[2],2);Player player4(name[3],3);
Player* players[4] = {&player1, &player2, &player3, &player4};

/* end */

// Starting Sequence
cout << "Let's Play!!" << endl;
playersRemaining = numberOfPlayers;
activePlayer = chooseStartingPlayer(numberOfPlayers);
cout << players[activePlayer]->getPlayerName() << " gets to go first" << endl;
// end

// Playing Sequence
do{
	turn(&players[0], &board[0], &activePlayer, &numberOfPlayers, &forcedExit, &playersRemaining);
	
	// change turn
	if (forcedExit == false){
		activePlayer = changeActivePlayer(&players[0],activePlayer,numberOfPlayers);
		showAllPlayerDetails(&players[0], numberOfPlayers);
		cout << "It's "<< players[activePlayer]->getPlayerName() << "'s turn now.\n Press Enter to Proceed..";
		cin.ignore( numeric_limits<streamsize>::max(), '\n' );
	}
	// end.............and proceed to the next turn/exit

}while(playersRemaining > 1 && forcedExit == false);

// end
// winning sequence
if (playersRemaining == 1){
	for (i=0;i<numberOfPlayers;i++){
		if (players[i]->getIsBankrupt() == false)
			cout << "\nCONGRATULATIONS\n"<<players[i]->getPlayerName() << " WINS!!" << endl;
	}
}
// end

cout << "Thank You for Playing...\nPress ENTER to finish...";
cin.ignore( numeric_limits<streamsize>::max(), '\n' );

return 0;
}
