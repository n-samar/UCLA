#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "Compiler.h"


/********************************** CONSTANTS ************************************/

const int MIN_GRASSHOPPER_STEP = 2;
const int MAX_GRASSHOPPER_STEP = 10;
const int GRASSHOPPER_SLEEP = 2;
const int ANT_PICK_UP_CAP = 400;
const int GRASSHOPPER_VENOM = 50;
const int JUMP_RADIUS = 10;
const int FOOD_RESERVE_CAP = 1800;
const int MAX_PHEROMONE    = 768;
const int INIT_BABYGRASSHOPPER_HEALTH = 500;
const int INIT_PHEROMONE = 256;
const int INIT_ANT_HEALTH = 1500;
const int INIT_ANTHILL_HEALTH = 8999;
const int GRASSHOPPER_BITE_SIZE = 200;
const int BIRTH_GIVING_ENERGY_NEEDED = 2000;
const int BIRTH_GIVING_ENERGY_EXPENDED = 1500;
const int INIT_ADULT_GRASSHOPPER_HEALTH = 1600;
const int ENERGY_HOLDER_DEATH_FOOD_DROP = 100;
const int ANT_BITE_SIZE = 100;
const int INIT_FOOD_AMOUNT = 6000;
const int MAX_COMMANDS = 10;
const int ANT_VENOM = 15;
const int CORPSE_FOOD = 100;
const int POISONED = 150;
const int ITEM_COLONY = 5;
const int MAX_JUMP_ATTEMPTS = 50;
const int ENEMY_COLONY = 4;
const int ANT_HILL_BITE_SIZE = 10000;

class Compiler;
class StudentWorld;


int getAntType(int colonyNum);
int getPherType(int colonyNum);
void getPosInDir(int &x, int &y, GraphObject::Direction dir);

GraphObject::Direction randDir();

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int imageId, int x, int y,
          Direction dir, int graphDepth)
    : GraphObject(imageId, x, y, dir)
    {
        m_world = world;
        m_alive = true;
    }
    
    StudentWorld* getWorld() {return m_world;}
    bool isDead() const { return !m_alive;}

    virtual bool doSomething() = 0;
    
protected:
    
    virtual void kill() {m_alive = false;}
    
private:
    StudentWorld* m_world;
    bool m_alive;
};
class Ant;
class EnergyHolder : public Actor
{
public:
    
    EnergyHolder(int colonyNum, StudentWorld* world,
                 int imageId, int x, int y,
                 Direction dir, int graphDepth, int hitPoints);
    
    int getColonyNum() {return m_colonyNum;}

    virtual bool doSomething();
    int getHitPoints() {return m_hitPoints;}
protected:
    virtual int pickUpFood(int cap);   // cap is max amount of food the object can eat
    virtual void dropFood(int amount);
    virtual bool eat(int cap);
    void setHitPoints(int val);

    
private:
    int m_hitPoints;
    int m_colonyNum;
};

class Insect : public EnergyHolder
{
public:
    Insect(int colonyNum, StudentWorld* world, int imageId, int x, int y,
           Direction dir, int graphDepth, int hitPoints)
    : EnergyHolder(colonyNum, world, imageId, x, y,
                   dir, graphDepth, hitPoints)
    {
        m_stunned = false;
        m_bitten  = false;
        m_blocked = false;
    }
    
    virtual int getVenom() = 0;
    int getSleep() {return m_sleepTicks;}
    

    
    // WARNING: This similar structure is repeated somewhere, take care of that
    virtual void poison();
    
    virtual bool zap(int amt);

    virtual Insect* getEnemyAt(int x, int y) = 0;
    virtual bool doSomething();
    
    bool move();
    
    bool isStunned() const {return m_stunned;}
    
    virtual void stun()
    {
        m_sleepTicks += 2;
        m_stunned = true;
    }
    
protected:
    


    void setSleep(int amt) {m_sleepTicks = amt;}
    virtual bool bite();
    virtual void kill();
    
    bool isBlocked() const {return m_blocked;}
    void blocked() {m_blocked = true;}
    void unblocked() {m_blocked = false;}
    
    void bitten() {m_bitten = true;}
    void unbitten() {m_bitten = false;}
    bool isBitten() const {return m_bitten;}
    
    void unstun() {m_stunned = false;}
    
private:
    
    bool m_stunned;
    int m_sleepTicks = 0;
    int m_distLeft = 0;
    bool m_bitten;
    bool m_blocked;
};


class AntHill : public EnergyHolder
{
public:
    AntHill(StudentWorld* world, int x, int y,
            int colonyNum, Compiler* compiler)
    : EnergyHolder(colonyNum, world, IID_ANT_HILL, x, y,
                   Direction::right, 2, INIT_ANTHILL_HEALTH),
    m_compiler(compiler){}

