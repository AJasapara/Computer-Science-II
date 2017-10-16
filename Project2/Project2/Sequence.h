#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <string>

typedef std::string ItemType;

class Sequence
{
public:
	Sequence() {
		head = nullptr;
		tail = nullptr;
		seqSize = 0;
	};
	// Constructs empty default Sequence

	Sequence(const Sequence &copy);
	// Creates a copy of the Sequence using newly allocated memory

	Sequence& operator= (const Sequence &copy);
	// Deallocated current memory and creates a copy of the Sequence using newly allocated memory

	bool empty() const {
		return head == nullptr;
	}
	// Determines if sequence is empty

	int size() const {
		return seqSize;
	}
	// Returns the size of the sequence

	bool insert(int pos, const ItemType& value);
	// Insert value into the sequence so that it becomes the item at
	// position pos.  The original item at position pos and those that
	// follow it end up at positions one higher than they were at before.
	// Return true if 0 <= pos <= size() and the value could be
	// inserted. Otherwise, leave the sequence unchanged and return false.
	// Notice that if pos is equal to size(), the value is inserted at the
	// end.

	int insert(const ItemType& value);
	// Let p be the smallest integer such that value <= the item at
	// position p in the sequence; if no such item exists (i.e.,
	// value > all items in the sequence), let p be size().  Insert
	// value into the sequence so that it becomes the item at position
	// p.  The original item at position p and those that follow it end
	// up at positions one higher than before.  Return p if the value
	// was actually inserted. 

	bool erase(int pos);
	// If 0 <= pos < size(), remove the item at position pos from
	// the sequence (so that all items that followed this item end up at
	// positions one lower than they were at before), and return true.
	// Otherwise, leave the sequence unchanged and return false.

	int remove(const ItemType& value);
	// Erase all items from the sequence that == value.  Return the
	// number of items removed (which will be 0 if no item == value).

	bool get(int pos, ItemType& value) const;
	// Copy into value the item at position pos
	// in the sequence and return true.  Otherwise, leave value unchanged
	// and return false.

	bool set(int pos, const ItemType& value);
	// Replace the item at position pos in the
	// sequence with value and return true.  Otherwise, leave the sequence
	// unchanged and return false.

	int find(const ItemType& value) const;
	// Let p be the smallest integer such that value == the item at
	// position p in the sequence; if no such item exists, let p be -1.
	// Return p.

	void swap(Sequence& other);
	// Exchange the contents of this sequence with the other one.

	void dump() const;
	// Prints to cerr the contents of the sequence and the size.

	~Sequence();
	// Deletes the dynamically allocated elements of the Sequence.

private:
	struct Node {
	public:
		ItemType value;
		Node* next;
		Node* prev;
		Node(ItemType val, Node* n, Node* p) {
			value = val;
			next = n;
			prev = p;
		}
	};
	// The Node struct for a doubly Linked List

	Node *head, *tail;
	int seqSize;
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
// Checks if seq1 contains seq2 at any point in the sequence.

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);
// Interweaves seq1 and seq2 until one finishes, at which point
// the rest of the other sequence is inserted at the end.


#endif