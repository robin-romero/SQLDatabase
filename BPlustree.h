//Robin Romero, CS8, 12/11/18
#pragma once
#include<iostream>
#include<iomanip>
#include<assert.h>
#include "bplustree_function.h"

using namespace std;

template<class T>
class BPlusTree {
public:
	//Iterator class
	class Iterator {
	public:
		friend class BPlusTree;
		Iterator(BPlusTree<T>* _it = NULL, int _key_ptr = 0) :it(_it), key_ptr(_key_ptr) {}

		T operator *() {
			assert(key_ptr < it->data_count);
			return it->data[key_ptr];
		}

		Iterator operator++(int un_used) {
			if (key_ptr == it->data_count - 1) {
				it = it->next;
				key_ptr = 0;
			}
			else {
				key_ptr++;
			}
			return *this;
		}

		Iterator& operator++() {
			if (key_ptr == it->data_count - 1) {
				it = it->next;
				key_ptr = 0;
			}
			else {
				key_ptr++;
			}
			return *this;
		}
		friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
			return (lhs.it == rhs.it);
		}

		friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
			return (lhs.it != rhs.it);
		}
		void print_Iterator() {
			
		}

		bool is_null() { return !it; }
	private:
		BPlusTree<T>* it;
		T* i;
		int key_ptr;
	};

	//==============================================PUBLIC===========================================//
	BPlusTree(bool dups = false);
	//================================BIG TREE THREE===================================
	BPlusTree(const BPlusTree<T>& other);
	~BPlusTree();
	BPlusTree<T>& operator =(const BPlusTree<T>& rhs);
	//=================================================================================

	void insert(const T& entry);					//insert entry into the tree
	void remove(const T& entry);					//remove the entry from the tree
	void clear_tree();								//clear this object(delete all ndoe)
	void copy_tree(const BPlusTree<T>& other);		//copy other into this object
	void copy_plus_tree(const BPlusTree<T>& other);	//copy the tree and the linked leaves
	bool contains(const T& entry) const;			//true if entry can be found in the array
	T& get(const T& entry);							//return a reference to entry in the tree
	Iterator find(const T& entry);					//return a pointer to this key, NULL if not found
	int size() const;								//count the number of elements in the tree
	bool empty() const { return data_count == 0; }	//true if the tree is empty(DONE)
	bool is_valid();								//returns true if conditions of a valid BPlusTree are met

	void print_tree(int level = 0, ostream& outs = cout)const;//works
	void print_tree_no_level(ostream & outs) const;

	//Iterator functions
	Iterator begin();
	Iterator end();
	Iterator lower_bound(const T& entry);
	Iterator upper_bound(const T& entry);

	void test();//prints the linked leaves

	friend ostream& operator <<(ostream& outs, const BPlusTree<T>& print) {//works(DONE)
		print.print_tree(0, outs);
		return outs;
	}
	//==================================================================================================//
	//======================================PRIVATE======================================================//
private:
	//rule size for data count and child count
	static const int MINIMUM = 1;
	static const int MAXIMUM = 2 * MINIMUM;

	BPlusTree* next;
	bool dups_ok;//true if duplicate keys may be inserted
	//node data and subsets
	int data_count;//number of data elements
	T data[MAXIMUM + 1];//hold the keys
	int child_count;//number of children
	BPlusTree* subset[MAXIMUM + 2];//suBPlusTrees

	int key_count;		//return number of keys in tree

	//true if this is a leaf node
	bool is_leaf() const {
		return child_count == 0;
	}
	//============================LOWER LEVEL FUNCTION==============================================//
	//insert element functions
	void loose_insert(const T& entry);	//allows MAXIMUM+1 data elements in the root
	void fix_excess(int i);				//fix excess of data elements in child i
	//remove element functions
	void loose_remove(const T& entry);	//allows MINIMUM-1 data elements in the root
	BPlusTree<T>* fix_shortage(int i);	//fix shortage of data elements in child i
	void remove_biggest(T& entry);		//remove the biggest child of this tree->entry
	void rotate_left(int i);			//transfer one element LEFT from child i
	void rotate_right(int i);			//transfer one element RIGHT from child i
	void transfer_left(int i);          //transfer one element LEFT from child i
	void transfer_right(int i);         //transfer one element RIGHT from child i
	BPlusTree<T>* merge_with_next_subset(int i);//merge subset i with subset i+1
	//Lower level functions 
	//T* find_ptr(const T& entry);		//return a ptr to this key, null if not found
	T& get_existing(const T& entry);
	void get_smallest(T& entry);		//entry = left most leaf
	void get_biggest(T& entry);			//entry = right most leaf
	BPlusTree<T>* get_smallest_node();	//returns the smallest node in the tree
	//BPlusTree<T>* lower_bound_find(const T& entry);

	bool data_rules();
	bool ascending();
	void copy_lower_level(const BPlusTree<T> & copy, BPlusTree<T>* &b);
	//=================================================================================================//
};

