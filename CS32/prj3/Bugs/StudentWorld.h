#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Field.h"
#include <string>
#include <map>
#include <list>
#include <vector>

class Actor;
class Food;
class Pheromone;
class AntHill;
class Insect;
class Ant;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir),
    m_winner(-1)
    {
        for(int i = 0; i < 4; i++)
        {
            m_antCounts[i] = 0;
            m_winnable[i] = false;
        }
    }

    ~StudentWorld() {cleanUp();}
    void addAnt(int colNum) {m_antCounts[colNum]++;}
    void decAnt(int colNum) {}
    
    int getTicks(){
        return m_tickCount;
    }
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    

    void getPosWithinRadius(int &x, int &y, int rad);
    void addFood(int x, int y, int amount);

    Pheromone* getPheromoneAt(int x, int y, int typeId);

    void addActor(Actor* actor);
    
    template<class T>
    std::list<Actor*>::iterator getAt(int x, int y,
                                      typename std::list<Actor*>::iterator it,
                                      Actor* me = nullptr);
    
    template<class T>
    T* getAt(int x, int y, Actor* me = nullptr);        // TODO: Why the heck am I using me?
    
    template<class T>
    std::vector<T*> getVector(int x, int y, Actor* me = nullptr);
    
private:
    void readActors();
    int m_anthillNum;
    Field f;
    void setDisplayText();
    void getWinner();
    std::string format(int ticks, int* anthills, int winning);
    void askActorsToDoSomething();
    void updateContainer();
    int compileBugs();
    int m_tickCount = 0;
    AntHill* m_ahArr[4];
    int m_antCounts[4];
    bool m_winnable[4];
    int m_winner;
    // Map that contains <position, actor list> pairs with actor list representing
    // a linked list of all the actors at the position.
    std::map< std::pair<int, int> ,std::list<Actor*> > m_actorMap;
    Compiler* m_compArr[4];
};

template<class T>
T* StudentWorld::getAt(int x, int y, Actor* me)
{
    std::list<Actor*>::iterator it = getAt<T>(x, y, m_actorMap[std::make_pair(x, y)].begin(), me);
    
    if(it != m_actorMap[std::make_pair(x, y)].end())
        return dynamic_cast<T*>(*it);
    return nullptr;
}

template<class T>
std::list<Actor*>::iterator StudentWorld::getAt(int x, int y, typename std::list<Actor*>::iterator it, Actor* me)
{
    for(;
        it != m_actorMap[std::make_pair(x, y)].end(); ++it)
        if(dynamic_cast<T*>(*it))
            return it;
    return it;
}

template<class T>
std::vector<T*> StudentWorld::getVector(int x, int y, Actor* me)
{
    std::vector<T*> v;
    for(auto it = getAt<T>(x, y, m_actorMap[std::make_pair(x, y)].begin(), me);
        it != m_actorMap[std::make_pair(x, y)].end(); it = getAt<T>(x, y, ++it, me))
    {
        v.push_back(dynamic_cast<T*>(*it));
    }
    return v;
}
#endif // STUDENTWORLD_H_
