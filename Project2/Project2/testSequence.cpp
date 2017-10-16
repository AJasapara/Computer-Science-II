#include "Sequence.h"
#include <iostream>
using namespace std;

int main() {
	Sequence sq;

	// test insert methods
	cerr << "INSERT" << endl;
	sq.insert("hello");
	sq.insert("goodbye");
	sq.insert(0, "hi");
	sq.insert(1, "again");
	sq.insert(0, "he");
	sq.insert("aaaa");
	sq.insert("zzzz");
	sq.insert("hh");
	sq.insert(11, "hi");
	sq.insert(8, "hi");
	sq.dump();
	cerr << endl << "ERASE" << endl;

	// test erase
	sq.erase(0);
	sq.erase(10);
	sq.erase(6);
	sq.dump();
	cerr << endl << "REMOVE" << endl;

	// test remove
	sq.insert("hello");
	sq.insert(7, "hello");
	sq.remove("hello");
	sq.remove("hi");
	sq.dump();
	cerr << endl << "FIND" << endl;

	// test find
	cerr << sq.find("hh")<< endl << sq.find("goodbye") << endl << sq.find("hello") << endl << endl << "SWAP" << endl;

	// test swap
	Sequence sq2;
	sq2.insert("hello");
	sq2.insert("he");
	sq2.insert("hh");
	sq.dump();
	sq2.dump();
	cerr << endl;
	sq.swap(sq2);
	sq.dump();
	sq2.dump();
	cerr << endl << "COPY & ASSIGNMENT" << endl;

	// test copy & assignment
	Sequence sq3(sq);
	sq.erase(0);
	sq3.dump();
	sq.dump();
	cerr << sq3.size() << sq.size() << endl << endl;

	sq3 = sq2;
	sq2.erase(0);
	sq3.dump();
	sq2.dump();
	sq.dump();
	cerr << endl << "GET & SET" << endl;

	// test get & set
	ItemType temp;
	sq.get(0, temp);
	cerr << temp << endl << endl;
	temp = "booger";
	sq.set(0, temp);
	cerr << temp << endl << endl;
	sq.dump();
	cerr << endl << "SUBSEQUENCE" << endl;

	// test subsequence
	sq.remove("hello");
	sq2.insert(0, "hello");
	sq2.set(0, temp);
	sq2.insert(0, "hello");
	sq2.insert(0, "hello");
	sq2.insert(0, "hello");
	sq2.remove("again");
	sq2.remove("goodbye");
	sq2.insert(4,"interrupt");
	sq2.dump();

	sq2.dump();
	sq.dump();
	cerr << endl;
	cerr << subsequence(sq2, sq) << endl << endl << "INTERLEAVE" << endl;

	// test interleave
	Sequence result = sq, sq5, sq6;
	sq2.remove("hello");
	sq2.remove("booger");
	sq2.remove("hh");
	sq2.insert("unique");
	sq2.dump();
	interleave(sq2, sq2, sq2);
	sq2.dump();
}