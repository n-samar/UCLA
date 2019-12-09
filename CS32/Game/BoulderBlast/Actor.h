#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, int x, int y, StudentWorld* world,
          Direction startDirection) : GraphObject(imageID, x, y, startDirection)
    {
        m_world = world;
        m_alive = true;
    }
    ~Actor()
    {
        
    }
    
    virtual void doSomething()
    {
        
    }
    
    bool isAlive(){
        return m_alive;
    }
    
    void setAlive(bool val)
    {
        m_alive = val;
    }
    
    StudentWorld* getWorld()
    {
        return m_world;
    }
    
    virtual void attacked()
    {
        
    }
    
    int virtual getType()=0;
    
    // Moves the Actor in specified direction
    void moveInDirection(Direction dir)
    {
        switch(dir)
        {
            case Direction::up:
                moveTo(getX(), getY()+1); break;
            case Direction::down:
                moveTo(getX(), getY()-1); break;
            case Direction::left:
                moveTo(getX()-1, getY()); break;
            case Direction::right:
                moveTo(getX()+1, getY()); break;
        }
    }
    
private:
    int m_row;                  // row position of the Actor
    int m_col;                  // column position of the Actor
    bool m_alive;
    StudentWorld* m_world;      // pointer to the game world
};

// ********************** OTHER *********************************

class Robot;

class Bullet : public Actor{
public:
    Bullet(int x, int y, StudentWorld* world, Direction dir, Actor* source)
    : Actor(IID_BULLET, x, y, world, dir)
    {
        m_source = source;
        setVisible(true);
    }
    
    void checkDamage();
    
    int getType()
    {
        return IID_BULLET;
    }
    
    void doSomething();
    
private:
    Actor* m_source;
};


class Wall : public Actor
{
public:
    Wall(int x, int y, StudentWorld* world)
    : Actor(IID_WALL, x, y, world, Direction::none)
    {
        setVisible(true);
    }
    
    int getType()
    {
        return IID_WALL;
    }
    
    void doSomething()
    {
        
    }
    
private:
};


class Hole : public Actor
{
public:
    Hole(int x, int y, StudentWorld* world)
    : Actor(IID_HOLE, x, y, world, Direction::none)
    {
        setVisible(true);
    }
    
    int getType()
    {
        return IID_HOLE;
    }
    
    void doSomething();
    
private:
};


class Exit : public Actor
{
public:
    Exit(int x, int y, StudentWorld* world)
    : Actor(IID_EXIT, x, y, world, Direction::none)
    {
        setVisible(false);
        setAlive(false);
    }
    
    int getType()
    {
        return IID_EXIT;
    }
    
    void doSomething();
    
private:
};


class Boulder : public Actor
{
public:
    Boulder(int x, int y, StudentWorld* world)
    : Actor(IID_BOULDER, x, y, world, Direction::none)
    {
        m_hitPoints = 10;
        setVisible(true);
    }
    
    int getType()
    {
        return IID_BOULDER;
    }
    
    bool pushed(Direction dir);
    int getHitPoints()
    {
        return m_hitPoints;
    }
    
    void decHitPoints()
    {
        m_hitPoints-=2;
    }
    
    
    void doSomething();
    
    void damage();
    
    void swallowed()
    {
        setVisible(false);
        this->setAlive(false);
    }
    
private:
    int m_hitPoints;
};


// ******************** PLAYER  ***********************************


class Player : public Actor
{
public:
    Player(int x, int y, StudentWorld* world)
    : Actor(IID_PLAYER, x, y, world, Direction::right)
    {
        m_hitPoints = 20;
        m_ammo = 20;
        setVisible(true);
        setDirection(Direction::right);
    }
    void restoreHealth()
    {
        m_hitPoints = 20;
    }
    
    int getType()
    {
        return IID_PLAYER;
    }
    
    void addAmmo(int amount)
    {
        m_ammo+=amount;
    }
    
    void decAmmo()
    {
        m_ammo--;
    }
    
    void setHitPoints(int n)
    {
        m_hitPoints = n;
    }
    
    int getHitPoints()
    {
        return m_hitPoints;
    }
    
    int getHealth()
    {
        return m_hitPoints;
    }
    int getAmmo()
    {
        return m_ammo;
    }
    void damage();
    
