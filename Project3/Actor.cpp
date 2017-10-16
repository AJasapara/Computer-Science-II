#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include <cmath>
using namespace std;

void Ant::doSomething() {
	if (!start()) return;
	commandsRun = 0;
	if (getStunned() > 0) {
		setStunned(getStunned() - 1);
		return;
	}
	while (commandsRun < 10) { // runs commands until 10
		Compiler::Command c;
		if (!m_compiler->getCommand(inst, c)) {
			setDead();
			return;
		}
		switch (c.opcode) { // runs specific command based on code in bug file
		case Compiler::invalid:
			setDead(); return;
		case Compiler::moveForward:
			if (moveActor()) {
				setJustMoved(true);
				setWaterStunned(false);
				prevBlocked = false;
				prevBitten = false;
			}
			else
				prevBlocked = false;
			inst++; return;
		case Compiler::eatFood:
			if (food < 100) {
				setHP(getHP() + food);
				food = 0;
			}
			else {
				setHP(getHP() + 100);
				food -= 100;
			}
			inst++; return;
		case Compiler::dropFood:
			dropFood(); inst++; return;
		case Compiler::bite:
			bite(); inst++; return;
		case Compiler::pickupFood:
			pickupFood();inst++; return;
		case Compiler::emitPheromone:
			getWorld()->dropPheromone(getX(), getY(), m_colony);inst++; return;
		case Compiler::faceRandomDirection:
			setDirection(randomDir()); inst++; return;
		case Compiler::rotateClockwise:
			rotate(true);inst++; return;
		case Compiler::rotateCounterClockwise:
			rotate(false);inst++; return;
		case Compiler::generateRandomNumber: 
			lastRandomNumber = randInt(0, c.operand1[0] - '0' - 1);
			if (c.operand1[0] == '0')
				lastRandomNumber = 0;
			commandsRun++; break;
		case Compiler::goto_command:
			inst = c.operand1[0] - '0';
			commandsRun++; break;
		case Compiler::if_command:
			if (ifTrue(c.operand1[0] - '0'))
				inst = c.operand2[0] - '0';
			else inst++;
			commandsRun++; break;
		}
	}
}

bool Ant::ifTrue(int num) { // determines if the if_condition is true
	switch (num) {
	case 0: return frontOfMe(false) != nullptr;
	case 1: return frontOfMe(true) != nullptr;
	case 2: return prevBitten;
	case 3: return food > 0;
	case 4: return getHP() <= 25;
	case 5: return onAnthill();
	case 6: 
		if(!getWorld()->empty(getX(),getY()))
			return (dynamic_cast<Food*>(getWorld()->getFrontActor(getX(), getY())) != nullptr);
		return false;
	case 7: 
		return enemy();
	case 8: return prevBlocked;
	case 9: return lastRandomNumber == 0;
	}
	return false;
}

Actor* Ant::frontOfMe(bool pher) { // determines if there's a pheromone or danger in front of Ant
	Direction dir = getDirection();
	int x = getX(), y = getY();
	switch (dir) {
	case up: y += 1; break;
	case right: x += 1; break;
	case down: y -= 1; break;
	case left: x -= 1; break;
	}
	if ((x >= 0 && x < VIEW_WIDTH) && (y >= 0 && y < VIEW_HEIGHT)) {
		if (getWorld()->empty(x, y))
			return nullptr;
		if (!pher) {
			Actor* a = getWorld()->getFrontActor(x, y);
			if (dynamic_cast<Poison*>(a) != nullptr || dynamic_cast<WaterPool*>(a) != nullptr) // checks for Poison/WaterPool
				return a;
		}	
		list<Actor*>::iterator it = getWorld()->someIterator(x, y, 0);
		while (it != getWorld()->someIterator(x, y, 1)) {
			if (pher) {
				Pheromone* p = dynamic_cast<Pheromone*>(*it);
				if (p != nullptr && p->getColony() == m_colony) // checks if Pheromone belongs to Ant's colony
					return p;
			}
			else {
				Insect*i = dynamic_cast<Insect*>(*it); // checks if Grasshopper/Ant of different colony
				if (i != nullptr) {
					Ant* other = dynamic_cast<Ant*>(i);
					if (other != nullptr) {
						if (other->m_colony != m_colony)
							return other;
					}
					else
						return i;
				}
			}
			it++;
		}
	}
	return nullptr;
}

bool Ant::onAnthill() {
	Anthill* ah = dynamic_cast<Anthill*>(getWorld()->getFrontActor(getX(), getY()));
	return (ah != nullptr && ah->getColony() == m_colony);
}

