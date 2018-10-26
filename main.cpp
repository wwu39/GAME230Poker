// memory checking
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include "Poker.h"

#define START_MONEY 10
#define ANTE 1

using namespace std;

int main() {
	// memory checking
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	cout << "Welcome to Video Poker!" << endl;

	int money = START_MONEY;
	int ante = ANTE;

	cout << "You have started with $" << money << endl;

	while (1) { // game loop
		money -= ante;
		cout << "You pay a $" << ante << " ante and now have $" << money << endl << endl;
		Poker * game = new Poker();
		game->draw(5);
		while (1) {
			game->displayHands();
			game->displayOptions();
			game->getOption();
			int rc = game->executeOption();
			if (rc == EXIT) {
				delete game;
				return 0;
			}
			if (rc == CHECK) {
				game->displayHands();
				money += game->winMoney();
				cout << "You now have $" << money << endl;
				if (money <= 0) {
					cout << endl << "GAME OVER" << endl;
					delete game;
					system("pause");
					return 0;
				}
				cout << endl << "Press enter to play another round..." << endl;
				cin.get();
				cin.ignore();
				cout << "--------------------------------------------------" << endl << endl;
				money -= ante;
				cout << "You pay a $" << ante << " ante and now have $" << money << endl << endl;
			}
		}
	}
}