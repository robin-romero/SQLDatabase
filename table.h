//Robin Romero, CS8, 12/11/18
#pragma once
#include "../Record/record.h"
#include "../Map/map.h"
#include "../MultiMap/mulitmap.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Table {
public:
	//constructors
	Table(string fname);
	Table(string fname, vector<string> v);

	void insert(vector<string> values);
	Table select_all();
	Table select(vector<string> fields_value, vector<string> conditions);

	long records();

	void print();

private:
	Map<string, MMap<string, long> > index;
	//saves the fields of inserted vector 
	vector<string> fields;
	//saves file name
	string file;
	//counter of how many records in the table
	long rec_count;
	//lower level functions
	vector<string> grab_all_record(long rec);
	vector<string> grab_record(vector<int> rd, long rec);
	void re_index();
	//differentiate values of the =, >, >= operator
	Map<string, int> field_num;
	void fill_map();
};
