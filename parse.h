//Robin Romero, CS8, 12/11/18
#pragma once
#include <iostream>
#include <string>
#include "../SQL/SQL.h"
#include "../StateMachine/state_machine.h"
#include "../Map/map.h"
#include "../MultiMap/mulitmap.h"
#include "../StateMachine/constants.h"

using namespace std;

class Parse {
public:
	Parse(vector<string> vect_command);//takes in a vector to compare to state machine
	void make_table(int parse_table[][MAX_COL]);
	MMap<string, string> add_multimap();//each significant state pushes value into mmap to be sent to sql

private:
	Map<string, int> keyword;
	MMap<string, string> commands;
	vector<string> tokens;
	static int parse_table[MAX_ROW][MAX_COL];
	int select_start = 0;
	int create_start = 19;
	int make_start = 20;
	int insert_start = 30;
};