//Robin Romero, CS8, 12/11/18
#pragma once
#include "../BPlusTree/BPlustree.h"
#include <vector>
#include <iostream>

using namespace std;

//===========================================STRUCT MPAIR================================================//
template <typename K, typename V>
struct MPair {
	K key;
	vector<V> value_list;

	//--------------------------------------------------------------------------------
	/*      MPair CTORs:
	 *  must have these CTORs:
	 *  - default CTOR / CTOR with a key and no value: this will create an empty vector
	 *  - CTOR with a key AND a value: pushes the value into the value_list
	 *  _ CTOR with a key and a vector of values (to replace value_list)
	 */
	 //--------------------------------------------------------------------------------
	MPair(const K& k = K()) : key(k) {}
	MPair(const K& k, const V& v);
	MPair(const K& k, const vector<V>& vlist);
	//--------------------------------------------------------------------------------

	//You'll need to overlod << for your vector:
	friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me) {
		outs << "{" << print_me.key << "}-|";
		for (int i = 0; i < print_me.value_list.size(); i++) {
			outs << print_me.value_list[i] << "|";
		}
		outs << "]";
		return outs;
	}

	friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
		return lhs.key == rhs.key;
	}
	friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
		return lhs.key < rhs.key;
	}
	friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
		return lhs.key > rhs.key;
	}
	friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
		return lhs.key <= rhs.key;
	}
	friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
		return lhs.key >= rhs.key;
	}
	friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
		return MPair<K, V>(lhs.key, rhs.value_list);
	}
};
//=======================================================================================================//

//===========================================CLASS MULTIMAP================================================//
template <typename K, typename V>
class MMap
{
public:
	typedef BPlusTree<MPair<K, V> > map_base;
	class Iterator {
	public:
		friend class MMap;
		Iterator(typename map_base::Iterator it) {

		}
		Iterator operator ++(int unused) {
			//remember working with vectors ++ will jump from one vector to another
			//may want to be able to go inside the vector with iterator
			return _it++;
		}
		Iterator operator ++() {
			return _it++;
		}
		MPair<K, V> operator *() {
			return *_it;
		}
		friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
			return lhs._it == rhs._it;
		}
		friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
			return lhs._it != rhs._it;
		}
	private:
		typename map_base::Iterator _it;
	};

	MMap();

	//  Iterators
	Iterator begin();
	Iterator end();
	Iterator l_bound(const K& key);
	Iterator u_bound(const K& key);

	//  Capacity
	int size() const { return key_count; }
	bool empty() const { return mmap.empty(); }

	//  Element Access
	const vector<V>& operator[](const K& key) const;
	vector<V>& operator[](const K& key);

	//  Modifiers
	void insert(const K& k);
	void insert(const K& k, const V& v);
	void erase(const K& key);
	void clear();

	//  Operations:
	bool contains(const K& key) const;
	vector<V> &get(const K& key);

	vector<long> lower_bound(const K& key);
	vector<long> lowerbound(const K& key);
	vector<long> upper_bound(const K& key);
	vector<long> upperbound(const K& key);

	int count(const K& key);
	// I have not writtent these yet, but we will need them:
	//    int count(const K& key);
	//    lower_bound
	//    upper_bound
	//    equal_range:

	bool is_valid();

	void it_print();

	friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me) {
		outs << print_me.mmap;
		return outs;
	}

private:
	BPlusTree<MPair<K, V> > mmap;
	int key_count;
};

template<typename K, typename V>
inline MMap<K, V>::MMap(){
	key_count = 0;
}

template<typename K, typename V>
typename inline MMap<K, V>::Iterator MMap<K, V>::begin(){
	return mmap.begin();
}

template<typename K, typename V>
typename inline MMap<K, V>::Iterator MMap<K, V>::end(){
	return mmap.begin();
}

template<typename K, typename V>
typename inline MMap<K, V>::Iterator MMap<K, V>::l_bound(const K & key){
	return mmap.lower_bound(key);
}

