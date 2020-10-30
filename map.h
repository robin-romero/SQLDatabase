//Robin Romero, CS8, 12/11/18
#pragma once
#include <iostream>
#include "../BPlusTree/BPlustree.h"

using namespace std;

template<typename K, typename V>
struct Pair {
	K key;
	V value;

	Pair(const K& k = K(), const V& v = V()) :key(k), value(v) {}

	friend ostream& operator <<(ostream& outs, const Pair<K, V>& print) {
		outs << "[" << print.key << "]-|" << print.value;
		return outs;
	}
	friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return lhs.key == rhs.key;
	}
	friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return lhs.key < rhs.key;
	}
	friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return lhs.key > rhs.key;
	}
	friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return lhs.key <= rhs.key;
	}
	friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return lhs.key >= rhs.key;
	}
	friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return Pair<K, V>(lhs.key, rhs.value);
	}
};

template<typename K, typename V>
class Map {
public:
	typedef BPlusTree<Pair<K, V> > map_base;
	class Iterator {
	public:
		friend class Map;
		Iterator(typename map_base::Iterator it) {
		}
		Iterator operator ++(int unused) {
			return _it++;
		}
		Iterator operator ++() {
			return _it++;
		}
		Pair<K, V> operator *() {
			return *_it;
		}
		friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
			return lhs._it == rhs._it;
		}
		friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
			return lhs._it != rhs._it;
		}
	private:
		typename map_base::Iterator _it;
	};

	Map();

	//  Iterators
	Iterator begin();
	Iterator end();

	//capacity
	int size() const { return key_count; }
	bool empty() const { return map.empty(); }

	//element
	V& operator [](const K& key);
	V& at(const K& key);
	const V& at(const K& key) const;

	//  Modifiers
	void insert(const K& k, const V& v);
	void insert(const K& k);
	void erase(const K& key);
	void clear();
	V get(const K& key);

	//Operations
	Iterator find(const K& key);
	bool contains(const Pair<K, V>& target) const;
	bool is_valid() { return map.is_valid(); }

	Iterator lower_bound(const K& key);
	Iterator upper_bound(const K& key);

	void print_it();

	friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me) {
		outs << print_me.map;
		return outs;
	}
private:
	int key_count;
	BPlusTree<Pair<K, V> > map;
};

template<typename K, typename V>
inline Map<K, V>::Map(){
	key_count = 0;
}

template<typename K, typename V>
typename inline Map<K,V>::Iterator Map<K, V>::begin(){
	return map.begin();
}

template<typename K, typename V>
typename inline Map<K, V>::Iterator Map<K, V>::end(){
	return map.end();
}

template<typename K, typename V>
inline V & Map<K, V>::operator[](const K & key){
	return map.get(Pair<K, V>(key, V())).value;
}

template<typename K, typename V>
inline V & Map<K, V>::at(const K & key){
	return map.get(Pair<K, V>(key, V())).value;
}

template<typename K, typename V>
inline const V & Map<K, V>::at(const K & key) const{
	return map.get(Pair<K, V>(key, V())).value;
}

template<typename K, typename V>
inline void Map<K, V>::insert(const K & k, const V & v){
	map.insert(Pair<K, V>(k, v));
	if (is_valid()) {
		key_count++;
	}
}

template<typename K, typename V>
inline void Map<K, V>::insert(const K & k){
	map.insert(Pair<K, V>(k));
	if (is_valid()) {
		key_count++;
	}
}

template<typename K, typename V>
inline void Map<K, V>::erase(const K & key){
	//need to implement in BPlusTree
}

template<typename K, typename V>
inline void Map<K, V>::clear(){
	map.clear_tree();
}

template<typename K, typename V>
inline V Map<K, V>::get(const K & key){
	return V(map.get(Pair<K, V>(key, V())).value);
}

template<typename K, typename V>
typename inline Map<K, V>::Iterator Map<K, V>::find(const K & key){
	return Iterator(map.find(key));
}

template<typename K, typename V>
inline bool Map<K, V>::contains(const Pair<K, V>& target) const{
	if (map.contains(Pair<K, V>(target.key))) {
		return true;
	}
	else {
		return false;
	}
}

template<typename K, typename V>
typename inline Map<K, V>::Iterator Map<K, V>::lower_bound(const K & key){
	return Iterator(map.lower_bound(key));
}

template<typename K, typename V>
typename inline Map<K, V>::Iterator Map<K, V>::upper_bound(const K & key){
	return Iterator(map.upper_bound(key));
}

template <typename K, typename V>
void Map<K, V>::print_it() {
	typename BPlusTree<Pair<K, V> >::Iterator it;
	
	for (it = map.begin(); it != map.end(); it++) {
		cout << *it;
		cout << "->";
	}
	cout << "|||";
	cout << endl;
}