//========================================DEFINITIONS===================================================//
template<class T>//works
inline BPlusTree<T>::BPlusTree(bool dups) {
	data_count = 0;
	child_count = 0;
	dups_ok = dups;
	next = NULL;
	key_count = 0;
}

template<class T>//works
inline BPlusTree<T>::BPlusTree(const BPlusTree<T>& other) {
	copy_plus_tree(other);
}

template<class T>//works
inline BPlusTree<T>::~BPlusTree() {
	clear_tree();
}

template<class T>//works
inline BPlusTree<T>& BPlusTree<T>::operator=(const BPlusTree<T>& rhs) {
	clear_tree();
	copy_plus_tree(rhs);
	return *this;
}

template<class T>//works
inline void BPlusTree<T>::clear_tree() {
	const bool debug = false;
	if (is_leaf()) {
		data_count = 0;
		return;
	}
	for (int i = 0; i < child_count; i++) {
		subset[i]->clear_tree();
		subset[i]->data_count = 0;
		delete subset[i];
	}
	child_count = 0;
	data_count = 0;
}

template<class T>//works
//DONT USE COPY TREE, WORKS ONLY FOR BTREE
inline void BPlusTree<T>::copy_tree(const BPlusTree<T>& other) {
	const bool debug = false;
	if (debug)cout << "Going inside copy tree" << endl;
	copy_array(data, other.data, data_count, other.data_count);
	child_count = other.child_count;

	if (debug) {
		cout << "Other data count: " << other.data_count << endl;
		for (int i = 0; i < other.data_count; i++) {
			cout << "[" << other.data[i] << "]";
		}
		cout << endl;

		cout << "This data count: " << data_count << endl;
		for (int i = 0; i < data_count; i++) {
			cout << "[" << data[i] << "]";
		}
		cout << endl;
	}

	//check if other is a leaf, if leaf then return youre done
	if (other.is_leaf()) {
		return;
	}
	//case that other isnt a leaf need to go into the children and copy their data
	if (!other.is_leaf()) {
		for (int i = 0; i < other.child_count; i++) {
			subset[i] = new BPlusTree<T>;
			subset[i]->copy_tree(*other.subset[i]);
		}
	}
}

template<class T>//implemented for bplus tree
inline void BPlusTree<T>::copy_plus_tree(const BPlusTree<T>& other) {
	//copy the original data from other into this
	const bool debug = false;
	BPlusTree<T>* link_ptr = NULL;
	copy_lower_level(other, link_ptr);
}

template<class T>//copy function lower level function
inline void BPlusTree<T>::copy_lower_level(const BPlusTree<T>& copy, BPlusTree<T>*& b) {

	copy_array(data, copy.data, data_count, copy.data_count);
	child_count = copy.child_count;
	if (copy.is_leaf()) {
		return;
	}
	else {
		for (int i = copy.child_count - 1; i >= 0; --i) {
			subset[i] = new BPlusTree<T>;
			subset[i]->copy_lower_level(*copy.subset[i], b);
			if (subset[i]->is_leaf()) {
				subset[i]->next = b;
				b = subset[i];
			}
		}
	}
}

