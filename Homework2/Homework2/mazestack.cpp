#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	stack<Coord> coordStack;
	coordStack.push(Coord(sr, sc));
	maze[sr][sc] = '#';
	while (!coordStack.empty()) {
		Coord b = coordStack.top();
		if (b.r() == er && b.c() == ec)
			return true;
		coordStack.pop();
		if (maze[b.r() - 1][b.c()] == '.') {
			coordStack.push(Coord(b.r() - 1, b.c()));
			maze[b.r() - 1][b.c()] = '#';
		}
		if (maze[b.r()][b.c()+1] == '.') {
			coordStack.push(Coord(b.r(), b.c()+1));
			maze[b.r()][b.c()+1] = '#';
		}
		if (maze[b.r() + 1][b.c()] == '.') {
			coordStack.push(Coord(b.r() + 1, b.c()));
			maze[b.r() + 1][b.c()] = '#';
		}
		if (maze[b.r()][b.c()-1] == '.') {
			coordStack.push(Coord(b.r(), b.c()-1));
			maze[b.r()][b.c()-1] = '#';
		}
	}
	return false;
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X.X......X",
		"X...XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}