template<typename K, typename V>
typename inline MMap<K, V>::Iterator MMap<K, V>::u_bound(const K & key){
	return mmap.upper_bound(key);
}

template<typename K, typename V>
inline const vector<V>& MMap<K, V>::operator[](const K & key) const{
	if (contains(key)) {
		return get(key);
	}
	else {
		insert(key, V());
		if (is_valid()) {
			key_count++;
		}
		return get(key);
	}
}

template<typename K, typename V>
inline vector<V>& MMap<K, V>::operator[](const K & key){
	if (contains(key)) {
		return get(key);
	}
	else {
		insert(key);
		return get(key);
	}
}

template<typename K, typename V>
inline void MMap<K, V>::insert(const K & k){
	mmap.insert(MPair<K, V>(k));
	if (is_valid()) {
		key_count++;
	}
}

template<typename K, typename V>
inline void MMap<K, V>::insert(const K & k, const V & v){
	mmap.insert(MPair<K, V>(k, v));
	if (is_valid()) {
		key_count++;
	}
}

template<typename K, typename V>
inline void MMap<K, V>::clear(){
	mmap.clear_tree();
}

template<typename K, typename V>
inline bool MMap<K, V>::contains(const K & key) const{
	if (mmap.contains(MPair<K, V>(key, V()))) {
		return true;
	}
	else {
		return false;
	}
}

template<typename K, typename V>
inline vector<V>& MMap<K, V>::get(const K & key)
{
	return mmap.get(MPair<K, V>(key, V())).value_list;
}

template<typename K, typename V>
inline bool MMap<K, V>::is_valid()
{
	if (mmap.is_valid()) {
		return true;
	}
	else {
		return false;
	}
}

template<typename K, typename V>
inline MPair<K, V>::MPair(const K & k, const V & v){
	key = k;
	value_list += v;
}

template<typename K, typename V>
inline MPair<K, V>::MPair(const K & k, const vector<V>& vlist){
	key = k;
	value_list = vlist;
}

template <typename K, typename V>
void MMap<K, V>::it_print() {
	typename BPlusTree<MPair<K, V> >::Iterator it;

	for (it = mmap.begin(); it != mmap.end(); it++) {
		cout << *it;
		cout << "->";
	}
	cout << "|||";
	cout << endl;
}

template<typename K, typename V>
inline vector<long> MMap<K, V>::lower_bound(const K & key) {
	typename BPlusTree<MPair<K, V> >::Iterator it;
	MPair<K, V> pair;
	vector<long> records;
	for (it = mmap.begin(); it != mmap.upper_bound(key); it++) {
		pair = *it;

		for (int i = 0; i < pair.value_list.size(); i++) {
			records.push_back(pair.value_list.at(i));
		}
	}
	return records;
}

template<typename K, typename V>
inline vector<long> MMap<K, V>::lowerbound(const K & key){
	typename BPlusTree<MPair<K, V> >::Iterator it;
	MPair<K, V> pair;
	vector<long> records;
	for (it = mmap.begin(); it != mmap.lower_bound(key); it++) {
		pair = *it;

		for (int i = 0; i < pair.value_list.size(); i++) {
			records.push_back(pair.value_list.at(i));
		}
	}
	return records;
}

template<typename K, typename V>
inline vector<long> MMap<K, V>::upper_bound(const K & key) {
	typename BPlusTree<MPair<K, V> >::Iterator it;
	MPair<K, V> pair;
	vector<long> records;
	for (it = mmap.lower_bound(key); it != mmap.end(); it++) {
		pair = *it;

		for (int i = 0; i < pair.value_list.size(); i++) {
			records.push_back(pair.value_list.at(i));
		}
	}
	return records;
}

template<typename K, typename V>
inline vector<long> MMap<K, V>::upperbound(const K & key){
	typename BPlusTree<MPair<K, V> >::Iterator it;
	MPair<K, V> pair;
	vector<long> records;
	for (it = mmap.upper_bound(key); it != mmap.end(); it++) {
		pair = *it;

		for (int i = 0; i < pair.value_list.size(); i++) {
			records.push_back(pair.value_list.at(i));
		}
	}
	return records;
}