template<class T>//works
inline void BPlusTree<T>::insert(const T & entry) {
	const bool debug = false;
	//if excess in the root
	//create a new node, copy all the contents of this root into it,
		//      clear this root node,
		//      make the new node this root's only child (subset[0])
		//
		//Then, call fix_excess on this only subset (subset[0])
	if (debug) {
		cout << "Going inside insert" << endl;
		cout << "Inserting: " << entry << endl;
	}
	loose_insert(entry);
	//printing out the linked leaves before fix excess and connecting more leaves
	if (debug) {
		cout << "Linked leaf after loose insert and before checking if root is > MAX" << endl;
		test();
	}
	//if excess in the root (data count > max)
	if (data_count > MAXIMUM) {
		if (debug)cout << "Currently data count in after inserting is > MAX" << endl;
		//create a temp to copy all the data and children of original node
		BPlusTree* temp = new BPlusTree<T>;
		//copy this into temp
		temp->copy_plus_tree(*this);
		if (debug) {
			cout << "Linked leaf inside root > MAX after copying into temp" << endl;
			//leaves arent linked after copying*****
			temp->test();
		}
		//DO NOT CLEAR THE TREE HARDCODE CC = 0, DC = 0;
		child_count = 0;
		data_count = 0;
		if (debug) {
			cout << "child count after clear: " << child_count << endl;
			cout << "data count after clear: " << data_count << endl;
		}
		//child count increments to open the first child to point to temp ptr
		//subset[0] = new BPlusTree<T>;
		subset[0] = new BPlusTree<T>;
		subset[0] = temp;
		child_count++;
		if (debug) {
			cout << "THIS: " << endl;
			cout << *this << endl;
			cout << "Linked leaf inside root > MAX after pointing subset[0] to temp" << endl;
			subset[0]->test();
			cout << "This is temp that copied the original copy: " << endl;
			cout << *temp;
			cout << "This is subset[0] that points to temp: " << endl;
			cout << *subset[0];
			cout << "child count before excess call: " << child_count << endl;
		}
		//calling fix excess on the root if it exceeds max
		fix_excess(0);
		if (debug) {
			cout << "PRINTING LINK AFTER FIX EXCESS IN ROOT" << endl;
			test();
		}
	}
}

template<class T>//works
inline void BPlusTree<T>::loose_insert(const T & entry) {
	const bool debug = false;
	/*
	  int i = first_ge(data, data_count, entry);
	  bool found = (i<data_count && data[i] == entry);

	  three cases:
		a. found: deal with duplicates
		! found:
		b. leaf : insert entry in data at position i
		c. !leaf: subset[i]->loose_insert(entry)
				  fix_excess(i) if there is a need
		   |   found     |   !found        |
	 ------|-------------|-----------------|-------
	 leaf  |  a. Deal    | b: insert entry |
		   |     with    |    at data[i]   |
	 ------|  duplicates |-----------------|-------
		   |             | d: subset[i]->  |
	 !leaf |             |    loose_insert |
		   |             |    fix_excess(i)|
	 ------|-------------|-----------------|-------
   */
	if (debug)cout << "Inside loose insert" << endl;
	if (debug)cout << "Inserting: " << entry << endl;
	int i;

	i = get_ge(data, data_count, entry);

	bool found = (i < data_count && data[i] == entry);

	//three cases
	if (dups_ok == false && found) {
		if (debug)cout << "it was found" << endl;
		return;
	}
	if (!found) {
		if (debug)cout << "Was not found" << endl;
		if (is_leaf()) {
			if (debug)cout << "It is a leaf && inserting item directly into data" << endl;
			insert_item(data, i, data_count, entry);
		}
		if (!is_leaf()) {
			if (debug)cout << "It is not a leaf && recursively inside loose_insert" << endl;
			subset[i]->loose_insert(entry);
			if (debug) {
				cout << "sub data count: " << subset[i]->data_count << endl;
				cout << *subset[i];
			}
			if (subset[i]->data_count > MAXIMUM) {
				if (debug) {
					cout << "This is before fixing excess" << endl;
					cout << *this;
					cout << "child at: " << i << " is exceeding max" << endl;
				}
				fix_excess(i);
			}
		}
	}
}

