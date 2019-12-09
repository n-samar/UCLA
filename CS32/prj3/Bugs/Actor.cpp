#include "Actor.h"
#include "StudentWorld.h"
#include <stack>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

using namespace std;



/****************************** STATIC FUNCTIONS **********************************/

GraphObject::Direction randDir()
{
    int rand = randInt(1, 4);
    switch(rand)
    {
        case 1:
            return GraphObject::Direction::up; break;
        case 2:
            return GraphObject::Direction::down; break;
        case 3:
            return GraphObject::Direction::left; break;
        case 4:
            return GraphObject::Direction::right; break;
    }
    return GraphObject::Direction::up;
}

void getPosInDir(int &x, int &y, GraphObject::Direction dir)
{
    switch(dir)
    {
        case GraphObject::Direction::up:    y++; break;
        case GraphObject::Direction::down:  y--; break;
        case GraphObject::Direction::left:  x--; break;
        case GraphObject::Direction::right: x++; break;
        default: break;
    }
}

int getPherType(int colonyNum)
{
    switch(colonyNum)
    {
        case 0:
            return IID_PHEROMONE_TYPE0;
        case 1:
            return IID_PHEROMONE_TYPE1;
        case 2:
            return IID_PHEROMONE_TYPE2;
        case 3:
            return IID_PHEROMONE_TYPE3;
    }
    return 0;
}

int getAntType(int colonyNum)
{
    switch(colonyNum)
    {
        case 0:
            return IID_ANT_TYPE0;
        case 1:
            return IID_ANT_TYPE1;
        case 2:
            return IID_ANT_TYPE2;
        case 3:
            return IID_ANT_TYPE3;
    }
    return 0;
}
/***************************** ADULTGRASSHOPPER ************************************/

bool AdultGrasshopper::doSomething()
{
    if(Insect::doSomething()) return true;         // 1. 2. 3. 4.
    if(randInt(0, 2) == 0)              // 5.
    {
        if(bite())
        {
            setSleep(GRASSHOPPER_SLEEP);
            return true;
        }
    }else if(randInt(0, 9) == 0)        // 6. 1 in 10 chance
    {
        int x = getX();
        int y = getY();
        getWorld()->getPosWithinRadius(x, y, JUMP_RADIUS);
        int jumpAttempts = 0;
        while(getWorld()->getAt<Pebble>(x, y) != nullptr && jumpAttempts < MAX_JUMP_ATTEMPTS)
        {
            x = getX();
            y = getY();
            getWorld()->getPosWithinRadius(x, y, JUMP_RADIUS);
            jumpAttempts++;
        }
        if(getWorld()->getAt<Pebble>(x, y) == nullptr)
        {
            moveTo(x, y);
            setSleep(GRASSHOPPER_SLEEP);
            return true;
        }
    }
    
    Grasshopper::doSomething();
    return false;
}

bool AdultGrasshopper::zap(int amt)
{
    Insect::zap(amt);
    if(randInt(0, 1) == 0)                      // retaliate, bite-back
        bite();
    return true;
}

/********************************** ANT *********************************************/

void Ant::dropFood()
{
    EnergyHolder::dropFood(m_foodReserve);
    m_foodReserve = 0;
}

bool Ant::onMyAnthill()
{
    int x = this->getX();
    int y = this->getY();
    std::vector<AntHill*> ah = getWorld()->getVector<AntHill>(x, y);
    int i = 0;
    for(; i < ah.size(); i++)
        if(ah[i]->getColonyNum() == getColonyNum())
            break;
    if(i == ah.size())
        return false;
    else return true;
}

Insect* Ant::getEnemyAt(int x, int y)
{
    vector<Insect*> res = getWorld()->getVector<Insect>(x, y);
    auto it = res.begin();
    for(; it != res.end() && (*it)->getColonyNum() == this->getColonyNum(); ++it)
        ;
    if(it == res.end())
        return nullptr;
    return *it;
}

bool Ant::eat()                                          // 4. c. ii.
{
    if(m_foodReserve == 0) return false;
    if(m_foodReserve > ANT_BITE_SIZE)
    {
        setHitPoints(getHitPoints() + ANT_BITE_SIZE);
        m_foodReserve -= ANT_BITE_SIZE;
    }else
    {
        setHitPoints(getHitPoints() + m_foodReserve);
        m_foodReserve = 0;
    }
    return true;
}

bool Insect::bite()
{
    Insect* enemy = getEnemyAt(getX(), getY());
    if(enemy)
    {
        enemy->zap(getVenom());
        return true;
    }
    return false;
}

