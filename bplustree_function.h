//Robin Romero, CS8, 11/5/2018
#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<assert.h>

using namespace std;

//template functions
//================================ARRAY UTILITY FUNCTIONS===============================//
//----------------------------------DECLARATIONS----------------------------------------//
template<class T>//works
T maximal(const T& a, const T& b);									//return the larger of the two items
template<class T>//works
void swap_value(T& a, T& b);										//swap the two items
template<class T>//works
int index_of_max(T data[], int& n);									//return index of the largest item in data
template<class T>
void ordered_insert(T data[], int& n, T entry);						//insert entry into the sorted array
template<class T>//works
int get_ge(const T data[], int n, const T& entry);					//return the first element in data that is not less than entry
template<class T>//works
void attach_item(T data[], int& n, const T& entry);					//append entry to the right of data
template<class T>	//works
void insert_item(T data[], int i, int& n, T entry);					//insert entry at index i on data
template<class T>//works
void detach_item(T data[], int& n, T& entry);						//remove the last element in data and place it in entry
template<class T>//works
void delete_item(T data[], int i, int& n, T& entry);				//delete item at index i and place it in entry
template<class T>//works
void merge(T data1[], int& n1, T data2[], int& n2);					//append data2 at the right of data1
template<class T>//works
void split(T data1[], int& n1, T data2[], int& n2);					//move n/2 elements from the right of data1 and move to data2
template<class T>//works
void copy_array(T dest[], const T src[], int& dest_size, int src_size);	//copy src[] into dest[]
template<class T>//works
void print_array(const T data[], int n, int pos = 0);				//print array data
template<class T>//works
bool is_gt(const T data[], int n, const T& item);					//item > all data[i]
template<class T>//works
bool is_lt(const T data[], int n, const T& item);					//item is <= all data[i]
//---------------------------------------------------------------------------------------------------------//
//-------------- Vector Extra operators: ---------------------------------------------------------------//
template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list
template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme
template<typename T>
bool vector_dup(const vector<T> vect, const T item);
//-----------------------------------------------------------------------------------------------------------//

//------------------------------------DEFINITIONS---------------------------------------------------------------//
template<class T>
inline T maximal(const T & a, const T & b){
	if (a >= b)
		return T(a);
	else
		return T(b);
}

template<class T>
inline void swap_value(T & a, T & b){
	T temp = a;
	a = b;
	b = temp;
}

template<class T>
inline int index_of_max(T data[], int & n){
	int max = 0;
	for (int i = 0; i < n; i++) {
		if (data[i] > data[max]) {
			max = i;
		}
	}
	return max;
}

template<class T>
inline void ordered_insert(T data[], int & n, T entry){
	//insert entry into sorted array
	//need to fix ordered sort which is screwing everything up
	int i;
	if (is_gt(data, n, entry)) {
		i = n;
	}
	else {
		i = get_ge(data, n, entry);
	}
	insert_item(data, i, n, entry);
}

template<class T>
inline int get_ge(const T data[], int n, const T & entry){
	for (int i = 0; i < n; i++) {
		if (data[i] >= entry) {
			return i;
		}
	}
	return n;
}

template<class T>
inline void attach_item(T data[], int & n, const T & entry){
	data[n] = entry;
	n++;
}

template<class T>
inline void insert_item(T data[], int i, int & n, T entry){
	n++;
	for (int j = n - 1; j > i; j--) {
		data[j] = data[j - 1];
	}
	data[i] = entry;
	
}

template<class T>
inline void detach_item(T data[], int & n, T & entry){
	entry = data[n - 1];
	n--;
}

template<class T>
inline void delete_item(T data[], int i, int & n, T & entry) {
	entry = data[i];
	for (int j = i; j < n; j++) {
		data[j] = data[j + 1];
	}
	n--;
}

template<class T>
inline void merge(T data1[], int & n1, T data2[], int & n2){
	int size = n1 + n2;
	int j = 0;
	for (int i = n1; i < size; i++) {
		data1[i] = data2[j];
		n2--;
		j++;
	}
	n1 = size;
}

template<class T>
inline void split(T data1[], int & n1, T data2[], int & n2){
	const bool debug = false;
	//move n1/2 elements FROM THE RIGHT
	int half = n1 / 2;
	int temp = n1 - half;
	if (debug) {
		cout << "This is half: " << half << endl;
		cout << "This is temp: " << temp << endl;
	}
	for (int i = 0; i < half; i++) {
		data2[n2++] = data1[temp++];
		/*n2++;
		temp++;*/
	}
	n1 -= half;
}

template<class T>
inline void copy_array(T dest[], const T src[], int & dest_size, int src_size) {
	for (int i = 0; i < src_size; i++) {
		dest[i] = src[i];
	}
	dest_size = src_size;
}

template<class T>
inline void print_array(const T data[], int n, int pos){
	for (int i = 0; i < n; i++) {
		cout << "[" << data[i] << "]->";
	}
	cout << "|||" << endl;
}

template<class T>
inline bool is_gt(const T data[], int n, const T & item) {
	//if item is > all elements return true
	T max = item;
	for (int i = 0; i < n; i++) {
		if (max < data[i])
			max = data[i];
	}
	if (max == item) {
		return true;
	}
	else {
		return false;
	}
}

template<class T>
inline bool is_lt(const T data[], int n, const T & item) {
	T max = item;
	for (int i = 0; i < n; i++) {
		if (max > data[i])
			max = data[i];
	}
	if (max == item) {
		return true;
	}
	else {
		return false;
	}
}

template<typename T>
inline ostream & operator<<(ostream & outs, const vector<T>& list)
{
	for (int i = 0; i < list.size(); i++) {
		outs << "[" << list[i] << "]";
	}
	return outs;
}

template<typename T>
inline vector<T>& operator+=(vector<T>& list, const T & addme){
	if (!vector_dup(list, addme)) {
		list.push_back(addme);
		return list;
	}
}

template<typename T>
inline bool vector_dup(const vector<T> vect, const T item){
	return count(begin(vect), end(vect), item) != 0;

}

//--------------------------------------------------------------------------------------------------------------//
//========================================================================================================//