template<class T>//works Thank the lord jesus christ
inline void BPlusTree<T>::fix_excess(int i) {
	/*
	this node's child i has one too many items: 3 steps:
	1. add a new subset at location i+1 of this node
	2. split subset[i] (both the subset array and the data array) and move half into
	 subset[i+1] (this is the subset we created in step 1.)
	3. detach the last data item of subset[i] and bring it and insert it into this node's data[]
	 //Note that this last step may cause this node to have too many items. This is OK. This will be
	 dealt with at the higher recursive level. (my parent will fix it!)
	*/
	const bool debug = false;
	if (debug)cout << "Going inside fix excess" << endl;
	if (debug)cout << "child count: " << child_count << endl;
	child_count++;
	//make a hole at i+1:
	for (int j = child_count - 1; j > i + 1; j--) {
		if(debug)cout << "FE: ^";
		subset[j] = subset[j - 1];
	}
	if (debug)cout << endl;
	T hold;
	//create a new BPlusTree at i + 1 in this current node
	subset[i + 1] = new BPlusTree<T>;
	if(debug)cout << "FE: created new node at i+1: " << i + 1 << endl;
	//pointing subset[i] to subset[i+1]
	if (subset[i]->is_leaf()) {
		if(debug)cout << "susbet[i] is leaf" << endl;
		subset[i + 1]->next = subset[i]->next;
		subset[i]->next = subset[i + 1];
	}
	//split subset[i] both subset and data array and move half into subset[i+1]
	if (debug)cout << "Splitting subset[i] that is exceeding max" << endl;
	if (debug)cout << *subset[i];

	split(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
	split(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);

	if (debug) {
		cout << "SUBSET[i] DATA COUNT: " << subset[i]->data_count << endl;
		cout << "SUBSET[i] CHILD COUNT: " << subset[i]->child_count << endl;
		cout << *subset[i];

		cout << "SUBSET[i + 1] DATA COUNT: " << subset[i + 1]->data_count << endl;
		cout << "SUBSET[i + 1] CHILD COUNT: " << subset[i + 1]->child_count << endl;
		cout << *subset[i + 1];
	}
	//detach last item of subset 

	//copy the last element of subset[i] into data
	//detach copied element from subset[i]
	detach_item(subset[i]->data, subset[i]->data_count, hold);
	if (debug) {
		cout << "SUBSET[i] DATA COUNT AFTER DETACH: " << subset[i]->data_count << endl;
		cout << *subset[i];
		cout << "Hold: " << hold << endl;
	}

	ordered_insert(data, data_count, hold);
	if (subset[i]->is_leaf()) {
		if (debug) {
			cout << "subset[i] is a leaf" << endl;
			cout << "inserting the detached item into subset[i+1] data" << endl;
		}
		ordered_insert(subset[i + 1]->data, subset[i + 1]->data_count, hold);
	}
	if (debug) {
		cout << "parent child count: " << child_count << endl;
		cout << "Parent data count: " << data_count << endl;
	}
}

template<class T>
inline void BPlusTree<T>::remove(const T & entry) {
	const bool debug = false;
	//Loose_remove the entry from this tree.
	//once you return from loose_remove, the root (this object) may have no data and only a single subset
	//now, the tree must shrink:
	//  point a temporary pointer (shrink_ptr) and point it to this root's only subset
	//  copy all the data and subsets of this subset into the root (through shrink_ptr)
	//  now, the root contains all the data and poiners of it's old child.
	//  now, simply delete shrink_ptr (blank out child), and the tree has shrunk by one level.
	//  Note, the root node of the tree will always be the same, it's the child node we delete
	if (debug)cout << "inside remove, Removing: " << entry << endl;
	loose_remove(entry);
	if (debug)cout << "Came back from loose remove" << endl;

	if (empty() && child_count == 1) {
		if (debug) {
			cout << "Root is empty but has child" << endl;
			cout << "Root child count: " << child_count << endl;
		}
		BPlusTree<T>* shrink = subset[0];
		if (debug) {
			cout << "This is shrink pointing to subset[0]:" << endl;
			cout << *shrink << endl;
		}
		copy_tree(*shrink);
		shrink->child_count = 0;
		delete shrink;
	}
}

template<class T>
inline void BPlusTree<T>::loose_remove(const T & entry) {
	const bool debug = false;
	/* four cases:
		   leaves:
				a. not found: there is nothing to do
				b. found    : just remove the target
		   non leaf:
				c. not found: subset[i]->loose_remove, fix_shortage(i)
				d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
					(No More remove_biggest)

			 |   !found               |   found                 |
	   ------|------------------------|-------------------------|-------
	   leaf  |  a: nothing            | b: delete               |
			 |     to do              |    target               |
	   ------|------------------------|-------------------------|-------
	   !leaf | c:                     | d: B_PLUS_TREE          |
			 |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
			 |  fix_shortage(i)       | fix_shortage(i+1) [...] |
	   ------|------------------------|-------------------------|-------


	 */
	if (debug)cout << "Inside loose remove" << endl;
	int i;
	T hold;
	i = get_ge(data, data_count, entry);
	if (debug)cout << "Current index: " << i << endl;

	bool found = (i < data_count && data[i] == entry);

	if (is_leaf()) {
		if (!found) {
			return;
		}
		else {
			delete_item(data, i, data_count, hold);
		}
	}
	else {
		if (!found) {
			subset[i]->loose_remove(entry);
			if (subset[i]->data_count < MINIMUM) {
				fix_shortage(i);
			}
		}
		else {
			subset[i + 1]->loose_remove(entry);
			if (subset[i + 1]->data_count < MINIMUM) {
				fix_shortage(i + 1);
			}
		}
	}
}

template<class T>
inline BPlusTree<T>* BPlusTree<T>::fix_shortage(int i) {
	const bool debug = false;
	/*
	* fix shortage in subtree i:
	* if child i+1 has more than MINIMUM,
	*          (borrow from right) transfer / rotate left(i+1)
	* elif child i-1 has more than MINIMUM,
	*          (borrow from left) transfer /rotate right(i-1)
	* elif there is a left sibling,
	*          merge with prev child: merge(i-1)
	* else
	*          merge with next (right) child: merge(i)
	*
	*
	* returns a pointer to the fixed_subset
	*/
	if (debug)cout << "Fix Shortage is called at: " << i << endl;
	if (i + 1 < child_count && subset[i + 1]->data_count > MINIMUM) {
		if (debug)cout << "Rotating left case" << endl;
		rotate_left(i);
	}
	else if (i > 0 && subset[i - 1]->data_count > MINIMUM) {
		if (debug)cout << "Rotating right case" << endl;
		rotate_right(i);
	}
	else if (i < child_count && i + 1 < child_count) {
		if (debug)cout << "merging normal case" << endl;
		merge_with_next_subset(i);
		if (debug) {
			cout << "Returned from merge" << endl;
			cout << *this;
			cout << "child count: " << child_count << endl;
		}
	}
	else {
		if (debug)cout << "merging if right is empty case" << endl;
		merge_with_next_subset(i - 1);
	}
}

template<class T>
inline void BPlusTree<T>::remove_biggest(T & entry) {
	const bool debug = false;
	// Keep looking in the last subtree (recursive)
	//  until you get to a leaf.
	// Then, detach the last (biggest) data item
	//
	// after the recursive call, fix shortage.
	if (debug)cout << "Inside remove biggest" << endl;
	if (!is_leaf()) {
		if (debug)cout << "Being recursively called inside remove biggest" << endl;
		subset[child_count - 1]->remove_biggest(entry);
		if (subset[child_count - 1]->data_count < MINIMUM) {
			fix_shortage(child_count - 1);
		}
	}
	if (is_leaf()) {
		detach_item(data, data_count, entry);
		if (debug)cout << "detached item: " << entry << endl;
		return;
	}
}

template<class T>
inline void BPlusTree<T>::rotate_left(int i) {
	const bool debug = false;
	T hold;
	BPlusTree<T>* temp;

	if (!subset[i]->is_leaf()) {
		delete_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp);
		if (debug)cout << "Temp: " << *temp << endl;
		attach_item(subset[i]->subset, subset[i]->child_count, temp);
		if (debug)cout << "Subset[i]: " << *subset[i] << endl;
	}
	delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, hold);
	if (debug)cout << "deleting item: " << hold << " from subset[" << i + 1 << "]->data" << endl;
	ordered_insert(data, data_count, hold);
	if (debug)cout << "inserted into data" << endl;
	if (debug) {
		cout << *this << endl;
	}
	delete_item(data, i, data_count, hold);
	if (debug)cout << "deleting item from data at " << i << ", item: " << hold << endl;
	ordered_insert(subset[i]->data, subset[i]->data_count, hold);
	if (debug)cout << "inserting: " << hold << " into subset[" << i << "]" << endl;
}