bool Ant::doSomething()
{
    if(Insect::doSomething()) return true;                  // 1. 2. 3.
    
    Compiler::Command c;
    int instructionCount = 0;
    while(instructionCount < MAX_COMMANDS)                  // 4. f. / 4. g.
    {
        if(!m_compiler->getCommand(m_instructionPtr, c))    // 4. a.
        {
            kill();                                         // 4. b. i.
            return true;                                    // 4. b. ii.
        }
        if(runCommand(c))
        {
            return true;
        }
        instructionCount++;
    }
    return false;
}


bool Ant::emitPheromone()                                           // 4. c. vi.
{
    std::vector<Pheromone*> v = getWorld()->getVector<Pheromone>(getX(), getY());
    int i = 0;
    for( ; i < v.size(); i++)
        if(v[i]->getColonyNum() == this->getColonyNum())
            break;
    
    // If the Pheromone of the appropriate type is not present
    if(i == v.size())
        getWorld()->addActor(new Pheromone(getColonyNum(), this->getWorld() , getX(), getY()));
    else
        v[i]->add(INIT_PHEROMONE);
    return true;
}
bool Grasshopper::eat()
{
    return EnergyHolder::eat(GRASSHOPPER_BITE_SIZE);
}

void Pheromone::add(int amount)
{
    setHitPoints(getHitPoints() + amount);
    if(getHitPoints() > MAX_PHEROMONE)
        setHitPoints(MAX_PHEROMONE);
}

bool Ant::pickUpFood()                                              // 4. c. v.
{
    int res;
    if(ANT_PICK_UP_CAP > FOOD_RESERVE_CAP - m_foodReserve)
    {
        if((res = EnergyHolder::pickUpFood(FOOD_RESERVE_CAP - m_foodReserve)))
        {
            m_foodReserve += res;
            return true;
        }
        return false;
    }else
    {
        if((res = EnergyHolder::pickUpFood(ANT_PICK_UP_CAP)))
        {
            m_foodReserve += res;
            return true;
        }
        return false;
    }
    return false;
}

// 4. x. c. 8.
bool Ant::dangerous(int x, int y)
{
    if(getEnemyAt(x, y))
        return true;
    if(getWorld()->getAt<Poison>(x, y))
        return true;
    return false;
}

void Ant::rotateClockwise()
{
    switch(getDirection())
    {
        case Direction::up: setDirection(Direction::right); break;
        case Direction::right: setDirection(Direction::down); break;
        case Direction::down: setDirection(Direction::left); break;
        case Direction::left: setDirection(Direction::up); break;
    }
}
void Ant::rotateCounterClockwise()
{
    switch(getDirection())
    {
        case Direction::up: setDirection(Direction::left); break;
        case Direction::left: setDirection(Direction::down); break;
        case Direction::down: setDirection(Direction::right); break;
        case Direction::right: setDirection(Direction::up); break;
    }
}

