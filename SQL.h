//Robin Romero, CS8, 12/11/18
#pragma once
#include "../StateMachine/STokenizer.h"
#include "../StateMachine/constants.h"
#include "../StateMachine/state_machine.h"
#include "../StateMachine/Token.h"
#include "../Table/table.h"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class SQL {
public:
	SQL(string com);
	vector<string> string_command();
	void manage_table(MMap<string, string> commands);

private:
	STokenizer stk;
	string pass_command;
	MMap<string, string> ptree;
};