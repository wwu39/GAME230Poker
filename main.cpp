#include "Poker.h"

using namespace std;

int main() {
	cout << "Welcome to Video Poker!" << endl;

	int money = 10;
	int ante = 1;

	cout << "You have started with $" << money << endl;

	while (1) { // game loop
		money -= ante;
		cout << "You pay a $" << ante << "ante and now have $" << money << endl;
		Poker * game = new Poker();
		game->draw(5);
		game->displayHands();
		game->displayDeck();
		game->displayOptions();
		delete game;
		system("pause");
		return 1;
	}
	return 0;
}