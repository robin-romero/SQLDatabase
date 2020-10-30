//Robin Romero, CS8, 12/11/18
#include "record.h"

using namespace std;

long Record::write(fstream & outs){
	//write to the end of the file.
	long pos = 0;
	outs.seekg(pos, ios_base::end);
	outs.write(&record[0][0], sizeof(record));
	pos = outs.tellp();
	return pos / sizeof(record);
}

long Record::read(fstream & ins, long recno){
	long pos = recno * sizeof(record);
	ins.seekg(pos, ios_base::beg);

	ins.read(&record[0][0], sizeof(record));
	return ins.gcount();
}

vector<string> Record::select_all_record(){
	//pushes all the values in record into vector
	vector<string> vect;
	int num = count_file();
	for (int i = 0; i < num; i++) {
		vect.push_back(record[i]);
	}
	return vect;
}

vector<string> Record::select_record(vector<int> r){
	//selects a specified record sent in from vector r
	vector<string> vect;
	for (int i = 0; i < r.size(); i++) {
		vect.push_back(record[r[i]]);
	}
	return vect;
}

void Record::set(vector<string> new_value){
	*this = Record(new_value);
}

long Record::rec_count(fstream & outs){
	//write to the end of the file.
	long pos = 0;
	outs.seekg(pos, ios_base::end);
	pos = outs.tellp();
	return pos / sizeof(record);
}

long Record::count_file(){
	int count = 0;
	for (int i = 0; i < ROW; i++) {
		if (record[i][0] != '\0') {
			count++;
		}
	}
	return count;
}

ostream & operator<<(ostream & outs, const Record & r){
	outs << setw(10);
	for (int i = 0; i < r.ROW; i++) {
		outs << left << r.record[i] << " ";
	}
	return outs;
}

bool file_exists(const char filename[]) {
	const bool debug = false;
	fstream ff;
	ff.open(filename,
		std::fstream::in | std::fstream::binary);
	if (ff.fail()) {
		if (debug) cout << "file_exists: File does NOT exist: " << filename << endl;
		return false;
	}
	if (debug) cout << "file_exists: File DOES exist: " << filename << endl;
	return true;
}

void open_fileRW(fstream& f, const char filename[]) throw(char*) {
	const bool debug = false;
	//openning a nonexistent file for in|out|app causes a fail()
	//  so, if the file does not exist, create it by openning it for
	//  output:
	if (!file_exists(filename)) {
		f.open(filename, 
			std::fstream::out | std::fstream::binary);
		if (f.fail()) {
			cout << "file open (RW) failed: with out|" << filename << "]" << endl;
			throw("file RW failed  ");
		}
		else {
			if (debug) cout << "open_fileRW: file created successfully: " << filename << endl;
		}
	}
	else {
		f.open(filename,
			std::fstream::in | std::fstream::out | std::fstream::binary);
		if (f.fail()) {
			cout << "file open (RW) failed. [" << filename << "]" << endl;
			throw("file failed to open.");
		}
	}
}
void open_fileW(fstream& f, const char filename[]) {
	f.open(filename,
		std::fstream::out | std::fstream::binary);
	if (f.fail()) {
		cout << "file open failed: " << filename << endl;
		throw("file failed to open.");
	}
}
