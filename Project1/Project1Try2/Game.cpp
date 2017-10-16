#include <iostream>
#include <string>
#include "Game.h"
#include "globals.h"
#include "Arena.h"
#include "Player.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRats)
{
	if (nRats < 0)
	{
		cout << "***** Cannot create Game with negative number of rats!" << endl;
		exit(1);
	}
	if (nRats > MAXRATS)
	{
		cout << "***** Trying to create Game with " << nRats
			<< " rats; only " << MAXRATS << " are allowed!" << endl;
		exit(1);
	}
	if (rows == 1 && cols == 1 && nRats > 0)
	{
		cout << "***** Cannot create Game with nowhere to place the rats!" << endl;
		exit(1);
	}

	// Create arena
	m_arena = new Arena(rows, cols);

	// Add player
	int rPlayer;
	int cPlayer;
	do
	{
		rPlayer = randInt(1, rows);
		cPlayer = randInt(1, cols);
	} while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
	m_arena->addPlayer(rPlayer, cPlayer);

	// Populate with rats
	while (nRats > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		if (r == rPlayer && c == cPlayer)
			continue;
		m_arena->addRat(r, c);
		nRats--;
	}
}

Game::~Game()
{
	delete m_arena;
}

string Game::takePlayerTurn()
{
	for (;;)
	{
		cout << "Your move (n/e/s/w/x/h or nothing): ";
		string playerMove;
		getline(cin, playerMove);

		Player* player = m_arena->player();
		int dir;

		if (playerMove.size() == 0)
		{
			if (recommendMove(*m_arena, player->row(), player->col(), dir))
				return player->move(dir);
			else
				return player->dropPoisonPellet();
		}
		else if (playerMove.size() == 1)
		{
			if (tolower(playerMove[0]) == 'x')
				return player->dropPoisonPellet();
			else if (tolower(playerMove[0]) == 'h') {
				m_arena->history().display();
				cout << "Press enter to continue.";
				cin.ignore(10000, '\n');
				clearScreen();
				m_arena->display("");
				continue;
			}
			else if (decodeDirection(playerMove[0], dir))
				return player->move(dir);
		}
		cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
	}
}

void Game::play()
{
	m_arena->display("");
	while (!m_arena->player()->isDead() && m_arena->ratCount() > 0)
	{
		string msg = takePlayerTurn();
		Player* player = m_arena->player();
		if (player->isDead())
		{
			cout << msg << endl;
			break;
		}
		m_arena->moveRats();
		m_arena->display(msg);
	}
	if (m_arena->player()->isDead())
		cout << "You lose." << endl;
	else
		cout << "You win." << endl;
}