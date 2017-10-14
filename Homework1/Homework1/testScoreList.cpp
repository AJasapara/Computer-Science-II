#include "ScoreList.h"
#include <iostream>
#include <cassert>
using namespace std;


int main() {
	ScoreList sl;
	sl.add(100);
	sl.add(99);
	sl.add(101);
	sl.add(-1100);
	sl.add(10);
	sl.add(100);
	sl.add(0);
	sl.add(20);
	sl.add(30);
	sl.add(99);
	sl.add(40);
	assert(sl.minimum() == 0);
	sl.remove(0);
	assert(sl.minimum() == 10);
	assert(sl.maximum() == 100);
	assert(sl.size() == 8);
}