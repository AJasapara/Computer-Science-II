#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;
class Compiler;

class Actor : public GraphObject // default Actor, cannot be initialized
{
public:
	Actor(int imageID, int x, int y, int depth) :GraphObject(imageID, x, y, right, depth) {	
		m_moved = false;
		just_moved = false;
		notpebble = true;
	}
	virtual void doSomething() = 0;
	void setMoved(bool move) { m_moved = move; }
	bool getMoved() { return m_moved; }
	bool justMoved() { return just_moved; }
	void setJustMoved(bool jm) { just_moved = jm; }
	StudentWorld* getWorld() { return m_world; }
	void setWorld(StudentWorld* sw) { m_world = sw; }
	bool notPebble() { return notpebble; }
	void setPebble() { notpebble = false; }
private:
	bool m_moved, just_moved, notpebble;
	StudentWorld* m_world;
};

class EnergyHolder : public Actor { // cannot be initialized
public:
	EnergyHolder(int imageID, int x, int y, int depth) :Actor(imageID, x, y, depth) {
		m_dead = false;
	}
	bool isDead() { return hp <= 0 || m_dead; }
	virtual void doSomething() = 0;
	void setDead() { m_dead = true; }
	void setHP(int nhp) { hp = nhp; }
	int getHP() { return hp; }
	bool start() { // standard first moves for all EnergyHolders
		setHP(getHP() - 1);
		if (isDead()) {
			setDead();
			return false;
		}
		return true;
	}
private:
	int hp;
	bool m_dead;
};

class Insect : public EnergyHolder {
public:
	Insect(int imageID, int x, int y) : EnergyHolder(imageID, x, y, 1) { // cannot be initialized
		stunned = 0;
		water_stunned = false;
		setDirection(randomDir());
	}
	Direction randomDir() { // creates random direction for an insect
		Direction dir;
		int rand = randInt(0, 3);
		switch (rand) {
		case 0: dir = left; break;
		case 1: dir = right; break;
		case 2: dir = up; break;
		case 3: dir = down; break;
		}
		return dir;
	}
	int getStunned() { return stunned; }
	void setStunned(int stun) { stunned = stun; }
	virtual void doSomething() = 0;
	bool moveActor();
	bool getWaterStunned() { return water_stunned; }
	void setWaterStunned(bool ws) { water_stunned = ws; }
private:
	int stunned;
	bool water_stunned;
};

class Ant : public Insect
{
public:
	Ant(int colony, int x, int y, Compiler* c, int imageID) : Insect(imageID, x, y) {
		setHP(1500);
		m_colony = colony;
		m_compiler = c;
		lastRandomNumber = 0;
		prevBitten = false;
		prevBlocked = false;
		inst = 0;
		food = 0;
		commandsRun = 0;
	}
	void bitten() { prevBitten = true; }
	virtual void doSomething();
private:
	int m_colony, lastRandomNumber, food, inst, commandsRun;
	Compiler* m_compiler;
	bool prevBitten, prevBlocked;
	void dropFood();
	void bite();
	void pickupFood();
	void rotate(bool clockwise);
	bool ifTrue(int num);
	bool enemy();
	bool onAnthill();
	Actor* frontOfMe(bool pher);
};

class GrassHopper : public Insect
{
public:
	GrassHopper(int x, int y, int imageID):Insect(imageID, x, y) {
		newDistance();
		m_sleep = false;
	}
	int getDistance() { return distance; }
	void newDistance(){ distance = randInt(2, 10); }
	void setDistance(int ndistance) { distance = ndistance; }
	void eat();
	virtual void doSomething() = 0;
	bool begTurn();
private:
	int distance;
	bool m_sleep;
};

class AdultGrasshopper : public GrassHopper {
public:
	AdultGrasshopper(int x, int y) :GrassHopper(x, y, IID_ADULT_GRASSHOPPER) {
		setHP(1600);
		setWaterStunned(true);
	}
	virtual void doSomething();
	bool bite(int now = 2);
private:
	bool jump();
};

class BabyGrasshopper : public GrassHopper
{
public:
	BabyGrasshopper(int x, int y):GrassHopper(x,y, IID_BABY_GRASSHOPPER) {
		setHP(500);
	}
	virtual void doSomething();
private:
};

class Anthill : public EnergyHolder
{
public:
	Anthill(int colony, int x, int y, Compiler* c) :EnergyHolder(IID_ANT_HILL, x, y, 2) {
		setHP(8999);
		m_colony = colony;
		m_compiler = c;
	}
	virtual void doSomething();
	int getColony() { return m_colony; }
private:
	int m_colony;
	Compiler* m_compiler;
};


class Poison : public Actor
{
public:
	Poison(int x, int y) : Actor(IID_POISON, x, y, 2) {}
	virtual void doSomething();
};

class Food : public EnergyHolder
{
public:
	Food(int x, int y, int f = 6000) : EnergyHolder(IID_FOOD, x, y, 2) {
		setHP(f);
	}
	virtual void doSomething() {}

private:
};

class WaterPool : public Actor
{
public:
	WaterPool(int x, int y): Actor(IID_WATER_POOL,x,y,2){}
	virtual void doSomething();
};

class Pebble : public Actor
{
public:
	Pebble(int x, int y):Actor(IID_ROCK, x,y,1){
		setPebble();
	}
	virtual void doSomething() {}
};

class Pheromone : public EnergyHolder
{
public:
	Pheromone(int ID, int x, int y, int colony) :EnergyHolder(ID, x, y, 2) {
		setHP(256);
	}
	virtual void doSomething() {
		if (!start()) return;
	}
	int getColony() { return m_colony; }
private:
	int m_colony;
};

#endif // ACTOR_H_
