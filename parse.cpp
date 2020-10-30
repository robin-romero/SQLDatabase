//Robin Romero, CS8, 12/11/18
#include "parse.h"

Parse::Parse(vector<string> vect_command){
	const bool debug = false;
	//create the table 
	make_table(parse_table);

	tokens = vect_command;

	if (debug) {
		cout << "inside parse" << endl;
		for (int i = 0; i < tokens.size(); i++) {
			cout << "Tokens: " << tokens.at(i) << endl;
		}
	}
	//mmap of commands to send to sql
	commands.insert("command");
	commands.insert("fields");
	commands.insert("table");
	commands.insert("condition");

}

int Parse::parse_table[MAX_ROW][MAX_COL];

void Parse::make_table(int parse_table[][MAX_COL]) {
	//initialize the table and create keywords that send each token to different states in the state machine
	init_table(parse_table);

	keyword["string"] += 1;
	keyword["select"] += 2;
	keyword["from"] += 3;
	keyword["where"] += 4;
	keyword["make"] += 5;
	keyword["table"] += 6;
	keyword["fields"] += 7;
	keyword["insert"] += 8;
	keyword["into"] += 9;
	keyword["values"] += 10;
	keyword[","] += 11;
	keyword["*"] += 12;
	keyword["create"] += 13;
	keyword["="] += 14;
	keyword[">"] += 15;
	keyword["<"] += 16;
	keyword[">="] += 17;
	keyword["<="] += 18;

	//select from table
	//==================================================================
	mark_cells(0, parse_table, keyword["select"], keyword["select"], 1);//command
	mark_cells(1, parse_table, keyword["string"], keyword["string"], 2);//fields
	mark_cells(2, parse_table, keyword[","], keyword[","], 3);
	mark_cells(3, parse_table, keyword["string"], keyword["string"], 2);//fields
	mark_cells(2, parse_table, keyword["from"], keyword["from"], 4);
	mark_cells(1, parse_table, keyword["*"], keyword["*"], 6);//fields
	mark_cells(6, parse_table, keyword["from"], keyword["from"], 4);
	mark_cells(4, parse_table, keyword["string"], keyword["string"], 5);//success/table/filename
	mark_cells(5, parse_table, keyword["where"], keyword["where"], 7);
	mark_cells(7, parse_table, keyword["string"], keyword["string"], 8);//fields
	mark_cells(8, parse_table, keyword["="], keyword["="], 9);//relational operator
	mark_cells(8, parse_table, keyword[">"], keyword[">"], 10);//relational operator
	mark_cells(8, parse_table, keyword["<"], keyword["<"], 11);//relational operator
	mark_cells(8, parse_table, keyword[">="], keyword[">="], 12);//relational operator
	mark_cells(8, parse_table, keyword["<="], keyword["<="], 13);//relational operator
	mark_cells(9, parse_table, keyword["string"], keyword["string"], 14);//value
	mark_cells(10, parse_table, keyword["string"], keyword["string"], 14);//value
	mark_cells(11, parse_table, keyword["string"], keyword["string"], 14);//value
	mark_cells(12, parse_table, keyword["string"], keyword["string"], 14);//value
	mark_cells(13, parse_table, keyword["string"], keyword["string"], 14);//value

	mark_fail(parse_table, 0);
	mark_fail(parse_table, 1);
	mark_fail(parse_table, 2);
	mark_fail(parse_table, 3);
	mark_fail(parse_table, 4);
	mark_fail(parse_table, 6);
	mark_fail(parse_table, 7);
	mark_fail(parse_table, 8);
	mark_fail(parse_table, 9);
	mark_fail(parse_table, 10);
	mark_fail(parse_table, 11);
	mark_fail(parse_table, 12);
	mark_fail(parse_table, 13);

	mark_success(parse_table, 5);
	mark_success(parse_table, 14);
	//==================================================================

	//create/make from table
	//==================================================================
	mark_cells(19, parse_table, keyword["create"], keyword["create"], 21);//command
	mark_cells(20, parse_table, keyword["make"], keyword["make"], 21);//command
	mark_cells(21, parse_table, keyword["table"], keyword["table"], 22);
	mark_cells(22, parse_table, keyword["string"], keyword["string"], 23);//table/filename
	mark_cells(23, parse_table, keyword["fields"], keyword["fields"], 24);
	mark_cells(24, parse_table, keyword["string"], keyword["string"], 25);//fields
	mark_cells(25, parse_table, keyword[","], keyword[","], 26);
	mark_cells(26, parse_table, keyword["string"], keyword["string"], 25);//success/fields

	mark_fail(parse_table, 19);
	mark_fail(parse_table, 20);
	mark_fail(parse_table, 21);
	mark_fail(parse_table, 22);
	mark_fail(parse_table, 23);
	mark_fail(parse_table, 24);
	mark_fail(parse_table, 26);

	mark_success(parse_table, 25);
	//==================================================================

	//insert into table
	//==================================================================
	mark_cells(30, parse_table, keyword["insert"], keyword["insert"], 31);//command
	mark_cells(31, parse_table, keyword["into"], keyword["into"], 32);
	mark_cells(32, parse_table, keyword["string"], keyword["string"], 33);//filename/table
	mark_cells(33, parse_table, keyword["values"], keyword["values"], 34);
	mark_cells(34, parse_table, keyword["string"], keyword["string"], 35);//fields
	mark_cells(35, parse_table, keyword[","], keyword[","], 36);
	mark_cells(36, parse_table, keyword["string"], keyword["string"], 35);//success/fields

	mark_fail(parse_table, 30);
	mark_fail(parse_table, 31);
	mark_fail(parse_table, 32);
	mark_fail(parse_table, 33);
	mark_fail(parse_table, 34);
	mark_fail(parse_table, 36);

	mark_success(parse_table, 35);
	//==================================================================
}

