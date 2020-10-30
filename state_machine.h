//Robin Romero, CS8, 12/11/18
#pragma once
#include<iostream>
#include "constants.h"

using namespace std;

//is the entire state machine used to compare each char in input
//and see which state it will go to
//====================================
//Declarations
//===================================
void init_table(int _table[][MAX_COL]);
void mark_success(int _table[][MAX_COL], int state);
void mark_fail(int _table[][MAX_COL], int state);
bool is_success(int _table[][MAX_COL], int state);
void mark_cells(int row, int _table[][MAX_COL], const char col[], int state);
void mark_cells(int row, int _table[][MAX_COL], int from, int to, int state);
void print_table(int _table[][MAX_COL]);
//void show_string(char s[], int _pos);

//=======================================
//Definitions
//=======================================
//fill all the cells with -1
inline void init_table(int _table[][MAX_COL]) {
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			_table[i][j] = -1;
		}
	}
}

//mark this state(row) with a 1 for success 
inline void mark_success(int _table[][MAX_COL], int state) {
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			_table[state][0] = 1;
		}
	}
}

//marks this state(row) with a fail 0
inline void mark_fail(int _table[][MAX_COL], int state) {
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			_table[state][0] = 0;
		}
	}
}

//returns true if its in a success state
//else returns false
inline bool is_success(int _table[][MAX_COL], int state) {
	if (_table[state][0] == 1) {
		return true;
	}
	else
		return false;
}

//marks states in cells if they abide by the rules
inline void mark_cells(int row, int _table[][MAX_COL], const char col[], int state) {

	const char* ptr = col;
	for (int j = 0; j < MAX_COL; j++) {
		if (j == *ptr) {
			_table[row][j] = state;
			ptr++;
		}
	}
}

inline void mark_cells(int row, int _table[][MAX_COL], int from, int to, int state) {
	for (int j = from; j <= to; j++) {
			_table[row][j] = state;
	}
}

inline void print_table(int _table[][MAX_COL])
{
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			cout << "[" << _table[i][j] << "]";
		}
		cout << endl;
	}
}


