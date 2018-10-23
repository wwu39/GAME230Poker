#pragma once
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


#include <iostream>

#define MAXHAND 5

// Suits
#define Spades 0
#define Hearts 1
#define Clubs  2
#define Diamonds 3

#define INVALID_CARD card(4,14)

using namespace std;

struct card
{
	card(unsigned char suit, unsigned char num);
	card * prev;
	unsigned char suit;
	unsigned char num;
	card * next;
	card& operator=(const card&);
	friend ostream & operator<<(ostream&, const card&);
};

class Poker
{
	// fields
	card * deck_top = nullptr;
	card * hands_top = nullptr;
	int deck_count = 0;
	int hand_count = 0;

	// private helpers
	void swap(card * head, int idx1, int idx2);
	void destroy(card * head);
	void draw();
	card remove(card * &head, int pos, int& count);
	void insert(card * &head, int pos, const card&, int& count);
	bool existInHands(card);
	void reconstructDeck();
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