bool Ant::enemy() { // determines if enemy is on same square
	list<Actor*>::iterator it = getWorld()->someIterator(getX(), getY(), 0);
	while (it != getWorld()->someIterator(getX(), getY(), 1)) {
		Insect*i = dynamic_cast<Insect*>(*it);
		if (i != nullptr) {
			Ant* other = dynamic_cast<Ant*>(i);
			if (other != nullptr) {
				if (other->m_colony != m_colony)
					return true;
			}
			else
				return true;
		}
		it++;
	} return false;
}

void Ant::dropFood() {
	Anthill* ah = dynamic_cast<Anthill*>(getWorld()->getFrontActor(getX(), getY()));
	if (ah != nullptr)
		ah->setHP(ah->getHP() + food);
	else
		getWorld()->addFood(getX(), getY(), food);
	food = 0;
}

void Ant::bite() {
	list<Actor*>::iterator it = getWorld()->someIterator(getX(), getY(), 0);
	while (it != getWorld()->someIterator(getX(), getY(), 1)) {
		Insect*i = dynamic_cast<Insect*>(*it);
		if (i != nullptr) {
			Ant* other = dynamic_cast<Ant*>(i);
			if (other != nullptr) {
				if (other->m_colony != m_colony) {
					other->setHP(other->getHP() - 15);
					other->bitten(); // sets bitten for other ant
					return;
				}
			}
			else {
				i->setHP(i->getHP() - 15);
				AdultGrasshopper* ag = dynamic_cast<AdultGrasshopper*>(i);
				if (ag != nullptr) ag->bite(1); // lets AdultGrasshopper bite back
				return;
			}
		}
		it++;
	}
}

void Ant::pickupFood() {
	Food* f = dynamic_cast<Food*>(getWorld()->getFrontActor(getX(), getY()));
	if (f != nullptr) {
		int desired = 1800 - food;
		if (desired > 400)
			desired = 400;
		int amt = f->getHP();
		if (desired > amt) {
			f->setHP(0);
			food += amt;
		}
		else {
			f->setHP(amt - desired);
			food += desired;
		}
	}
}

void Ant::rotate(bool clockwise) {
	Direction dir = getDirection();
	if (clockwise) {
		switch (dir) {
		case up: setDirection(right); break;
		case right: setDirection(down); break;
		case down: setDirection(left); break;
		case left: setDirection(up); break;
		}
	}
	else {
		switch (dir) {
		case up: setDirection(left); break;
		case left: setDirection(down); break;
		case down: setDirection(right); break;
		case right: setDirection(up); break;
		}
	}
}

void Anthill::doSomething() {
	if (!start()) return;
	if (getHP() >= 2000) { // creates Ant if it has enough health
		getWorld()->createAnt(getX(), getY(), m_colony, m_compiler);
		setHP(getHP() - 1500);
	}
}

bool Insect::moveActor() { // moves Actor in direction it's facing, returns false if it runs into a Pebble
	GraphObject::Direction dir = getDirection();
	int x = getX(), y = getY();
	switch (dir) {
	case GraphObject::left:
		if (x - 1 >= 0)
			if ((getWorld()->empty(x - 1, y)) || getWorld()->getFrontActor(x-1, y)->notPebble()) {
				moveTo(x - 1, y);
				getWorld()->addActor(x-1,y, this);
				return true;
			}
		break;
	case GraphObject::right:
		if (x + 1 < VIEW_WIDTH)
			if ((getWorld()->empty(x + 1, y))|| getWorld()->getFrontActor(x + 1, y)->notPebble()) {
				moveTo(x + 1, y);
				getWorld()->addActor(x + 1, y, this);
				setMoved(true);
				return true;
			}
		break;
	case GraphObject::up:
		if (y + 1 < VIEW_HEIGHT)
			if ((getWorld()->empty(x, y + 1)) || getWorld()->getFrontActor(x, y+1)->notPebble()) {
				moveTo(x, y+1);
				getWorld()->addActor(x, y+1, this);
				setMoved(true);
				return true;
			}
		break;
	case GraphObject::down:
		if (y - 1 >= 0)
			if ((getWorld()->empty(x, y - 1)) || getWorld()->getFrontActor(x, y-1)->notPebble()) {
				moveTo(x, y-1);
				getWorld()->addActor(x, y-1, this);
				return true;
			}
		break;
	}
	return false;
}