    void doSomething();
    
private:
    int m_hitPoints;
    int m_ammo;
    StudentWorld* m_world;
};




// ******************** GOODIES ***********************************

class Goodie : public Actor
{
public:
    Goodie(int ID, int x, int y, StudentWorld* world)
    : Actor(ID, x, y, world, Direction::none){}
    void doSomething()
    {
        
    }
    
    int virtual getType()=0;
    
    virtual void swallowed()
    {
        
    }
    
private:
};


class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(int x, int y, StudentWorld* world)
    : Goodie(IID_EXTRA_LIFE, x, y, world)
    {
        setVisible(true);
    }
    
    int getType()
    {
        return IID_EXTRA_LIFE;
    }
    
    void doSomething()
    {
        
    }
    
    void swallowed()
    {
        
    }
    
private:
};


class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(int x, int y, StudentWorld* world)
    : Goodie(IID_RESTORE_HEALTH,x, y, world)
    {
        setVisible(true);
    }
    
    int getType()
    {
        return IID_RESTORE_HEALTH;
    }
    
    void doSomething();
    
    void swallowed();
    
private:
};

class AmmoGoodie : public Goodie
{
public:
    AmmoGoodie(int x, int y, StudentWorld* world)
    : Goodie(IID_AMMO ,x, y, world)
    {
        setVisible(true);
    }
    
    int getType()
    {
        return IID_AMMO;
    }
    
    void doSomething();
    
    void swallowed();
    
private:
};

class BlueJewel : public Goodie
{
public:
    BlueJewel(int x, int y, StudentWorld* world)
    : Goodie(IID_JEWEL, x, y, world)
    {
        setVisible(true);
    }
    
    int getType()
    {
        return IID_JEWEL;
    }
    
    void swallowed();
    
    void doSomething()
    {
        
    }
    
private:
};

// ******************** ROBOTS ************************************

class Robot : public Actor
{
public:
    Robot(int ID, int x, int y, StudentWorld* world, Direction dir)
    : Actor( ID, x, y, world, dir)
    {
        setVisible(true);
    }
    void doSomething()
    {
        
    }
    
    int virtual getType()=0;
    
    virtual void damage()
    {
        
    }
    
    int getHitPoints()
    {
        return m_hitPoints;
    }
    
    void setHitPoints(int n)
    {
        m_hitPoints = n;
    }
    
private:
    int m_hitPoints;
};

class SnarlBot : public Robot
{
public:
    SnarlBot(int ID, int x, int y, StudentWorld* world, Direction dir);
    
    int getType()
    {
        return IID_SNARLBOT;
    }
    
    bool checkShot();
    
    void doSomething();
    
    void getNewDirection();
    
    void damage();
    
private:
    int m_ticks;
};


class KleptoBot : public Robot
{
public:
    KleptoBot(int ID, int x, int y, StudentWorld* world);
    
    void findNewDirection();
    
    bool eatGoodie();
    
    void moveKleptoBot();
    
    int getType()
    {
        return IID_KLEPTOBOT;
    }
    
    virtual void doSomething();
    
    void damage();
    
private:
    bool m_hasGoodie;
    int m_distBeforeTurn;
    int m_ticksToTurn;
protected:
    int m_ticks;
    Goodie* m_goodie;
};


class AngryKleptoBot : public KleptoBot
{
public:
    AngryKleptoBot(int x, int y, StudentWorld* world)
    : KleptoBot(IID_ANGRY_KLEPTOBOT, x, y, world)
    {
        setVisible(true);
    }
    
    bool shoot();
    
    void getNewDirection();
    
    int getType()
    {
        return IID_ANGRY_KLEPTOBOT;
    }
    
    void doSomething();
    
private:
};

// ******************** FACTORIES *********************************

class Factory : public Actor
{
public:
    Factory(int x, int y, StudentWorld* world, bool angry)
    : Actor(IID_ROBOT_FACTORY,  x, y, world, Direction::none)
    {
        m_angry = angry;
        setVisible(true);
    }
    
    int getType()
    {
        return IID_ROBOT_FACTORY;
    }
    
    void doSomething();
    
private:
    bool m_angry;
};
#endif // ACTOR_H_
