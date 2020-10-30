//Robin Romero, CS8, 12/11/18
#include "../../../#include/Table/table.h"
#include "../../../#include/SQL/SQL.h"
#include "../../../#include/Parse/parse.h"
#include <iostream>
#include <algorithm>
#include<fstream>

using namespace std;

void sql_test();

int main(int argc, char *argv[])
{
	cout << endl << endl << endl << "================================" << endl;

	sql_test();

	cout << endl << endl << endl << "================================" << endl;
	return 0;
}

void sql_test() {

	ifstream batchfile("select.txt");
	string line;
	string incoming;
	int num = 0;

	cout << "TESTING FROM BATCH" << endl;
	cout << "================================" << endl;

	if (batchfile)
	{
		while (getline(batchfile, line)) {
			if (line != "") {
				incoming = line;
				cout << "incoming line:" << endl;
				cout << "[" << num << "]" <<  line << endl << endl;
				num++;
				SQL s(incoming);
				Parse p(s.string_command());
				s.manage_table(p.add_multimap());

				cout << "================================" << endl << endl;
			}
		}

		batchfile.close();
	}
	else {
		cout << "invalid file";
	}


	cout << endl;

	cout << "test complete" << endl;
	cout << "================================" << endl;

	//string command;
	//char input;

	//cout << "<CREATE> : {  create <TABLE_NAME> fields <FIELD_NAME>[, <FIELD_NAME>...]  }" << endl;
	//cout << "<INSERT> : { insert <INTO> <TABLE_NAME> values <VALUE>[, <VALUE>...]      }" << endl;
	//cout << "<SELECT> : {  select <*| FIELD_NAME>[, , FIELD_NAME > ...]";
	//cout << "from <TABLE_NAME>" << endl;
	//cout << "where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>" << endl;
	//cout << "[<LOGICAL_OPERATOR>";
	//cout << "<FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>...]" << endl;
	//cout << "}" << endl;

	//cout << "<VALUE> : A string of alphanumeric characters, or a string of alphanumeric";
	//cout << " characters and spaces enclosed by double quotation marks :";
	//cout << "Jean Luise, Finch, 1923" << endl;
	//cout << " < RELATIONAL OPERATOR > : [= | > | < | >= | <=]" << endl;

	//do {
	//	cout << "Enter your command: " << endl;
	//	getline(cin, command);
	//	SQL s(command);
	//	Parse parse(s.string_command());
	//	s.manage_table(parse.add_multimap());

	//	cout << "Would you like to enter another command(y/n):";
	//	cin >> input;
	//} while (input != 'n' || input != 'N');
}