bool Ant::runCommand(const Compiler::Command& c)
{
    bool changedSimulation = false;
    int target = m_instructionPtr+1;      // 4. d. i.
    // will be used to set the pointer to the next instruction at end
    // DO NOT use m_instructionCount until end!!
    
    if(c.opcode == Compiler::moveForward)                   // 4. c. i.
    {
        changedSimulation = true;
        if(move())                                          // 4. c. i. 1.
        {
            unblocked();                                    // 4. c. i. 1. a.
            unbitten();                                     // 4. c. i. 1. b.
            
        }else
        {
            blocked();                                      // 4. c. i. 2.
        }
    }else if(c.opcode == Compiler::rotateClockwise){
        rotateClockwise();
        changedSimulation = true;
    }else if(c.opcode == Compiler::rotateCounterClockwise){
        rotateCounterClockwise();
        changedSimulation = true;
    }else if(c.opcode == Compiler::eatFood)                 // 4. c. ii.
    {
        eat();                                           // 4. c. ii.
        changedSimulation = true;
    }else if(c.opcode == Compiler::dropFood)
    {
        dropFood();                                     // 4. c. iii.
        changedSimulation = true;
    }else if(c.opcode == Compiler::bite)
    {                                         // 4. c. iv.
            bite();
            changedSimulation = true;
    }else if(c.opcode == Compiler::pickupFood)
    {
        pickUpFood();                                    // 4. c. v.
        changedSimulation = true;
    }else if(c.opcode == Compiler::emitPheromone)
    {
        emitPheromone();                                 // 4. c. vi.
            changedSimulation = true;
    }else if(c.opcode == Compiler::faceRandomDirection)
    {
        setDirection(randDir());                            // 4. c. vii.
        changedSimulation = true;
    }else if(c.opcode == Compiler::generateRandomNumber)    // 4. c. viii.
    {
        if(c.operand1 == "0")
        {
            m_randNum = 0;
        }else
        {
            m_randNum = randInt(0, stoi(c.operand1) - 1);
        }
    }else if(c.opcode == Compiler::goto_command)
    {
        target = stoi(c.operand1);                          // 4. c. ix.
    }else if(c.opcode == Compiler::if_command)              // 4. c. x.
    {
        if(stoi(c.operand1) == Compiler::Condition::last_random_number_was_zero)     // 4. c. x. 1.
        {
            if(m_randNum == 0)
            {
                target = stoi(c.operand2);                  // 4. d. ii.
            }
        }else if(stoi(c.operand1) == Compiler::Condition::i_am_carrying_food)        // 4. c. x. 2.
        {
            if(m_foodReserve > 0)
            {
                target = stoi(c.operand2);              // 4. d. ii.
            }
        }else if(stoi(c.operand1) == Compiler::Condition::i_am_hungry)               // 4. c. x. 3.
        {
            if(getHitPoints() <= 25)
            {
                target = stoi(c.operand2);              // 4. d. ii.
            }
        }else if(stoi(c.operand1) == Compiler::Condition::i_am_standing_with_an_enemy)             // 4. c. x. 4.
        {
            Insect* enemy = getEnemyAt(getX(), getY());
            if(enemy)
            {
                target = stoi(c.operand2);              // 4. d. ii.
            }
        }else if(stoi(c.operand1) == Compiler::Condition::i_am_standing_on_food)     // 4. c. x. 5.
        {
            if(getWorld()->getAt<Food>(getX(), getY()))
            {
                target = stoi(c.operand2);              // 4. d. ii.
            }
        }else if(stoi(c.operand1) == Compiler::Condition::i_am_standing_on_my_anthill)             // 4. c. x. 6.
        {
            
            if(onMyAnthill())
            {
                target = stoi(c.operand2);              // 4. d. ii.
            }
        }else if(stoi(c.operand1) == Compiler::Condition::i_smell_pheromone_in_front_of_me)        // 4. c. x. 7.
        {
            int x = getX();
            int y = getY();
            getPosInDir(x, y, getDirection());
            if(getWorld()->getAt<Pheromone>(x, y) != nullptr
               && getWorld()->getAt<Pheromone>(x, y)->getColonyNum() == getColonyNum())
            {
                target = stoi(c.operand2);                  // 4. d. ii.
            }
        }else if(stoi(c.operand1) == Compiler::Condition::i_smell_danger_in_front_of_me)           // 4. c. x. 8.
        {
            int x = getX();
            int y = getY();
            getPosInDir(x, y, getDirection());
            if(dangerous(getX(), getY()))
            {
                target = stoi(c.operand2);                  // 4. d. ii.
            }
        }else if(stoi(c.operand1) == Compiler::Condition::i_was_bit)                 // 4. c. x. 9.
        {                                                   // WARNING: Not too sure this is exactly right, probably
            if(isBitten())
            {
                target = stoi(c.operand2);                  // 4. d. ii.
            }
        }else if(stoi(c.operand1) == Compiler::Condition::i_was_blocked_from_moving)               // 4. c. x. 10.
        {
            if(isBlocked())
                target = stoi(c.operand2);                  // 4. d. ii.
        }
    }
    m_instructionPtr = target;                              // 4. d.
    return changedSimulation;
}


/******************************* ANTHILL *****************************************/

void AntHill::giveBirth()
{
    Ant* newAnt = new Ant(getWorld(), getX(), getY(), this->getColonyNum(), m_compiler);
    getWorld()->addActor(newAnt);
    getWorld()->addAnt(this->getColonyNum());
    setHitPoints(getHitPoints() - BIRTH_GIVING_ENERGY_EXPENDED);
}

bool AntHill::doSomething()
{
    if(EnergyHolder::doSomething()) // 1.
    {
        return true;        // 2.
    }
    if(eat(ANT_HILL_BITE_SIZE))
    {
        
        return true;
    }            // 3.
    
    if(getHitPoints()  >= BIRTH_GIVING_ENERGY_NEEDED)         // 4.
    {
        giveBirth();
    }
    return false;
}

/***************************** BABYGRASSHOPPER ************************************/

bool BabyGrasshopper::doSomething()
{
    if(Insect::doSomething())
    {
        return true;                                // 1. 2. 3. 4. 5.
    }
    
    if(getHitPoints()                               // 4. 5.
       >= INIT_ADULT_GRASSHOPPER_HEALTH)
    {
        getWorld()->addActor(new AdultGrasshopper(getWorld(), getX(), getY()));
        this->kill();
        return true;
    }
    
    Grasshopper::doSomething();
    
    return false;
}