template<class T>
inline void BPlusTree<T>::rotate_right(int i) {
	const bool debug = false;
	T hold;
	BPlusTree<T>* temp;

	if (!subset[i]->is_leaf()) {
		detach_item(subset[i - 1]->subset, subset[i - 1]->child_count, temp);
		if (debug)cout << "Temp: " << *temp << endl;
		insert_item(subset[i]->subset, 0, subset[i]->child_count, temp);
		if (debug)cout << "Subset[i-1]: " << *subset[i] << endl;
	}
	if (debug) {
		cout << "called before deleting from data" << endl;
		cout << *this << endl;
	}
	detach_item(subset[i - 1]->data, subset[i - 1]->data_count, hold);
	if (debug)cout << "detaching item: " << hold << " from subset[" << i - 1 << "]->data" << endl;
	ordered_insert(data, data_count, hold);
	if (debug) {
		cout << *this << endl;
	}
	detach_item(data, data_count, hold);
	if (debug)cout << "deleting item from data at " << i << ", item: " << hold << endl;
	ordered_insert(subset[i]->data, subset[i]->data_count, hold);
	if (debug)cout << "inserting: " << hold << " into subset[" << i << "]" << endl;
}

template<class T>
inline BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i) {
	const bool debug = false;
	/*
	* ----------------------------------------------------------------------
	*  Merge subset[i] with subset [i+1] REMOVE data[i];
	*  non leaf: same as BTree
	*  leaves  : delete but do not bring down data[i]
	* ----------------------------------------------------------------------
	*
	*   1. remove data[i] from this object
	*   2. if not a leaf, append it to child[i]->data:
	*   3. Move all data items from subset[i+1]->data to right of subset[i]->data
	*   4. Move all subset pointers from subset[i+1]->subset to
	*          right of subset[i]->subset
	*   5. delete subset[i+1] (store in a temp ptr)
	*   6. if a leaf, point subset[i]->next to temp_ptr->next
	*   6. delete temp ptr
	*
	*/
	if (!subset[i]->is_leaf()) {
		T hold;
		delete_item(data, i, data_count, hold);
		if (debug)cout << "deleted item from data: " << hold << endl;
		attach_item(subset[i]->data, subset[i]->data_count, hold);
		if (debug) {
			cout << "Attached item at subset[i] data: " << endl;
			cout << *subset[i];
			cout << endl << endl;
		}
		merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
		if (debug) {
			cout << "merged item at subset[" << i << "] data: " << endl;
			for (int i = 0; i < subset[i]->data_count; i++) {
				cout << "[" << subset[i]->data[i] << "]";
			}
			cout << endl;
		}
		merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
		if (debug) {
			cout << "Subset[i] after merging: " << endl;
			cout << *subset[i];
			cout << "subset[i] child count: " << subset[i]->child_count << endl;
		}
		//delete subset[i+1]
		//AREA WHERE IM CRASHING
		BPlusTree<T>* temp;
		delete_item(subset, i + 1, child_count, temp);
		delete temp;
	}
	else {
		T temp;
		delete_item(data, i, data_count, temp);
		if (debug)cout << "deleted item from data: " << temp << endl;
		merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
		if (debug) {
			cout << "merged item at subset[" << i << "] data: " << endl;
			for (int i = 0; i < subset[i]->data_count; i++) {
				cout << "[" << subset[i]->data[i] << "]";
			}
			cout << endl;
		}
	}
	return subset[i];
}

