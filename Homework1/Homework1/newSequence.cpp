#include "newSequence.h"

Sequence::Sequence() {
	seq = new ItemType[DEFAULT_MAX_ITEMS];
}

Sequence::Sequence(int length) {
	seq = new ItemType[length];
	seqMaxSize = length;
}

Sequence::Sequence(const Sequence& copy) {
	seq = new ItemType[copy.seqMaxSize];
	seqMaxSize = copy.seqMaxSize;
	seqSize = copy.seqSize;
	ItemType temp;
	for (int i = 0; i < seqSize; i++) {
		copy.get(i, temp);
		set(i, temp);
	}
}

Sequence& Sequence::operator= (const Sequence& copy) {
	if (this != &copy) {
		delete [] seq;
		seq = new ItemType[copy.seqMaxSize];
		seqMaxSize = copy.seqMaxSize;
		seqSize = copy.seqSize;
		ItemType temp;
		for (int i = 0; i < seqSize; i++) {
			copy.get(i, temp);
			set(i, temp);
		}
	}
	return *this;
}

bool Sequence::insert(int pos, const ItemType& value) {
	if (pos < 0 || pos > seqSize || seqSize == seqMaxSize)
		return false;
	for (int i = seqSize; i > pos; i--)
		seq[i] = seq[i - 1];
	seq[pos] = value;
	seqSize++;
	return true;
}

int Sequence::insert(const ItemType& value) {
	for (int i = 0; i < seqSize; i++)
		if (value <= seq[i])
			if (insert(i, value))
				return i;
	if (insert(seqSize, value))
		return seqSize;
	return -1;

}
bool Sequence::erase(int pos) {
	if (pos < 0 || pos >= seqSize)
		return false;
	for (int i = pos; i < seqSize - 1;i++)
		seq[i] = seq[i + 1];
	seqSize--;
	return true;

}

int Sequence::remove(const ItemType& value) {
	int count = 0;
	for (int i = 0; i < seqSize; i++)
		if (seq[i] == value) {
			erase(i);
			i--;
			count++;
		}
	return count;
}

bool Sequence::get(int pos, ItemType& value) const {
	if (pos < 0 || pos >= seqSize)
		return false;
	value = seq[pos];
	return true;
}

bool Sequence::set(int pos, const ItemType& value) {
	if (pos < 0 || pos >= seqSize)
		return false;
	seq[pos] = value;
	return true;
}

int Sequence::find(const ItemType& value) const {
	for (int i = 0; i < seqSize; i++)
		if (seq[i] == value)
			return i;
	return -1;
}

void Sequence::swap(Sequence& other) {
	ItemType* temp = seq;
	seq = other.seq;
	other.seq = temp;

	int tempSize = seqSize;
	seqSize = other.seqSize;
	other.seqSize = tempSize;

	int tempMaxSize = seqMaxSize;
	seqMaxSize = other.seqMaxSize;
	other.seqMaxSize = tempMaxSize;
}

Sequence::~Sequence() {
	delete [] seq;
}