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
#include <string>

#define MAXHAND 5

// Suits
#define Spades 0
#define Hearts 1
#define Clubs  2
#define Diamonds 3

#define INVALID_CARD card(4,14)
#define INVALID_VALUE 0
#define INVALID_SUIT 4
#define EXIT 1
#define CHECK 2
#define TAIL -1

using namespace std;

const string sorry = "Sorry, I didn't understand, please reenter: ";

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
	string input;
	int delCount = MAXHAND;

	// private helpers
	// linked list related
	void swap(card * head, int idx1, int idx2);
	void destroy(card * &head);
	card remove(card * &head, int pos, int& count);
	void insert(card * &head, int pos, const card&, int& count);

	// deck and hands
	void draw();
	bool existInHands(card);
	void reconstructDeck();
	void shuffle();

	// checkers
	bool isRoyalFlush() const; // $800 BO
	bool isStraightFlush() const; // $50 BO
	bool is4OfAKind() const; // $25
	bool isFullHouse() const; // $9 BO
	bool isFlush() const; // $6
	bool isStraight() const; // $5
	bool is3OfAKind() const; // $3
	bool is2Pairs() const; // $2
	bool isPairJackOrBetter() const; // $1

	// miscellaneous
	bool isOption(string) const;
	bool isLetter(char) const;
	unsigned char getValue(string) const;
	unsigned char getSuit(string) const;
	void printValue(int) const;
public:
	Poker();
	~Poker();
	void draw(int);
	void displayHands();
	void displayDeck() const;
	void displayOptions() const;
	int winMoney();
	void getOption();
	int executeOption();
};