    virtual bool doSomething();

private:
    void giveBirth();
    Compiler* m_compiler;
};

class Ant : public Insect
{
public:
    virtual int getVenom() {return ANT_VENOM;}
    virtual Insect* getEnemyAt(int x, int y);
    Ant(StudentWorld* world, int x, int y,
        int colonyNum, Compiler* compiler)
    : Insect(colonyNum, world, getAntType(colonyNum), x, y,
             randDir(), 1, INIT_ANT_HEALTH),
    m_compiler(compiler),
    m_instructionPtr(0),
    m_foodReserve(0),
    m_randNum(0)
    {
        setHitPoints(INIT_ANT_HEALTH);
        setDirection(randDir());
    }
    
    bool onMyAnthill();
    virtual bool doSomething();
    
private:
    bool dangerous(int x, int y);
    bool runCommand(const Compiler::Command& c);
    bool emitPheromone();
    virtual bool eat();
    bool pickUpFood();
    virtual void dropFood();        // used when dropping food if instructed by the .bug code, not when dead!
    virtual void kill();
    void rotateClockwise();
    void rotateCounterClockwise();
    int m_instructionPtr;
    Compiler* m_compiler;
    int m_randNum;
    int m_foodReserve;
};

class Food : public EnergyHolder
{
public:
    Food(StudentWorld* world, int x, int y, int amt = 100)   // Pass false to insectDeath if
                                                                // created at start of simulation
    : EnergyHolder(ITEM_COLONY, world, IID_FOOD, x, y,
                   GraphObject::Direction::right, 2, amt) {}
    // Checks if it should die
    bool doSomething() {
        if(getHitPoints() <= 0)
        {
            kill();
            return true;
        }
        return false;}
    
    // WARNING: Is there a limit to this?
    // WARNING: Very similar to pheromone's add
    void add(int amount) {setHitPoints(getHitPoints() + amount);}
};

class Pheromone : public EnergyHolder
{
public:
    // WARNING: Too many arguments
    Pheromone(int colonyNum, StudentWorld* world, int x, int y)
    : EnergyHolder(colonyNum, world, getPherType(colonyNum),
                   x, y, Direction::right, 2, INIT_PHEROMONE) {}
    void add(int amount);
};

class Grasshopper : public Insect
{
public:
    Grasshopper(StudentWorld* world, int typeId,
                int x, int y, Direction dir, int hitPoints)
    : Insect(ENEMY_COLONY, world, typeId, x, y, dir, 1, hitPoints) {};
    
    virtual int getVenom() {return GRASSHOPPER_VENOM;}
    virtual Insect* getEnemyAt(int x, int y);
    virtual bool doSomething();

private:
    virtual bool eat();
    int m_distLeft = randInt(2, 10);
};

class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(StudentWorld* world, int x, int y)
    : Grasshopper(world, IID_BABY_GRASSHOPPER, x, y,
                  randDir(), INIT_BABYGRASSHOPPER_HEALTH){}
    virtual bool doSomething();
};

class AdultGrasshopper : public Grasshopper
{
public:
    virtual bool zap(int amt);
    virtual void stun(){}
    virtual void poison(){}
    AdultGrasshopper(StudentWorld* world, int x, int y)
    : Grasshopper(world, IID_ADULT_GRASSHOPPER, x, y,
                  randDir(), INIT_ADULT_GRASSHOPPER_HEALTH){}
    virtual bool doSomething();
};

class Pebble : public Actor
{
public:
    Pebble(StudentWorld* world, int x, int y)
    : Actor(world, IID_ROCK, x, y, Direction::right, 1){}
    // Does nothing
    bool doSomething() {return false;}
};


class Damager : public Actor
{
public:
    Damager(StudentWorld* world, int typeId, int x, int y,
            Direction dir, int depth)
    : Actor(world, typeId, x, y, dir, depth){}
    
    // Implement doSomething() to do the general stuff, maybe use a vector of all currently damaged objects
    virtual bool doSomething();
    virtual void damage(Insect* i) = 0;
    virtual void damageOld(Insect* i) = 0;
private:
    std::vector<Insect*> m_damagedInsects;

};

class Pool : public Damager
{
public:
    Pool(StudentWorld* world, int x, int y)
    : Damager(world, IID_WATER_POOL, x, y, Direction::right, 2){}
    virtual void damageOld(Insect* i) {}
    virtual void damage(Insect* i) {i->stun();}
};

class Poison : public Damager
{
public:
    Poison(StudentWorld* world, int x, int y)
    : Damager(world, IID_POISON, x, y, Direction::right, 2){}
    virtual void damageOld(Insect* i) {i->poison();}
    virtual void damage(Insect* i) {i->poison();}
};
#endif // ACTOR_H_
