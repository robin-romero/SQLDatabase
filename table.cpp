//Robin Romero, CS8, 12/11/18
#include "table.h"

using namespace std;

//changes string to upper case
//transform(su.begin(), su.end(), su.begin(), ::toupper); 

Table::Table(string fname){
	//string file name inserted creates a file of that name
	file = fname + ".bin";
	fstream f;
	if (file_exists(file.c_str())) {
		fields = grab_all_record(0);
		fill_map();
		rec_count = records();
		if (rec_count > 0 && index.size() < 1) {
			re_index();
		}
	}
	else {
		open_fileRW(f, file.c_str());
		f.close();
		rec_count = 0;
	}
}

Table::Table(string fname, vector<string> v){
	//creates file name with inserted string and inserts vectors that are inserted in
	//constructor
	fstream f;
	fields = v;
	file = fname + ".bin";

	if (file_exists(file.c_str())) {
		remove(file.c_str());
	}
	Record r(fields);
	long rec_num;
	open_fileRW(f, file.c_str());
	rec_num = r.write(f);
	f.close();
	rec_count = 1;

	for (int i = 0; i < fields.size(); i++) {
		index.insert(fields.at(i));
	}
}

void Table::insert(vector<string> values){
	fstream f;
	Record r(values);
	long rec_num;
	open_fileRW(f, file.c_str());
	rec_num = r.write(f);
	f.close();
	
	for (int i = 0; i < values.size(); i++) {
		index[fields.at(i)][values.at(i)] += rec_num - 1;
	}
}

Table Table::select_all(){
	//reads all the records inside the file
	//create a temp table 
	//[
	//read every record in the file and store into a vector
	//temp_table.insert(vector)
	//] loop
	//return temp table
	vector<string> vect = grab_all_record(0);
	Table temp("select_all", vect);
	for (int i = 1; i < rec_count; i++) {
		temp.insert(grab_all_record(i));
	}

	temp.print();

	return temp;
}

Table Table::select(vector<string> fields_value, vector<string> conditions){
	//creates a temp table with fields passed and inserting all the values into 
	//that temp table
	//look for the fields passed in from args
	vector<int> field_number;
	//saves all the records with desired values
	vector<long> record_num;

	if (fields_value.at(0) == "*") {
		for (int i = 0; i < fields.size(); i++) {
			field_number.push_back(field_num[fields.at(i)]);
		}
	}
	else {
		for (int i = 0; i < fields_value.size(); i++) {
			field_number.push_back(field_num[fields_value.at(i)]);
		}
	}

	//grab the first record from the original file and create a new file with returned specified fields
	vector<string> vect = grab_record(field_number, 0);
	Table temp("temp_select", vect);
	//condition allows user to pick records with a specified value
	if (conditions.at(1) == "=") {
		record_num = index[conditions.at(0)][conditions.at(2)];
		for (int i = 0; i < record_num.size(); i++) {
			temp.insert(grab_record(field_number, record_num.at(i)));
		}
	}
	//records are less than specified value
	if (conditions.at(1) == "<") {
		record_num = index[conditions.at(0)].lowerbound(conditions.at(2));
		for (int i = 0; i < record_num.size(); i++) {
			temp.insert(grab_record(field_number, record_num.at(i)));
		}
	}
	//records are greater than specified value
	if (conditions.at(1) == ">") {
		record_num = index[conditions.at(0)].upperbound(conditions.at(2));
		for (int i = 0; i < record_num.size(); i++) {
			temp.insert(grab_record(field_number, record_num.at(i)));
		}
	}
	if (conditions.at(1) == "<=") {
		record_num = index[conditions.at(0)].lower_bound(conditions.at(2));
		for (int i = 0; i < record_num.size(); i++) {
			temp.insert(grab_record(field_number, record_num.at(i)));
		}
	}
	if (conditions.at(1) == ">=") {
		record_num = index[conditions.at(0)].upper_bound(conditions.at(2));
		for (int i = 0; i < record_num.size(); i++) {
			temp.insert(grab_record(field_number, record_num.at(i)));
		}
	}

	temp.print();

	return temp;
}

long Table::records(){
	fstream f;
	Record r;
	open_fileRW(f, file.c_str());
	rec_count = r.rec_count(f);
	f.close();
	return rec_count;
}

void Table::print(){
	fstream f;
	Record r;
	open_fileRW(f, file.c_str());
	for (int i = 0; i < records(); i++) {
		r.read(f, i);
		cout << "Record[" << i << "]: " << r << endl;
	}
	f.close();

	/*for (int i = 0; i < fields.size(); i++) {
		index[fields[i]].it_print();
	}*/
}

vector<string> Table::grab_all_record(long rec){
	fstream f;
	Record r;
	//grabs all the records and puts into a vector at specified record number
	vector<string> v;
	open_fileRW(f, file.c_str());
	r.read(f, rec);
	f.close();
	v = r.select_all_record();
	return v;
}

vector<string> Table::grab_record(vector<int> rd, long rec){
	fstream f;
	Record r;
	vector<string> v;
	open_fileRW(f, file.c_str());
	r.read(f, rec);
	f.close();
	v = r.select_record(rd);
	return v;
}

void Table::re_index(){
	vector<string> tempvec;
	fields = grab_all_record(0);

	for (long i = 0; i < rec_count; i++) {
		tempvec = grab_all_record(i);
		for (int j = 0; j < fields.size(); j++) {
			index[fields.at(j)][tempvec.at(j)] += i;
		}
	}
}

void Table::fill_map(){
	//fills map with fields from record 0 of file
	vector<string> fill = grab_all_record(0);
	for (int i = 0; i < fill.size(); i++) {
		field_num[fill.at(i)] += i;
	}
}
