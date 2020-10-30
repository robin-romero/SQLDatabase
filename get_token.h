#pragma once
#include "constants.h"
#include "state_machine.h"
#include "STokenizer.h"
#include <iostream>

using namespace std;

bool debug = true;

bool get_token(string s, int& _pos, int _table[][MAX_COL], int start_state);

inline bool get_token(string s, int& _pos, int _table[][MAX_COL], int start_state) {
	//position ptr will start at the beginning of the string
	//used to move to next char after the previous char is used to send current state
	//look at notes written on how to do the for loop
	//assigned start_state to the current state so that it can be auto initialized to correct state

	//CHANGE THE VARIABLES IN THIS FUNCTION TO MATCH THE HEADER AND WORK
	bool success = false;
	string success_token;
	int i = _pos;
	while (start_state != -1 && i <= size) {
		success_token += _buffer[i];
		if (debug) {
			cout << "Success_Token: " << success_token << endl;
			cout << "Previous state: " << start_state << endl;
		}
		start_state = _table[start_state][_buffer[i]];
		if (debug) {
			cout << "i: " << i << ", char: " << _buffer[i] << ", Current state: " << start_state << endl;
		}
		if (is_success(_table, start_state)) {
			token = success_token;
			if (_pos < size) {
				_pos = i + 1;
			}
			else {
				_pos = i;
			}
			if (debug) {
				cout << "Token_update: " << token << endl;
			}
			success = true;
		}
		i++;
	}
	if (success) {
		if (debug)
			cout << "This is current position: " << _pos << " at: " << _buffer[_pos] << endl;
		return true;
	}
	else {
		return false;
	}
}

