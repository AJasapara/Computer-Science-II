#include "Sequence.h"
#include <iostream>

Sequence::Sequence(const Sequence &copy) {
	if (copy.empty()) // empty list
	{
		head = nullptr;
		tail = nullptr;
		seqSize = 0;
		return;
	}
	
	// starts with head and allocates new nodes to create a deep copy
	head = new Node(copy.head->value, nullptr, nullptr);
	Node *c = copy.head->next, *p = head;
	while (c != nullptr) {
		Node *temp = new Node(c->value, nullptr, p); // new node pointing prev to p, and next to nullptr
		p->next = temp; // setting p to point next at temp
		p = p->next;
		c = c->next;
	}
	tail = p;
	seqSize = copy.seqSize;
}

Sequence& Sequence::operator= (const Sequence &copy) {
	if (this != &copy) {
		Node* h = head; 		// delete previous list
		while (h != nullptr) {
			Node* temp = h->next;
			delete h;
			h = temp;
		}

		if (copy.empty()) 		// copy new one as seen above
		{
			head = nullptr;
			tail = nullptr;
			seqSize = 0;
			return *this;
		}
		head = new Node(copy.head->value, nullptr, nullptr);
		Node *c = copy.head->next, *p = head;
		while (c != nullptr) {
			Node *temp = new Node(c->value, nullptr, p);
			p->next = temp;
			p = p->next;
			c = c->next;
		}
		tail = p;
		seqSize = copy.seqSize;
	}
	return *this;
}

bool Sequence::insert(int pos, const ItemType& value) {
	int index = 0;
	Node* p = head;
	if (pos == 0 && p == nullptr) {   // empty list
		Node* temp = new Node(value, nullptr, nullptr);
		head = temp;
		tail = temp;
		seqSize++;
		return true;
	}
	while (p != nullptr) {
		if (pos == 0) { // front of list
			Node* temp = new Node(value,p,nullptr);
			p->prev = temp;
			head = temp;
			seqSize++;
			return true;
		}
		else if (pos-1 == index) {
			Node* temp;
			if (pos != seqSize && p->next != nullptr) { // middle of list
				temp = new Node(value, p->next, p);
				p->next->prev = temp; // set next Node to point prev at temp
			}
			else { // end of list
				temp = new Node(value, nullptr, p); // no next Node, set to nullptr
				tail = temp;
			}
			p->next = temp; // set p to point next at temp
			seqSize++;
			return true;
		}
		p = p->next;
		index++;
	}
	return false;
}

int Sequence::insert(const ItemType& value) {
	int index = 0;
	Node* p = head;
	if (p == nullptr) { // empty list
		Node* temp = new Node(value, nullptr, nullptr);
		head = temp;
		tail = temp;
		seqSize++;
		return index;
	}
	if (value <= p->value) { // front of list
		Node* temp = new Node(value, p, nullptr);
		p->prev = temp;
		head = temp;
		seqSize++;
		return index;
	}
	index++;
	while (p->next != nullptr) { // middle of list
		if (value <= p->next->value) {
			Node* temp = new Node(value, p->next, p);
			p->next = temp; // set p to point next at temp
			temp->next->prev = temp; // set next Node to point prev at temp
			seqSize++;
			return index;
		}
		p = p->next;
		index++;
	}
	// end of list
	Node* temp = new Node(value, nullptr, p);
	tail = temp;
	p->next = temp;
	seqSize++;
	return seqSize - 1;
}

bool Sequence::erase(int pos) {
	int index = 0;
	Node *p = head;
	if (p == nullptr) // empty list
		return true;
	if (pos == 0) { // delete first node
		head = p->next;
		head->prev = nullptr;
		delete p;
		seqSize--;
		return true;
	}
	while (p != nullptr) {
		if (pos == index) { 
			p->prev->next = p->next; // set prev Node to point to next Node, cutting out p
			seqSize--;
			if (p->next != nullptr) { // middle of list
				p->next->prev = p->prev;  // set next Node to point to prev Node, cutting out p
				delete p;
				return true;
			}
			else { // end of list
				tail = p->prev; // no next node, set tail to prev Node
				delete p;
				return true;
			}
		}
		p = p->next;
		index++;
	}
	return false;
}

