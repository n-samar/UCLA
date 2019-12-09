#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "GameConstants.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class Player;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
        m_p = nullptr;
        m_tickCount = 0;
	}

    ~StudentWorld()
    {
        m_actors.clear();
        delete m_p;
        delete this;
    }
    Player* getPlayer();
    Robot* getRobotAt(int x, int y);
    
    int getPlayerX()
    {
        return m_p->getX();
    }
    
    bool isExit(int x, int y);
    
    
    KleptoBot* getKleptoBotAt(int x, int y);
    
    static int getType(Actor* a);
    
    int getPlayerY()
    {
        return m_p->getY();
    }
    
    Actor::Direction getPlayerDirection()
    {
        return m_p->getDirection();
    }
    
    int countKleptoBots(int x, int y);
    
    Boulder* getBoulderAt(int x, int y);
    
    template <class ActorType>
    int count();
    
    bool isStepable(int x, int y);
    
    bool isEmpty(int x, int y);
    
    void updateDisplayText();
    
    bool isGoodie(int x, int y);
    
    bool isBulletBarrier(Actor* a);
    
    bool isClear(int var1, int var2, int const1, bool vertical);
    
    Goodie* getGoodie(int x, int y);
    
    void decPlayerAmmo()
    {
        m_p->decAmmo();
    }
    
    Actor* getObjAt(int x, int y);
    
    std::string getLevelString(int level);
    
    template <class ActorType>
    int countAt(int x, int y);
    
    int init();
    
    // return pointer to the alive specified Actor object with coordinates (x, y)
    // if no alive object of specified type is present return nullptr
    template <class ActorType>
    ActorType* getAt(int x, int y)
    {
        // iterate through all objects in the game world
        for(int i = 0; i < m_actors.size(); i++)
        {
            if(m_actors[i]->getX() == x                     // check coordinate
               && m_actors[i]->getY() == y                  // check coordinate
               && dynamic_cast<ActorType*>(m_actors[i])
               != nullptr                                   // check type
               && m_actors[i]->isAlive())                   // check alive
                
                return dynamic_cast<ActorType*>(m_actors[i]);
        }
        return nullptr;
    }
    
    bool jewelsCollected();
    
    void restorePlayerHealth();
    
    void increasePlayerAmmo(int amount);
    
    int getTicks()
    {
        return m_tickCount;
    }
    
    bool isClearX(int y1, int y2, int x);
    
    bool isClearY(int x1, int x2, int y);
    
    int move();
    
    void cleanUp();
    
    int loadLevel(int level);
    
    int getBonus()
    {
        return m_bonus;
    }
    
    int getScore()
    {
        return m_score;
    }
    
    void add(Actor* a);
    
private:
    std::vector<Actor*> m_actors;
    Player* m_p;
    std::vector<Actor*>::iterator m_nActors;
    int m_score;
    int m_bonus;
    int m_tickCount;
};

#endif // STUDENTWORLD_H_