/********************************* INSECT *****************************************/

bool Insect::move()
{
    // If attempt was successful return true, otherwise false
    int x = getX();
    int y = getY();
    getPosInDir(x, y, getDirection());
    Actor* host;
    if((host = getWorld()->getAt<Pebble>(x, y)))
    {
        m_blocked = true;
        return false;
    }
    this->moveTo(x, y);
    m_bitten = false;
    return true;
}

bool Insect::doSomething()
{
    
    if(getWorld()->getAt<Pool>(getX(), getY()) == nullptr)
    {
        unstun();
    }
    if(EnergyHolder::doSomething()) return true;    // 1. 2.
    if(m_sleepTicks > 0)                            // 3.
    {
        m_sleepTicks--;                             // 3. a.
        return true;                                // 3. c.
    }
    return false;
}

bool Insect::zap(int amt)
{
    m_bitten = true;
    setHitPoints(getHitPoints() - amt);
    if(getHitPoints() <= 0)
    {
        setHitPoints(0);
        kill();
    }
    return true;
}

void Insect::poison()
{
    setHitPoints(getHitPoints() - POISONED);
    if(getHitPoints() <= 0)
        kill();
}

void Insect::kill()
{
    Actor::kill();
    getWorld()->addFood(getX(), getY(), CORPSE_FOOD);
}

/****************************** GRASSHOPPER **********************************/



bool Grasshopper::doSomething()
{
    // 1. 2. 3.
    if(Insect::eat(GRASSHOPPER_BITE_SIZE))              // 6.
    {
        if(randInt(0, 1) == 0)
        {
            setSleep(GRASSHOPPER_SLEEP);                // 7.
            return true;
        }
    }
    // 8.
    if(m_distLeft <= 0)
    {
        setDirection(randDir());
        m_distLeft = randInt(MIN_GRASSHOPPER_STEP, MAX_GRASSHOPPER_STEP);
    }
    
    // 9. 10.
    if(!move()) 
    {
        m_distLeft = 0;
    }
    else
    {
       m_distLeft--;
    }
    setSleep(GRASSHOPPER_SLEEP);
    return false;
}


Insect* Grasshopper::getEnemyAt(int x, int y)
{
    vector<Insect*> res = getWorld()->getVector<Insect>(x, y);
    auto it = res.begin();
    for(; it != res.end() && (*it) == this; ++it)
        ;
    if(it == res.end())
        return nullptr;
    return *it;
}
void Ant::kill()
{
    EnergyHolder::dropFood(CORPSE_FOOD);
    getWorld()->decAnt(getColonyNum());
    Actor::kill();
}

EnergyHolder::EnergyHolder(int colonyNum, StudentWorld* world,
             int imageId, int x, int y,
             Direction dir, int graphDepth, int hitPoints)
: Actor(world, imageId, x, y, dir, graphDepth)
{
    m_colonyNum = colonyNum;
    m_hitPoints = hitPoints;
}

void EnergyHolder::setHitPoints(int val) {
    m_hitPoints = val;
}

/***************************** DAMAGER *****************************************/

bool Damager::doSomething()
{
    vector<Insect*> v = getWorld()->getVector<Insect>(getX(), getY());
    
    for(int i = 0; i < v.size(); i++)
    {
        bool isNew = true;
        for(int j = 0; j < m_damagedInsects.size(); j++)
            if(v[j] == v[i])
                isNew = false;
        
        if(isNew)
            damage(v[i]);
        else damageOld(v[i]);
    }
    m_damagedInsects = v;
    return false;
}

/***************************** POISON ******************************************/



/************************************ POOL **************************************/


/****************************** ENERGYHOLDER ************************************/


bool EnergyHolder::doSomething()
{
    setHitPoints(getHitPoints() - 1);   // 1.
    if(m_hitPoints <= 0)                // 2.
    {
        kill();
        return true;
    }
    return false;
}

// 4. c. iii.
void EnergyHolder::dropFood(int amount)
{
    getWorld()->addFood(getX(), getY(), amount);
}

int EnergyHolder::pickUpFood(int biteSize)
{
    Food* food = getWorld()->getAt<Food>(getX(), getY());
    if(food == nullptr)
    {
        return 0;
    }
    if(food->getHitPoints() > biteSize)
    {
        food->setHitPoints(food->getHitPoints()-biteSize);
        return biteSize;
        
    }else
    {
        int result = food->getHitPoints();
        food->setHitPoints(0);
        return result;
    }
}

bool EnergyHolder::eat(int cap)
{
    int res = pickUpFood(cap);
    if(res > 0)
    {
        setHitPoints(getHitPoints() + res);
        return true;
    }
    return false;
}
