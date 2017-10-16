#include "StudentWorld.h"
#include "Field.h"
#include "Compiler.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init() {
	vector<string> fileNames = getFilenamesOfAntPrograms(); // creates compilers for each ant colony and initializes them
	string error;
	numColonies = fileNames.size();
	for (int i = 0; i < numColonies; i++) {
		compilerForEntrant[i] = new Compiler;
		if (!compilerForEntrant[i]->compile(fileNames[i], error))
		{
			setError(fileNames[0] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}
		names[i] = compilerForEntrant[i]->getColonyName();
	}
	Field f;
	if (f.loadField(getFieldFilename()) != Field::load_success) // loads field
		return GWSTATUS_LEVEL_ERROR;
	for (int x = 0; x < VIEW_WIDTH; x++) // loads field into 2-D array of lists
		for (int y = 0; y < VIEW_HEIGHT; y++) {
			Field::FieldItem fi = f.getContentsOf(x, y); 
			if (fi == Field::rock)
				m_field[y][x].push_back(new Pebble(x, y));
			if (fi == Field::grasshopper)
				m_field[y][x].push_back(new BabyGrasshopper(x, y));
			else if (fi == Field::food)
				m_field[y][x].push_back(new Food(x, y));
			else if (fi == Field::water)
				m_field[y][x].push_back(new WaterPool(x, y));
			else if (fi == Field::poison)
				m_field[y][x].push_back(new Poison(x, y));
			else if (numColonies >= 1 && fi == Field::anthill0)
				m_field[y][x].push_back(new Anthill(0, x, y, compilerForEntrant[0]));
			else if (numColonies >= 2 && fi == Field::anthill1)
				m_field[y][x].push_back(new Anthill(1, x, y, compilerForEntrant[1]));
			else if (numColonies >= 3 && fi == Field::anthill2)
				m_field[y][x].push_back(new Anthill(2, x, y, compilerForEntrant[2]));
			else if (numColonies >= 4 && fi == Field::anthill3)
				m_field[y][x].push_back(new Anthill(3, x, y, compilerForEntrant[3]));
			if (!m_field[y][x].empty())
				m_field[y][x].back()->setWorld(this); // sets world for each actor just added
		}
	ticks = 0;
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::addFood(int x, int y, int amt) { // creates new food or increases food HP at x,y
	Food* f = dynamic_cast<Food*>(*(m_field[y][x].begin()));
	if (f != nullptr)
		f->setHP(f->getHP() + amt);
	else
		m_field[y][x].push_front(new Food(x, y, amt));
}

void StudentWorld::dropPheromone(int x, int y, int colony) { // creates new pheromone or increases pheromone HP at x,y
	list<Actor*>::iterator it = m_field[y][x].begin();
	while (it != m_field[y][x].end()) {
		Pheromone* p = dynamic_cast<Pheromone*>(*it);
		if (p != nullptr && p->getColony() == colony) {
			p->setHP(p->getHP() + 256);
			if (p->getHP() > 768) p->setHP(768);
			return;
		}
		it++;
	}
	m_field[y][x].push_back(new Pheromone(colony + 11, x, y, colony));
	m_field[y][x].back()->setWorld(this);
}

void StudentWorld::deleteActor(EnergyHolder* e, int x, int y, list<Actor*>::iterator temp) { // deletes the EnergyHolder at location
	if (dynamic_cast<Insect*>(e) != nullptr)
		addFood(x, y, 100);
	delete e;
	m_field[y][x].erase(temp);
}

void StudentWorld::addActor(int x, int y, Actor * a) { // adds Actor to the list at location x,y
	m_field[y][x].push_back(a);
	m_field[y][x].back()->setWorld(this);
}

void StudentWorld::createAnt(int x, int y, int colony, Compiler* c) { // creates a new Ant of colony at x,y
	int image;
	score[colony]++;
	switch (colony) {
	case 0: image = IID_ANT_TYPE0; break;
	case 1: image = IID_ANT_TYPE1; break;
	case 2: image = IID_ANT_TYPE2; break;
	case 3: image = IID_ANT_TYPE3; break;
	}
	m_field[y][x].push_back(new Ant(colony, x, y, c, image));
	m_field[y][x].back()->setWorld(this);
}

int StudentWorld::move() {
	//int numColonies = getFilenamesOfAntPrograms().size();
	ticks++;
	for (int x = 0; x < VIEW_WIDTH; x++) // loops through entire field
		for (int y = 0; y < VIEW_HEIGHT; y++) {
			list<Actor*>::iterator it;
			it = m_field[y][x].begin();
			while (it != m_field[y][x].end()) { // loops through entire list of Actors
				list<Actor*>::iterator temp = it;
				bool tempMove = false;
				it++;			
				if ((*temp)->getMoved()) // if actor moved during this tick to a later spot in the array
					(*temp)->setMoved(false);
				else {
					(*temp)->doSomething();
					if ((*temp)->justMoved()) { // if actor just changed locations
						tempMove = true;
						(*temp)->setJustMoved(false);
						m_field[y][x].erase(temp);
					}
				}
				if (!tempMove) { // deletes each dead actor in the list
					EnergyHolder * e = dynamic_cast<EnergyHolder*>(*temp);
					if (e != nullptr && e->isDead())
						deleteActor(e,x,y,temp);
				}
			}
		}
	ostringstream oss; // sets status at top to show ticks and scores
	oss.setf(ios::fixed);
	oss << "Ticks:" << setw(5) << ticks << " -  ";
	int tie = 0;
	for (int i = 1; i < numColonies; i++)
		if (score[i - 1] == score[i])
			tie++;
	for (int i = 0; i < numColonies; i++) // determines what colony is in the lead
		if (score[i] > score[inLead])
			inLead = i;
	for (int i = 0; i < numColonies; i++) {
		oss << names[i];
		if (tie < numColonies-1 && i == inLead)
			oss << "*";
		oss << ": " << setfill('0') << setw(2) << score[i] << "  ";
	}
	setGameStatText(oss.str());

	if (ticks >= 2000) { // determines if there is a winner at the end
		if (tie >= 3 && score[0] == 5)
			return GWSTATUS_NO_WINNER;
		setWinner(names[inLead]);
		return GWSTATUS_PLAYER_WON;
	}

	return GWSTATUS_CONTINUE_GAME;
}

// CLEAN UP
void StudentWorld::cleanUp() {
	for (int i = 0; i < numColonies; i++) // deletes the compilers
		delete compilerForEntrant[i];
	numColonies = 0;
	for (int x = 0; x < VIEW_WIDTH; x++)
		for (int y = 0; y < VIEW_HEIGHT; y++) { // deletes all Actors and components inside the lists
			list<Actor*>::iterator it;
			it = m_field[y][x].begin();
			while (it != m_field[y][x].end()) {
				delete *it;
				list<Actor*>::iterator temp = it;
				it++;
				m_field[y][x].erase(temp);
			}
		}
	
}