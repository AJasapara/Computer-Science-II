#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

class Compiler;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir) // initializes all member variables to 0
	{
		ticks = 0;
		inLead = 0;
		numColonies = 0;
		for (int i = 0; i < 3; i++)
			score[i] = 0;
	}
	void addFood(int x, int y, int amt);
	void deleteActor(EnergyHolder* e, int x, int y, std::list<Actor*>::iterator temp);
	void addActor(int x, int y, Actor* a);
	std::list<Actor*>::iterator someIterator(int x, int y, int a) { // returns an iterator to the beginning or the end of the list
		std::list<Actor*>::iterator it = m_field[y][x].begin();
		if (a == 1)
			it = m_field[y][x].end();
		return it;
	}

	bool empty(int x, int y) { // returns if the list is empty
		return m_field[y][x].empty();
	}

	Actor* getFrontActor(int x, int y) { // returns a pointer to the first Actor in a list, if the list isn't empty
		if (!m_field[y][x].empty())
			return *(m_field[y][x].begin());
		return nullptr;
	}

	void createAnt(int x, int y, int colony, Compiler* c);
	void dropPheromone(int x, int y, int colony);

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	virtual ~StudentWorld() { cleanUp(); }

private:
	std::list<Actor*> m_field[VIEW_HEIGHT][VIEW_WIDTH];
	int ticks;
	int score[4];
	std::string names[4];
	int inLead;
	Compiler* compilerForEntrant[4];
	int numColonies;
};

#endif // STUDENTWORLD_H_
