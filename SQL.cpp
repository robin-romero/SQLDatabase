//Robin Romero, CS8, 12/11/18
#include "SQL.h"

SQL::SQL(string com){
	pass_command = com;
}

vector<string> SQL::string_command(){
	const bool debug = false;
	vector<string> v;
	string space = " ";
	string no_space = "";
	string two_space = "  ";
	string tab = "\t";
	stk.set_string(pass_command.c_str());
	Token t;
	stk >> t;
	while (t.token_str() == " " || t.token_str() == "\t" || t.token_str() == "\n") {
		t = Token();
		stk >> t;
	}
	v.push_back(t.token_str());

	while (stk.more()) {
		//process token here...
		t = Token();
		stk >> t;
		while (t.token_str() == " " || t.token_str() == "\t" || t.token_str() == "\n") {
			t = Token();
			stk >> t;
		}
		v.push_back(t.token_str());
	}

	v.erase(remove(v.begin(), v.end(), space), v.end());
	v.erase(remove(v.begin(), v.end(), no_space), v.end());
	v.erase(remove(v.begin(), v.end(), two_space), v.end());
	v.erase(remove(v.begin(), v.end(), tab), v.end());

	if (debug) {
		cout << "inside sql" << endl;
		for (int i = 0; i < v.size(); i++) {
			cout << "Token: " << v.at(i) << endl;
		}
	}
	
	return v;
}

void SQL::manage_table(MMap<string, string> commands){
	const bool debug = false;
	ptree = commands;

	if (debug) {
		cout << "Ptree" << endl;
		cout << ptree << endl;
	}

	//COMMANDS
	//commands.insert("command");
	//commands.insert("fields");
	//commands.insert("table");
	//commands.insert("condition");

	//based on the ptree command, go to different cases and insert into a file or create a file or select from a file
	if (ptree.get("command").at(0) == "insert") {
		Table temp(ptree.get("table").at(0));
		temp.insert(ptree["fields"]);
	}
	if (ptree.get("command").at(0) == "make" || ptree.get("command").at(0) == "create") {
		Table temp(ptree.get("table").at(0), ptree["fields"]);
	}
	if (ptree.get("command").at(0) == "select") {
		Table temp(ptree.get("table").at(0));
		if (ptree.get("fields").at(0) == "*") {
			if (ptree.get("condition").empty()) {
				temp.select_all();
			}
			else {
				temp.select(ptree["fields"], ptree["condition"]);
			}
		}
		else {
			temp.select(ptree["fields"], ptree["condition"]);
		}
	}
}
