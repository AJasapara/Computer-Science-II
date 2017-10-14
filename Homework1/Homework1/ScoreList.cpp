#include "ScoreList.h"

unsigned long ScoreList::minimum() const {
	if (seq.empty())
		return NO_SCORE;

	unsigned long min;
	seq.get(0, min);
	for (int i = 0; i < size(); i++) {
		unsigned long temp;
		seq.get(i, temp);
		if (temp < min)
			min = temp;
	}
	return min;
}
// Return the lowest score in the score list.  If the list is
// empty, return NO_SCORE.

unsigned long ScoreList::maximum() const {
	if (seq.empty())
		return NO_SCORE;

	unsigned long max;
	seq.get(0, max);
	for (int i = 0; i < size(); i++) {
		unsigned long temp;
		seq.get(i, temp);
		if (temp > max)
			max = temp;
	}
	return max;
}
// Return the highest score in the score list.  If the list is
// empty, return NO_SCORE.