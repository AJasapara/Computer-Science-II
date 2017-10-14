#include <queue>
#include <iostream>
#include <string>
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
	queue<Coord> coordQueue;
	coordQueue.push(Coord(sr, sc));
	maze[sr][sc] = '#';
	while (!coordQueue.empty()) {
		Coord b = coordQueue.front();
		if (b.r() == er && b.c() == ec)
			return true;
		coordQueue.pop();
		if (maze[b.r() - 1][b.c()] == '.') {
			coordQueue.push(Coord(b.r() - 1, b.c()));
			maze[b.r() - 1][b.c()] = '#';
		}
		if (maze[b.r()][b.c() + 1] == '.') {
			coordQueue.push(Coord(b.r(), b.c() + 1));
			maze[b.r()][b.c() + 1] = '#';
		}
		if (maze[b.r() + 1][b.c()] == '.') {
			coordQueue.push(Coord(b.r() + 1, b.c()));
			maze[b.r() + 1][b.c()] = '#';
		}
		if (maze[b.r()][b.c() - 1] == '.') {
			coordQueue.push(Coord(b.r(), b.c() - 1));
			maze[b.r()][b.c() - 1] = '#';
		}
	}
	return false;
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
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