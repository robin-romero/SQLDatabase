//Robin Romero, CS8, 12/11/18
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);


class Record {
public:
	Record() {
		std::memset(record, NULL, sizeof(record));
		recno = -1;
	}

	Record(vector<string> v) {
		std::memset(record, NULL, sizeof(record));
		recno = -1;
		for (int i = 0; i < v.size(); i++) {
			strcpy_s(record[i], v[i].c_str());
		}
	}
	long write(fstream& outs);
	long read(fstream& ins, long recno);
	vector<string> select_all_record();
	vector<string> select_record(vector<int> r);

	void set(vector<string> new_value);
	long rec_count(fstream& outs);//works almost identical to write
	friend ostream& operator<<(ostream& outs,
		const Record& r);
private:
	static const int ROW = 50;
	static const int COL = 100;
	int recno;
	char record[ROW][COL];
	long count_file();
};