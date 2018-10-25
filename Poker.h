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
	void swap(card * head, int idx1, int idx2);
	void destroy(card * head);
	void draw();
	card remove(card * &head, int pos, int& count);
	void insert(card * &head, int pos, const card&, int& count);
	bool existInHands(card);
	void reconstructDeck();
	bool isOption(string);
	bool isLetter(char);
	unsigned char getValue(string);
	unsigned char getSuit(string);
public:
	Poker();
	~Poker();
	void shuffle();
	void draw(int);
	void displayHands() const;
	void displayDeck() const;
	void displayOptions() const;
	void discard();
	void getOption();
	int executeOption();
};

