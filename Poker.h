#pragma once
#include <iostream>

#define MAXHAND 5

// Suits
#define Spades 0
#define Hearts 1
#define Clubs  2
#define Diamonds 3

using namespace std;

struct card
{
	card(int suit, int num);
	card * prev;
	unsigned char suit;
	unsigned char num;
	card * next;
	friend ostream & operator<<(ostream&, const card&);
};

class Poker
{
	// fields
	card * deck_top = nullptr;
	card * hands_top = nullptr;
	int deck_count = 0;

	// private helpers
	void swapCardsInDeck(int, int);

public:
	Poker();
	~Poker();
	void shuffle();
	void draw(int);
	void displayHands();
	void displayDeck();
	void displayOptions();
	void discard();
};

