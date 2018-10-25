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
	insert(hands_top, TAIL, remove(deck_top, 0, deck_count), hand_count);
}

card Poker::remove(card * &head, int pos, int& count)
{
	if (head == nullptr) return INVALID_CARD;
	card * cur_cd = head; // local iterator
	if (pos == 0) {
		// change head if deleting the 1st
		if (head->next != nullptr) {
			head = head->next; 
			head->prev = nullptr;
		} else {
			delete head;
			head = nullptr;
		}
		
	} else {
		for (int i = 0; i < pos; ++i) cur_cd = cur_cd->next;
		cur_cd->prev->next = cur_cd->next;
		if (cur_cd->next != nullptr) 
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
	} else if (pos == TAIL) { // insert at tail
		card * cur_cd = head; 
		for (; cur_cd->next != nullptr; cur_cd = cur_cd->next) {} // get to tail
		cur_cd->next = new_cd;
		new_cd->prev = cur_cd;
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
			card c { suit, num };
			if (!existInHands(c)) insert(deck_top, 0, c, deck_count);
		}
	shuffle();
}

bool Poker::isOption(string input)
{
	if (input == "DECK") return true;
	if (input == "NONE") return true;
	if (input == "ALL") return true;
	if (input == "EXIT") return true;
	if (input == "SWAP") return true;
	if (input.length() <= 5) { 
		for (auto & c : input)
			if (c < 'A' || c > 'E') return false;
		return true;
	}
	return false;
}

bool Poker::isLetter(char c)
{
	if (c >= 'A'&&c <= 'E')return true;
	return false;
}

unsigned char Poker::getValue(string str)
{
	try {
		unsigned char num = stoi(str);
		if (num >= 1 && num <= 13) return num;
		return INVALID_VALUE;
	} catch (const invalid_argument& e) {
		return INVALID_VALUE;
	}
	return INVALID_VALUE;
}

unsigned char Poker::getSuit(string input)
{
	if (input.length() > 1) return INVALID_SUIT;
	unsigned char ch = toupper(input[0]);
	switch (ch) {
		case 'S': return Spades;
		case 'H': return Hearts;
		case 'C': return Clubs;
		case 'D': return Diamonds;
		default: return INVALID_SUIT;
	}
}

void Poker::displayHands() const
{
	cout << "Your hand contains: " << endl;
	card * temp = hands_top;
	char i = 'A';
	while (temp != nullptr) {
		cout << i << ": " << *temp << ((i - 'A' <= delCount) ? " (Kept)" : "") << endl;
		temp = temp->next;
		++i;
	}
	cout << endl;
}

void Poker::displayDeck() const
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

void Poker::displayOptions() const
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

void Poker::getOption() 
{
	cin >> input;
	for (auto & c : input) c = toupper(c);
	while (!isOption(input)) {
		cout << sorry << endl;
		cin >> input;
		for (auto & c : input) c = toupper(c);
	}
}

int Poker::executeOption()
{
	if (input == "DECK") {
		displayDeck();
		return 0;
	}
	if (input == "NONE") {
		// discard all cards in your hand.
		destroy(hands_top);
		hands_top = nullptr;
		draw(5);
		return 0;
	}
	if (input == "ALL") {
		// keep all cards in your hand.
		// done
		return 0;
	}
	if (input == "EXIT") {
		return EXIT;
	}
	if (input == "SWAP") {
		// swap a card in your hand with one in the deck
		cout << "Enter the letter of the card in hand: ";
		string in;
		cin >> in;
		char letter = toupper(in[0]);
		while (in.length() > 1 || !isLetter(letter)) {
			cout << sorry << endl;
			cin >> in;
		}
		card hand = remove(hands_top, letter - 'A', hand_count);
		cout << "Remove " << hand << " from hand." << endl;
		cout << endl << "Enter the value of the card in the deck to swap with: ";
		cin >> in;
		unsigned char num;
		while ((num = getValue(in)) == INVALID_VALUE) {
			cout << endl << "Invalid value, please reenter: ";
			cin >> in;
		}
		cout << endl << "Enter the suit (c,d,h,s) of the card in the deck to swap with: ";
		cin >> in;
		unsigned char suit;
		while ((suit = getSuit(in)) == INVALID_SUIT) {
			cout << endl << "Invalid value, please reenter: ";
			cin >> in;
		}
		// search from the deck
		card * cur_cd = deck_top;
		bool found = false;
		for (; cur_cd->next != nullptr; cur_cd = cur_cd->next)
			if (cur_cd->num == num && cur_cd->suit == suit) { 
				found = true;
				break;
			}
		if (!found) {
			cout << card(suit, num) << " not found in the deck." << endl;
			return 0;
		}
		// found, swaping
		cur_cd->suit = hand.suit;
		cur_cd->num = hand.num;
		insert(hands_top, TAIL, card(suit, num), hand_count);
		return 0;
	}
	// determine which hands to discard
	bool delHands[MAXHAND] = { true, true, true, true, true };
	for (unsigned int i = 0; i < input.length(); ++i)
		delHands[input[i] - 'A'] = false;
	// reverse deleting doesn't cause problem
	delCount = 0;
	for (int i = MAXHAND - 1; i >= 0; --i)
		if (delHands[i]) {
			remove(hands_top, i, hand_count);
			++delCount;
		}
	draw(delCount);
	return 0;
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
