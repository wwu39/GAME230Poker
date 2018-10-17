#include "Poker.h"

#include <ctime>
#include <cstdlib>

void Poker::swapCardsInDeck(int a, int b)
{
	// get to specified locations
	card * tmp1 = deck_top;
	for (int i = 0; i < a; ++i) tmp1 = tmp1->next;
	card * tmp2 = deck_top;
	for (int i = 0; i < b; ++i) tmp2 = tmp2->next;

	// swaping only suit and num
	unsigned char tmpsuit = tmp1->suit;
	unsigned char tmpnum = tmp1->num;
	tmp1->suit = tmp2->suit;
	tmp1->num = tmp2->num;
	tmp2->suit = tmpsuit;
	tmp2->num = tmpnum;
}

Poker::Poker()
{
	// initializing the deck
	deck_top = new card(0, 1);
	++deck_count;
	card * cur_cd = deck_top;
	for (int i = 0; i < 4; ++i) {
		for (int j = 1; j <= 13; ++j) {
			if (i == 0 && j == 1)continue;
			card * temp = new card(i, j);
			cur_cd->next = temp;
			temp->prev = cur_cd;
			cur_cd = temp;
			temp = nullptr;
			++deck_count;
		}
	}
	shuffle();
}


Poker::~Poker()
{
	// free all blocks
	while (deck_top->next != nullptr) {
		deck_top = deck_top->next;
		delete deck_top->prev;
		deck_top->prev = nullptr;
	}
	delete deck_top;
	deck_top = nullptr;
}

void Poker::shuffle()
{
	for (int i = 0; i < 2 * deck_count; ++i) {
		srand((unsigned)time(nullptr) % (i + 1));
		int idx1 = rand() % deck_count;
		srand((unsigned)time(nullptr) % (2 * i + 1));
		int idx2 = rand() % deck_count;

		cout << "Swapping (" << idx1 << ") and (" << idx2 << ") cards" << endl;
		swapCardsInDeck(idx1, idx2);
	}
	
}

void Poker::draw(int num)
{
	for (int i = 0; i < num; ++i) {

	}
}

void Poker::displayHands()
{
	cout << "Your hand contains: " << endl;
	card * temp = hands_top;
	char i = 'A';
	while (temp != nullptr) {
		cout << i << ": " << *temp << endl;
		temp = temp->next;
		++i;
	}
}

void Poker::displayDeck()
{
	cout << "Deck(" << deck_count << "):" << endl;
	card * temp = deck_top;
	int count = 1;
	while (temp != nullptr) {
		cout << count << ": " << *temp << endl;
		temp = temp->next;
		++count;
	}
}

void Poker::displayOptions()
{
	cout << endl;
	cout << "The deck contains "<< deck_count <<" cards." << endl << endl;
	cout << "OPTIONS..." << endl
		<< "- Type the letters for the cards you wish to keep. (i.e., \"acd\")" << endl
		<< "- Type \"deck\" to view the cards remaining in the deck." << endl
		<< "- Type \"none\" to discard all cards in your hand." << endl
		<< "- Type \"all\" to keep all cards in your hand." << endl
		<< "- Type \"exit\" to exit the game." << endl
		<< "- Type \"swap\" to CHEAT and swap a card in your hand with one in the deck." << endl
		<< "YOUR CHOICE: " << endl;
}

void Poker::discard()
{
}

ostream & operator<<(ostream & os, const card & c)
{
	switch (c.num) {
		case 11: os << "Jack";  break;
		case 12: os << "Queen"; break;
		case 13: os << "King";  break;
		case 1:  os << "Ace";   break;
		default: os << +c.num;   break;
	}
	os << " of ";
	switch (c.suit) {
		case Spades:    os << "Spades"  ; break;
		case Hearts:    os << "Hearts"  ; break;
		case Clubs:	    os << "Clubs"   ; break;
		case Diamonds:  os << "Diamonds"; break;
		default: os << "ERROR!!!";
	}
	return os;
}

card::card(int suit, int num)
{
	this->suit = suit;
	this->num = num;
	prev = nullptr;
	next = nullptr;
}