template<class T>//works Freaking only thing that works
inline void BPlusTree<T>::print_tree(int level, ostream & outs) const {
	const bool debug = false;
	if (!is_leaf()) {
		subset[child_count - 1]->print_tree(level + 1, outs);
		for (int i = data_count - 1; i >= 0; i--) {
			outs << setw(level * 4) << data[i] << endl;
			subset[i]->print_tree(level + 1, outs);
		}
	}
	if (is_leaf()) {
		for (int i = data_count - 1; i >= 0; i--) {
			outs << setw(level * 4) << data[i] << endl;
		}
		return;
	}
}

template<class T>//works Freaking only thing that works
inline void BPlusTree<T>::print_tree_no_level(ostream & outs) const {
	const bool debug = false;
	if (!is_leaf()) {
		subset[child_count - 1]->print_tree_no_level(outs);
		for (int i = data_count - 1; i >= 0; i--) {
			outs << "[" << data[i] << "]" << endl;
			subset[i]->print_tree_no_level(outs);
		}
	}
	if (is_leaf()) {
		for (int i = data_count - 1; i >= 0; i--) {
			outs << "[" << data[i] << "]" << endl;
		}
		return;
	}
}

template<class T>//Works
typename inline BPlusTree<T>::Iterator BPlusTree<T>::find(const T & entry) {
	BPlusTree<T>* target;
	int i = get_ge(data, data_count, entry);
	bool found = (i < data_count && data[i] == entry);

	if (found) {
		if (is_leaf()) {
			target = this;
			return Iterator(target, i);
		}
		else {
			subset[i + 1]->find(entry);
		}
	}
	else if (!found && is_leaf()) {
		return Iterator(NULL);
	}
	else {
		return subset[i]->find(entry);
	}
}

