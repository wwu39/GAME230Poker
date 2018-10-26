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
#include <algorithm>

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

void Poker::destroy(card * &head)
{
	if (head == nullptr) return;
	while (head->next != nullptr) {
		head = head->next;
		delete head->prev;
		head->prev = nullptr;
	}
	delete head;
	head = nullptr;
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

bool Poker::isRoyalFlush() const
{
	int A[5] = { -4,-3,-2,-1,0 };
	int j = 0;
	for (card * i = hands_top; i != nullptr; i = i->next) {
		if (i->suit != hands_top->suit) return false;
		A[j++] = i->num;
	}
	sort(A, A + 5);
	return A[0] == 1 && A[1] == 10 && A[2] == 11 && A[3] == 12 && A[4] == 13;
}

bool Poker::isStraightFlush() const
{
	int A[5] = { -4,-3,-2,-1,0 };
	int j = 0;
	for (card * i = hands_top; i != nullptr; i = i->next) {
		if (i->suit != hands_top->suit) return false;
		A[j++] = i->num;
	}
	sort(A, A + 5);
	return A[1] == A[0] + 1 && A[2] == A[1] + 1 && A[3] == A[2] + 1 && A[4] == A[3] + 1;
}

bool Poker::isFullHouse() const
{
	int A[5] = { -4,-3,-2,-1,0 };
	int j = 0;
	for (card * i = hands_top; i != nullptr; i = i->next) A[j++] = i->num;
	sort(A, A + 5);
	return (A[0] == A[1] && A[1] == A[2] && A[3] == A[4])
		|| (A[0] == A[1] && A[2] == A[3] && A[3] == A[4]);
}

bool Poker::is4OfAKind() const
{
	int A[5] = { -4,-3,-2,-1,0 };
	int j = 0;
	for (card * i = hands_top; i != nullptr; i = i->next) A[j++] = i->num;
	sort(A, A + 5);
	return (A[0] == A[1] && A[1] == A[2] && A[2] == A[3])
		|| (A[1] == A[2] && A[2] == A[3] && A[3] == A[4]);
}

bool Poker::isFlush() const
{
	// all same suit
	for (card * i = hands_top->next; i != nullptr; i = i->next)
		if (i->suit != hands_top->suit) return false;
	return true;
}

bool Poker::isStraight() const
{
	int A[5] = { -4,-3,-2,-1,0 };
	int j = 0;
	for (card * i = hands_top; i != nullptr; i = i->next) A[j++] = i->num;
	sort(A, A + 5);
	return A[1] == A[0] + 1 && A[2] == A[1] + 1 && A[3] == A[2] + 1 && A[4] == A[3] + 1;
}

bool Poker::is3OfAKind() const
{
	int A[5] = { -4,-3,-2,-1,0 };
	int j = 0;
	for (card * i = hands_top; i != nullptr; i = i->next) A[j++] = i->num;
	sort(A, A + 5);
	return (A[0] == A[1] && A[1] == A[2])
		|| (A[1] == A[2] && A[2] == A[3])
		|| (A[2] == A[3] && A[3] == A[4]);
}

bool Poker::is2Pairs() const
{
	int A[5] = { -4,-3,-2,-1,0 };
	int j = 0;
	for (card * i = hands_top; i != nullptr; i = i->next) A[j++] = i->num;
	sort(A, A + 5);
	return (A[0] == A[1] && A[2] == A[3])
		|| (A[0] == A[1] && A[3] == A[4])
		|| (A[1] == A[2] && A[3] == A[4]);
}

bool Poker::isPairJackOrBetter() const
{
	int A[5] = { -4,-3,-2,-1,0 };
	int j = 0;
	for (card * i = hands_top; i != nullptr; i = i->next)
		if (i->num >= 11 || i->num == 1) A[j++] = i->num;
	sort(A, A + 5);
	return A[5] == A[4] || A[4] == A[3] || A[3] == A[2] || A[2] == A[1] || A[1] == A[0];
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
		} else { // deleting the only one
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
	for (card * i = hands_top; i != nullptr; i = i->next)
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

bool Poker::isOption(string input) const
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

bool Poker::isLetter(char c) const
{
	if (c >= 'A'&&c <= 'E')return true;
	return false;
}

unsigned char Poker::getValue(string str) const
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

unsigned char Poker::getSuit(string input) const
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

void Poker::printValue(int v) const
{
	switch (v) {
		case 11: cout << "J";  break;
		case 12: cout << "Q"; break;
		case 13: cout << "K";  break;
		case 1:  cout << "A";   break;
		default: cout << v;   break;
	}
	cout << " ";
}

void Poker::displayHands()
{
	cout << "Your hand contains: " << endl;
	card * temp = hands_top;
	char i = 'A';
	while (temp != nullptr) {
		cout << i << ": " << *temp << ((i - 'A' < MAXHAND - delCount) ? " (Kept)" : "") << endl;
		temp = temp->next;
		++i;
	}
	cout << endl;
	delCount = MAXHAND;
}

void Poker::displayDeck() const
{
	cout << endl << "Deck(" << deck_count << "):";
	bool spades[13] = {}, hearts[13] = {}, clubs[13] = {}, diamonds[13] = {};
	for (card * i = deck_top; i != nullptr; i = i->next)
		switch (i->suit) {
			case Spades: spades[i->num - 1] = true; break;
			case Hearts: hearts[i->num - 1] = true; break;
			case Clubs: clubs[i->num - 1] = true; break;
			case Diamonds: diamonds[i->num - 1] = true; break;
		}
	cout << endl << "Spades: "; for (int i = 0; i < 13; ++i) if (spades[i]) printValue(i + 1);
	cout << endl << "Hearts: "; for (int i = 0; i < 13; ++i) if (hearts[i]) printValue(i + 1);
	cout << endl << "Clubs: "; for (int i = 0; i < 13; ++i) if (clubs[i]) printValue(i + 1);
	cout << endl << "Diamonds: "; for (int i = 0; i < 13; ++i) if (diamonds[i]) printValue(i + 1);
	cout << endl << endl;
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

int Poker::winMoney()
{
	int win = 0;
	if (isRoyalFlush()) {
		win = 800;
		cout << "Royal Flush!" << endl;
	}
	else if (isStraightFlush()) {
		win = 50;
		cout << "Straight Flush!" << endl;
	}
	else if (is4OfAKind()) {
		win = 25;
		cout << "Four of a kind!" << endl;
	}
	else if (isFullHouse()) {
		win = 9;
		cout << "Full House!" << endl;
	}
	else if (isFlush()) {
		win = 6;
		cout << "Flush!" << endl;
	}
	else if (isStraight()) {
		win = 5;
		cout << "Straight!" << endl;
	}
	else if (is3OfAKind()) {
		win = 3;
		cout << "Three of a kind!" << endl;
	}
	else if (is2Pairs()) {
		win = 2;
		cout << "Two pairs!" << endl;
	}
	else if (isPairJackOrBetter()) {
		win = 1;
		cout << "Pair of Jacks or better!" << endl;
	}
	else {
		win = 0;
		cout << "No Poker hand scored. :(" << endl;
	}
	cout << "You earned $" << win << endl;

	// discard all hands and draw 5
	destroy(hands_top);
	draw(5);
	return win;
}

void Poker::getOption() 
{
	cin >> input;
	for (auto & c : input) c = toupper(c);
	while (!isOption(input)) {
		cout << sorry;
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
		return CHECK;
	}
	if (input == "ALL") {
		delCount = 0;
		return CHECK;
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
			cout << sorry;
			cin >> in;
			letter = toupper(in[0]);
		}
		card hand = remove(hands_top, letter - 'A', hand_count);
		cout << "Enter the value of the card in the deck to swap with: ";
		cin >> in;
		unsigned char num;
		while ((num = getValue(in)) == INVALID_VALUE) {
			cout << "Invalid value, please reenter: ";
			cin >> in;
		}
		cout << "Enter the suit (c,d,h,s) of the card in the deck to swap with: ";
		cin >> in;
		unsigned char suit;
		while ((suit = getSuit(in)) == INVALID_SUIT) {
			cout << "Invalid suit, please reenter: ";
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
			cout << card(suit, num) << " not found in the deck." << endl << endl;
			return 0;
		}
		// found, swaping
		cur_cd->suit = hand.suit;
		cur_cd->num = hand.num;
		insert(hands_top, TAIL, card(suit, num), hand_count);
		delCount = 1;
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
	cout << "You kept "<< MAXHAND - delCount <<" and drew " << delCount << " cards." << endl;
	return CHECK;
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
