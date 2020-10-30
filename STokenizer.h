//Robin Romero, CS8, 12/11/18
#pragma once
#ifndef STOKENIZER_H
#define STOKENIZER_H
#include <iostream>
#include <cstring>
#include <string>
#include "constants.h"
#include "Token.h"
#include "state_machine.h"

using namespace std;

class STokenizer
{
public:
	//CTORs
	STokenizer();
	//Initializes the char array into buffer by copying evrything 
	//in str into buffer
	STokenizer(char str[]);
	//true if no more tokens to be read
	bool done();
	//true if more tokens to be read
	bool more();
	//extracts one token, stk -> token
	friend STokenizer& operator >>(STokenizer& s, Token& t);
	//sets a new string as input just like constructor
	void set_string(const char str[]);

private:
	//creates table for all token we recognize
	void make_table(int _table[][MAX_COL]);
	//extracts the lingest string that match acceptable tokens
	//bool get_token(int start_state);
	//input string
	char _buffer[MAX_BUFFER];
	//current position
	int _pos;
	static int _table[MAX_ROW][MAX_COL];
	bool get_token(int start_state, string& token);
	int size;
};

#endif // !STOKENIZER_H