template<class T>
inline int BPlusTree<T>::size() const{
	//returns the number of elements in the tree
	return key_count;
}

template<class T>//works
inline bool BPlusTree<T>::contains(const T & entry) const {
	const bool debug = false;
	/*
	searching for entry if exists in tree
	1. Make local variable i, equal to first index such that data[i] is not less than target
	ift there is no such index, then set i equal to data_count, indicates that all of entries
	are less than the target
	2.
	if(found entry at data[i]
		return true;
	else if(root has no children && not in data[i])
		return false;
	else
		return subset[i]->contains(entry);
	*/
	int i;
	i = get_ge(data, data_count, entry);

	bool found = (i < data_count && data[i] == entry);

	if (found) {
		if (is_leaf()) {
			return true;
		}
		else {
			subset[i + 1]->contains(entry);
		}
	}
	else if (is_leaf() && !found) {
		return false;
	}
	else {
		return subset[i]->contains(entry);
	}
}

template<class T>//works
inline T & BPlusTree<T>::get(const T & entry) {
	const bool debug = false;
	//returns a reference to entry in the tree

	if (!contains(entry)) {
		insert(entry);
	}
	return get_existing(entry);
}

//template<class T>//works
//inline T* BPlusTree<T>::find_ptr(const T & entry){
//	const bool debug = false;
//	T* target;
//	int i = get_ge(data, data_count, entry);
//	bool found = (i < data_count && data[i] == entry);
//
//	if (found) {
//		if (is_leaf()) {
//			target = &data[i];
//			return target;
//		}
//		else {
//			subset[i + 1]->find_ptr(entry);
//		}
//	}
//	else if (!found && is_leaf()) {
//		return NULL;
//	}
//	else {
//		return subset[i]->find_ptr(entry);
//	}
//}

template<class T>//works
inline T & BPlusTree<T>::get_existing(const T & entry) {
	const bool debug = false;
	int i;
	i = get_ge(data, data_count, entry);

	bool found = (i < data_count && data[i] == entry);

	if (is_leaf()) {
		if (found) {
			return data[i];
		}
		else {
			assert(found);
		}
	}
	else {
		if (found)
			return subset[i + 1]->get(entry);
		else {
			return subset[i]->get(entry);
		}
	}
}

template<class T>
inline void BPlusTree<T>::get_smallest(T & entry) {
	const bool debug = false;
	//replace entry with the left_most leaf of this tree
	// Keep looking in the first subtree (recursive)
	//  until you get to a leaf.
	// Then, replace entry with data[0] of the leaf node
	if (is_leaf()) {
		entry = data[0];
	}
	if (!is_leaf()) {
		subset[0]->get_smallest(entry);
	}
}

template<class T>
inline void BPlusTree<T>::get_biggest(T & entry){
	//if not the last subset recursively call subset[childcount-1]
	if (is_leaf()) {
		entry = data[data_count - 1];
	}
	if (!is_leaf()) {
		subset[child_count - 1]->get_biggest(entry);
	}
}

