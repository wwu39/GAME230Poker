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

#include <ctime>
#include <cstdlib>

void Poker::swap(card * head, int a, int b)
{
	// get to specified locations
	card * tmp1 = head;
	for (int i = 0; i < a; ++i) tmp1 = tmp1->next;
	card * tmp2 = head;
	for (int i = 0; i < b; ++i) tmp2 = tmp2->next;

	// swaping
	card temp = *tmp1;
	*tmp1 = *tmp2;
	*tmp2 = temp;
}

void Poker::destroy(card * head)
{
	if (head == nullptr) return;
	while (head->next != nullptr) {
		head = head->next;
		delete head->prev;
		head->prev = nullptr;
	}
	delete head;
}

Poker::Poker()
{
	// initializing the deck
	deck_top = new card(0, 1);
	++deck_count;
	card * cur_cd = deck_top;
	for (int i = 0; i < 4; ++i) {
		for (int j = 1; j <= 13; ++j) {
			if (i == 0 && j == 1) continue;
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
	destroy(deck_top);
	deck_top = nullptr;
	destroy(hands_top);
	hands_top = nullptr;
}

void Poker::shuffle()
{
	for (int i = 0; i < 2 * deck_count; ++i) {
		srand((unsigned)time(nullptr) % (i + 1));
		int idx1 = rand() % deck_count;
		srand((unsigned)time(nullptr) % (2 * i + 1));
		int idx2 = rand() % deck_count;
		swap(deck_top, idx1, idx2);
	}
}

void Poker::draw(int num)
{
	for (int i = 0; i < num; ++i) draw();
}

void Poker::draw()
{
	// if run out of deck, rebuild one
	if (deck_top == nullptr) reconstructDeck();
	insert(hands_top, 0, remove(deck_top, 0, deck_count), hand_count);
}

card Poker::remove(card * &head, int pos, int& count)
{
	if (head == nullptr) return INVALID_CARD;
	card * cur_cd = head; // local iterator
	if (pos == 0) {
		// change head if deleting the 1st
		head = head->next; 
		head->prev = nullptr;
	} else {
		for (int i = 0; i < pos; ++i) cur_cd = cur_cd->next;
		cur_cd->prev->next = cur_cd->next;
		cur_cd->next->prev = cur_cd->prev;
	}
	card ret = *cur_cd;
	delete cur_cd;
	cur_cd = nullptr;
	--count;
	return ret;
}

void Poker::insert(card * &head, int pos, const card& c, int& count)
{
	card * new_cd = new card(c.suit, c.num);
	if (head == nullptr) { // insert the 1st
		head = new_cd;
		++count;
		return;
	}
	if (pos == 0) { // insert at head
		new_cd->next = head;
		head->prev = new_cd;
		head = new_cd; // change head
	} else {
		card * cur_cd = head;
		for (int i = 0; i < pos; ++i) cur_cd = cur_cd->next;
		cur_cd->prev->next = new_cd;
		new_cd->prev = cur_cd->prev;
		cur_cd->prev = new_cd;
		new_cd->next = cur_cd;
	}
	++count;
}

bool Poker::existInHands(card c)
{
	if (hands_top == nullptr) return false;
	for (card * i = hands_top; i->next != nullptr; i = i->next)
		if (i->suit == c.suit && i->num == c.num) return true;
	return false;
}

void Poker::reconstructDeck()
{
	for (unsigned char suit = 0; suit < 4; ++suit)
		for (unsigned char num = 1; num <= 13; ++num) {
			card c{ suit, num };
			if (!existInHands(c)) insert(deck_top, 0, c, deck_count);
		}
	shuffle();
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
	cout << endl;
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
	cout << endl;
}

void Poker::displayOptions()
{
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

card::card(unsigned char suit, unsigned char num)
{
	this->suit = suit;
	this->num = num;
	prev = nullptr;
	next = nullptr;
}

card & card::operator=(const card & c)
{
	suit = c.suit;
	num = c.num;
	return *this;
}