MMap<string, string> Parse::add_multimap(){
	const bool debug = false;
	//loop through the vector to check each command with the state machine
	int state;
	//start at a specific state based on the check of the first command
	if (tokens.at(0) == "select") {
		state = select_start;
		if(debug)cout << "Current starting state: " << state << endl;
	}
	if (tokens.at(0) == "make") {
		state = make_start;
		if (debug)cout << "Current starting state: " << state << endl;
	}
	if (tokens.at(0) == "create") {
		state = create_start;
		if (debug)cout << "Current starting state: " << state << endl;
	}
	if (tokens.at(0) == "insert") {
		state = insert_start;
		if (debug)cout << "Current starting state: " << state << endl;
	}

	for (int i = 0; i < tokens.size(); i++) {
		if (keyword.contains(tokens.at(i))) {
			if (debug)cout << "Current token: " << tokens.at(i) << endl;
			if (debug)cout << "This is the current state: " << state << endl;
			state = parse_table[state][keyword[tokens.at(i)]];
			if (debug)cout << "This is new state: " << state << endl;
			switch (state) {
			//===============command=============================
			case 1: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["command"] += tokens.at(i);
				break;
			}//insert into field since its a key word
			case 6: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["fields"] += tokens.at(i);
				break;
			}
			case 9: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["condition"] += tokens.at(i);
				break;
			}
			case 10: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["condition"] += tokens.at(i);
				break;
			}
			case 11: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["condition"] += tokens.at(i);
				break;
			}
			case 12: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["condition"] += tokens.at(i);
				break;
			}
			case 13: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["condition"] += tokens.at(i);
				break;
			}
			case 21: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["command"] += tokens.at(i);
				break;
			}
			case 31: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["command"] += tokens.at(i);
				break;
			}
			//================================================
			}
		}
		else {
			state = parse_table[state][keyword["string"]];
			switch (state) {
				//if non of the tokens were found in keywords, they are string values
			//=============fields===========================
			case 2: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["fields"] += tokens.at(i);
				break;
			}
			case 8: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["condition"] += tokens.at(i);
				break;
			}
			case 14: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["condition"] += tokens.at(i);
				break;
			}
			case 25: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["fields"] += tokens.at(i);
				break;
			}
			case 35: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["fields"] += tokens.at(i);
				break;
			}
			//================================================
			//=============table=============================
			case 5: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["table"] += tokens.at(i);
				break;
			}
			case 23: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["table"] += tokens.at(i);
				break;
			}
			case 33: {
				if (debug)cout << tokens.at(i) << " brought me to state: " << state << endl;
				commands["table"] += tokens.at(i);
				break;
			}
			//=================================================
			}
		}
	}

	return commands;
}