template<class T>//seems to work correctly
inline BPlusTree<T>* BPlusTree<T>::get_smallest_node() {
	//return the smallest node in your tree
	//recursively call the first subset until it is a leaf and point to that node
	//return ptr that is pointing to the smallest node
	const bool debug = true;
	BPlusTree<T>* target;
	if (is_leaf()) {
		//leaf is found
		target = this;
		return target;
	}
	else {
		//recursively go into subset[0] until leaf
		subset[0]->get_smallest_node();
	}
}

//template<class T>
//inline BPlusTree<T>* BPlusTree<T>::lower_bound_find(const T& entry){
//	BPlusTree<T>* target;
//
//	int i = get_ge(data, data_count, entry);
//	bool found = (i < data_count && data[i] == entry);
//
//	if (found) {
//		if (is_leaf()) {
//			target = this;
//			return target;
//		}
//		else {
//			subset[i + 1]->lower_bound_find(entry);
//		}
//	}
//	else if (!found && is_leaf()) {
//		target = this;
//		return target->next;
//	}
//	else {
//		return subset[i]->lower_bound_find(entry);
//	}
//}

template<class T>
inline bool BPlusTree<T>::is_valid(){
	const bool debug = false;
	if(debug)cout << "is valid called" << endl;
	if (data_rules()) {
		if (ascending()) {
			if (is_leaf()) {
				return true;
			}
		}
	}
	else {
		if(debug)cout << "False was called when checking rules: " << endl;
		return false;
	}
	if (!is_leaf()) {
		for (int i = 0; i < child_count; i++) {
			subset[i]->is_valid();
		}
	}
}

template<class T>
inline bool BPlusTree<T>::data_rules() {
	const bool debug = false;
	if ((data_count >= MINIMUM) && (data_count <= MAXIMUM) && (child_count >= 0) && (child_count <= MAXIMUM + 1)) {
		return true;
	}
	else {
		return false;
	}
}

template<class T>
inline bool BPlusTree<T>::ascending() {
	const bool debug = false;
	for (int i = 0; i < data_count; i++) {
		if (data[i] > data[i + 1])
			return false;
	}
	return true;
}

template<class T>
typename inline BPlusTree<T>::Iterator BPlusTree<T>::begin(){
	return Iterator(get_smallest_node());
}

template<class T>
typename inline BPlusTree<T>::Iterator BPlusTree<T>::end(){
	return Iterator(NULL);
}

template<class T>
typename inline BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T & entry){
	const bool debug = false;
	BPlusTree<T>* target;

	int i = get_ge(data, data_count, entry);
	bool found = (i < data_count && data[i] == entry);

	if (found) {
		if (is_leaf()) {
			target = this;
			return Iterator(target, i);
		}
		else {
			return subset[i + 1]->lower_bound(entry);
		}
	}
	else if (!found && is_leaf()) {
		target = this;
	
		if (i >= target->data_count) {
			return Iterator(target->next, 0);
		}
		else {
			return Iterator(target, i);
		}
	}
	else {
		return subset[i]->lower_bound(entry);
	}
}

template<class T>
typename inline BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T & entry){
	BPlusTree<T>* target;

	int i = get_ge(data, data_count, entry);
	bool found = (i < data_count && data[i] == entry);

	if (found) {
		if (is_leaf()) {
			target = this;
			if (i + 1 >= target->data_count) {
				return Iterator(target->next, 0);
			}
			else {
				return Iterator(target, i+1);
			}
		}
		else {
			return subset[i + 1]->upper_bound(entry);
		}
	}
	else if (!found && is_leaf()) {
		target = this;	

		if (i+1 >= target->data_count) {
			//worry about the edge case since it reaches null and crashes
			return Iterator(target->next, 0);
		}
		else {
			return Iterator(target, i+1);
		}
	}
	else {
		return subset[i]->upper_bound(entry);
	}
}

template<class T>
inline void BPlusTree<T>::test(){
	BPlusTree<T>* start = get_smallest_node();
	while (start != NULL) {
		for (int i = 0; i < start->data_count; i++) {
			cout << "[" << start->data[i] << "]";
		}
		cout << "->";
		start = start->next;
	}
	cout << "|||" << endl;
}
