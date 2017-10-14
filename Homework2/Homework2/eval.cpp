#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

bool isOperand(char ch) {
	return ch >= '0' && ch <= '9';
}

bool isBOperator(char ch) {
	return  ch == '&' || ch == '|';
}

bool precedence(char op1, char op2) {
	if (op1 == '!')
		return op2 == '!';
	if (op1 == '&')
		return op2 != '|';
	return true;
}

int evaluate(string infix, const bool values[], string& postfix, bool& result)
// Evaluates a boolean expression
// Postcondition: If infix is a syntactically valid infix boolean
//   expression, then postfix is set to the postfix form of that
//   expression, result is set to the value of the expression (where
//   in that expression, each digit k represents element k of the
//   values array), and the function returns zero.  If infix is not a
//   syntactically valid expression, the function returns 1.  (In that
//   case, postfix may or may not be changed, but result must be
//   unchanged.)
{
	string str = "";
	for (int i = 0; i < infix.size(); i++)
		if (infix[i] != ' ')
			str += infix[i];
	infix = str;
	if (infix == "") return 1;
	int open = 0, close = 0, numOperand = 0;
	for (int i = 0; i < infix.size(); i++) {
		if (isOperand(infix[i])) {
			numOperand++;
			if (i + 1 < infix.size() && (isOperand(infix[i + 1]) || infix[i + 1] == '('))
				return 1;
		}
		if (isBOperator(infix[i]))
			if (i + 1 >= infix.size() || (!isOperand(infix[i + 1]) && infix[i + 1] != '(' && infix[i+1] != '!'))
				return 1;
		if (infix[i] == '(') {
			open++;
			if (i + 1 < infix.size() && isBOperator(infix[i + 1]))
				return 1;
		}
		else if (infix[i] == ')') {
			close++;
			if (i + 1 < infix.size() && isOperand(infix[i + 1]))
				return 1;
		}
	}
	if (numOperand == 0) return 1;
	if (infix[infix.size() - 1] == '!')
		return 1;
	if (open != close)
		return 1;
	postfix = "";
	stack<char> opStack;
	for (int i = 0; i < infix.size(); i++) {
		switch (infix[i]) {
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			postfix += infix[i];
			break;
		case '(':
			opStack.push(infix[i]);
			break;
		case ')':
			while (opStack.top() != '(') {
				postfix += opStack.top();
				opStack.pop();
			}
			opStack.pop();  // remove the '('
			break;
		case '&': case '!': case '|':
			while (!opStack.empty() && opStack.top() != '(' && precedence (infix[i], opStack.top())) {
				postfix += opStack.top();
				opStack.pop();
			}
			opStack.push(infix[i]);
			break;
		}
	}	
	while (!opStack.empty()) {
		postfix += opStack.top();
		opStack.pop();
	}
	stack<bool> boolStack;
	for (int i = 0; i < postfix.size(); i++) {
		if (isOperand(postfix[i]))
			boolStack.push(values[postfix[i] - '0']);
		else if (postfix[i] == '!') {
			bool op2 = boolStack.top();
			boolStack.pop();
			boolStack.push(!op2);
		}
		else {
			bool op2 = boolStack.top();
			boolStack.pop();
			bool op1 = boolStack.top();
			boolStack.pop();
			if (postfix[i] == '|')
				boolStack.push(op1 | op2);
			else
				boolStack.push(op1 & op2);
		}
	}
	result = boolStack.top();
	boolStack.pop();
	return 0;
}

int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;	
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|" && !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
	cout << "Passed all tests" << endl;
}