int Sequence::remove(const ItemType& value) {
	int index = 0, count = 0;
	Node* p = head;
	while (p != nullptr) {
		if (p->value == value) { // removes all Nodes with value
			p = p->next;
			erase(index);
			count++;
		}
		else {
			p = p->next;
			index++;
		}
	}
	return count;
}

bool Sequence::get(int pos, ItemType& value) const {
	int index = 0;
	Node *p = head;
	while (p != nullptr) {
		if (pos == index) {
			value = p->value; // sets value to value at pos p
			return true;
		}
		p = p->next;
		index++;
	}
	return false;
}

bool Sequence::set(int pos, const ItemType& value) {
	int index = 0;
	Node *p = head;
	while (p != nullptr) {
		if (pos == index) {
			p->value = value; // puts value into Node's value at pos p
			return true;
		}
		p = p->next;
		index++;
	}
	return false;
}

int Sequence::find(const ItemType& value) const {
	int index = 0;
	Node *p = head;
	while (p != nullptr) {
		if (p->value == value) 
			return index; // returns position of value in Sequence
		p = p->next;
		index++;
	}
	return -1;
}

void Sequence::swap(Sequence& other) {
	Node *tempHead, *tempTail;
	tempHead = other.head; 	// swaps the head Nodes
	other.head = head;
	head = tempHead;
	tempTail = other.tail; 	// swaps the tail Nodes
	other.tail = tail;
	tail = tempTail;

	int tempSize = other.seqSize; 	// swaps the sizes
	other.seqSize = seqSize;
	seqSize = tempSize;
}

void Sequence::dump() const {
	Node* p = head;
	while (p != nullptr) { // print each value
		std::cerr << p->value << std::endl;
		p = p->next;
	}

	//print size
	std::cerr << seqSize << std::endl;
}

Sequence::~Sequence() {
	Node* p = head;
	while (p != nullptr) {
		Node* temp = p->next; // temp node to point to next one
		delete p;             // so this node can be deleted
		p = temp;
	}
}

int subsequence(const Sequence& seq1, const Sequence& seq2) {
	if (seq2.empty())
		return -1;
	ItemType temp, temp2;
	for (int i = 0; i < seq1.size(); i++) // runs through each element in seq1
		for (int j = i; (j - i) < seq2.size(); j++) { // runs through seq2.size() elements in seq1 and seq2
			if ((!seq1.get(j, temp)) || (!seq2.get(j-i, temp2)) || temp != temp2) // if either items are out of bounds
				break;															  // or values are not equals, breaks out of inner loop
			else if (j - i == seq2.size() - 1) // if end of seq2 is reached, and loop hasn't broken yet
				return i;					   // seq2 exists as subsequence at index i
		}
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
	Sequence sq1 = seq1, sq2 = seq2; // prevents aliasing by using copies of seq1 and seq2
	Sequence sq; // removes any previous elements by setting result to new Sequence
	result = sq;
	int maxsq = 1, size = sq2.size();
	if (sq1.size() < sq2.size()) { // determines which sq is larger
		maxsq = 2;
		size = sq1.size();
	}
	ItemType temp;
	int index = 0;
	for (int i = 0; i < size; i++, index+=2) { // loops through smaller size to interweave elements from both sequences
		sq1.get(i, temp);
		result.insert(index, temp);
		sq2.get(i, temp);
		result.insert(index+1, temp);
	}
	if (sq1.size() == sq2.size()) // end if sizes are equal
		return;
	if (maxsq == 1) // if sq1 is larger, add remainder of sq1 elements at end of result
		for (int i = size; i < sq1.size();i++, index++) {
			sq1.get(i, temp);
			result.insert(index, temp);
		}

	if (maxsq == 2) // if sq2 is larger, add remainder of sq1 elements at end of result
		for (int i = size; i < sq2.size();i++, index++) {
			sq2.get(i, temp);
			result.insert(index, temp);
		}
}