bool GrassHopper::begTurn() { // standard beginning of turn procedures for all Grasshoppers
	m_sleep = false;
	if (!start()) return false;
	if (getStunned() > 0) {
		setStunned(getStunned() - 1);
		return false;
	}
	setStunned(2);
	return true;
}

void GrassHopper::eat() { // standard eating and moving method for all Grasshoppers
	Food* f = dynamic_cast<Food*>(getWorld()->getFrontActor(getX(), getY()));
	if (f != nullptr) { // if food on square, eats it
		int amt = f->getHP();
		if (amt <= 200) {
			setHP(getHP() + amt);
			f->setHP(0);
			getWorld()->deleteActor(f, getX(), getY(), getWorld()->someIterator(getX(), getY(), 0));
		}
		else {
			setHP(getHP() + 200);
			f->setHP(amt - 200);
		}
		int randSleep = randInt(0, 1);
		if (randSleep == 0) // 50/50 chance it sleeps
		m_sleep = true;
	}
	if (!m_sleep) { // if it doesn't sleep, it moves this turn/changes direction
		if (getDistance() == 0) {
			setDirection(randomDir());
			newDistance();
		}
		if (moveActor()) {
			setWaterStunned(false);
			setDistance(getDistance() - 1);
			setJustMoved(true);
		}
		else
			setDistance(0);
	}
}

void AdultGrasshopper::doSomething() {
	if (!begTurn()) return;
	if (bite()) return;
	if (jump()) return;
	eat();
	setWaterStunned(true); // ensure it never gets stunned by WaterPool
}

bool AdultGrasshopper::jump() {
	int jumping = randInt(0,9);
	if (jumping == 0) {
		double rand = double(randInt(1, 10)); // determines random radius
		double angle = double(randInt(0, 360));  // determines random direction
		int x = getX() + rand*cos(angle*3.1415926535/180.0), y = getY() + rand*sin(angle*3.1415926535 / 180.0);
		if ((x >= 0 && x < VIEW_WIDTH) && (y >= 0 && y < VIEW_HEIGHT)) // if valid point, jumps there
			if (getWorld()->empty(x, y) || getWorld()->getFrontActor(x, y)->notPebble()) {
				moveTo(x, y);
				getWorld()->addActor(x, y, this);
				setJustMoved(true);
				return true;
			}
	}
	return false;
}

bool AdultGrasshopper::bite(int now) {
	int rand = randInt(0, now);
	if (rand == 0) {
		list<Actor*>::iterator it = getWorld()->someIterator(getX(), getY(), 0);
		while (it != getWorld()->someIterator(getX(), getY(), 1)) {
			Insect*i = dynamic_cast<Insect*>(*it); // bites a random insect
			if (i != nullptr && i != this) {
				i->setHP(i->getHP() - 50);
				AdultGrasshopper* ag = dynamic_cast<AdultGrasshopper*>(i);
				if (ag != nullptr) ag->bite(1); // gives AdultGrasshopper chance to bite back
				Ant* a = dynamic_cast<Ant*>(i);
				if (a != nullptr) a->bitten(); // sets prevBitten for Ant on this square
				return true;
			}
			it++;
		}
	}
	return false;
}

void BabyGrasshopper::doSomething() {
	if (!begTurn()) return;
	if (getHP() >= 1600) { // if it has more than 1600 energy, converts into AdultGrasshopper
		setDead();
		getWorld()->addActor(getX(),getY(),(new AdultGrasshopper(getX(), getY())));
		return;
	}
	eat();
}

void WaterPool::doSomething() {
	list<Actor*>::iterator it = getWorld()->someIterator(getX(), getY(), 0);
	while (it != getWorld()->someIterator(getX(), getY(), 1)) {
		Insect* i = dynamic_cast<Insect*>(*it); // stuns all insects
		if (i != nullptr && !i->getWaterStunned()) { // makes sure to not re-stun insects on square
			i->setStunned(i->getStunned() + 2); 
			i->setWaterStunned(true);
		}
		it++;
	}
}

void Poison::doSomething() {
	list<Actor*>::iterator it = getWorld()->someIterator(getX(),getY(),0);
	while (it != getWorld()->someIterator(getX(), getY(), 1)) {
		Insect* i = dynamic_cast<Insect*>(*it);    // poisons all insects every tick
		if (i != nullptr && !i->getWaterStunned()) // Since AdultGrasshoppers are always waterStunned, and water/poison cannot coincide
			i->setHP(i->getHP() - 150);			   // this makes the AdultGrasshoppers impervious to Poison. All others lose 150 hp per tick.
		it++;
	}
}