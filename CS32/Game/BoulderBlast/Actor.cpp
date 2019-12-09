#include "Actor.h"
#include "GameWorld.h"
#include "StudentWorld.h"
#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include <type_traits>
#include <cassert>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// move the (x, y) variables in direction dir
void dirChange(int &x, int &y, GraphObject::Direction dir)
{
    switch(dir)
    {
        case GraphObject::Direction::up:    y++; break;
        case GraphObject::Direction::down:  y--; break;
        case GraphObject::Direction::left:  x--; break;
        case GraphObject::Direction::right: x++; break;
    }
}

void AmmoGoodie::swallowed()
{
    if(isAlive())
    {
        getWorld()->increasePlayerAmmo(20);
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
    setVisible(false);
    setAlive(false);
}

void AmmoGoodie::doSomething()
{
    if(!isAlive()) return;
    if(getWorld()->getPlayerX() == getX()
       && getWorld()->getPlayerY() == getY())
    {
        swallowed();
    }
}

void RestoreHealthGoodie::swallowed()
{
    if(isAlive())
    {
        getWorld()->increaseScore(500);
        getWorld()->restorePlayerHealth();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
    setVisible(false);
    setAlive(false);
}

void RestoreHealthGoodie::doSomething()
{
    if(!isAlive()) return;
    if(getWorld()->getPlayerX() == getX() && getWorld()->getPlayerY() == getY())
    {
        swallowed();
    }
}

// return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        std::swap(max, min);
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

void Hole::doSomething()
{
    if(!isAlive()) return;
    
    // if there is a boulder on the hole swallow the boulder, kill the hole
    if(getWorld()->getBoulderAt(getX(), getY()) != nullptr)
    {
        setAlive(false);
        (getWorld()->getBoulderAt(getX(), getY()))->swallowed();
        setVisible(false);
    }
}

// handle the situation when a player gets damaged
void Player::damage()
{
    // kill player if no health left
    if(getHitPoints() < 1)
    {
        setAlive(false);
        setVisible(false);
    }else
        setHitPoints(getHitPoints()-1);     // otherwise reduce players health
}

// check if the bullet hit a target, then give approapriate damage
void Bullet::checkDamage()
{
    // if a bullet hit a robot different from the robot that fired the bulled then damage
    if(dynamic_cast<Robot*>(m_source) == nullptr
       && getWorld()->getRobotAt(getX(), getY()) != nullptr)
    {
        getWorld()->getRobotAt(getX(), getY())->damage();
        setAlive(false);
        setVisible(false);
    }
    // if a bullet hit the player and wasn't fired by the player then damage the player
    else if(dynamic_cast<Player*>(m_source) == nullptr
        && getWorld()->getPlayerX() == getX()
        && getWorld()->getPlayerY() == getY())
    {
        getWorld()->getPlayer()->damage();
        setAlive(false);
        setVisible(false);
    }
    // if the bullet hits a boulder, damage the boulder
    else if(getWorld()->getAt<Boulder>(getX(), getY()) != nullptr)
    {
        getWorld()->getAt<Boulder>(getX(), getY())->damage();
        setAlive(false);
        setVisible(false);
    }
    // if the bullet hits a wall, kill the bullet
    else if(getWorld()->getAt<Wall>(getX(), getY()) != nullptr)
    {
        setAlive(false);
        setVisible(false);
    }
}

void Bullet::doSomething()
{
    if(!isAlive()) return;
    
    checkDamage();
    
    moveInDirection(getDirection());
    
    if(!isAlive()) return;
    
    checkDamage();  // check damage again for the new position of the bullet
}

void Exit::doSomething()
{
    // create the exit if all Blue Jewels are collected
    if(getWorld()->jewelsCollected() && !isVisible())
    {
        setVisible(true);
        setAlive(true);
        getWorld()->playSound(SOUND_REVEAL_EXIT);
    }
    
    // if player reaches the exit then finish level
    if(getWorld()->getAt<Player>(getX(), getY()) != nullptr
    && isAlive())
    {
            getWorld()->playSound(SOUND_FINISHED_LEVEL);
            getWorld()->advanceToNextLevel();
            getWorld()->increaseScore(2000);
    }
}

SnarlBot::SnarlBot(int ID, int x, int y, StudentWorld* world, Direction dir)
: Robot(ID, x, y, world, dir)
{
    setVisible(true);
    setHitPoints(10);
    
    m_ticks = (28 - getWorld()->getLevel()) / 4;
    if(m_ticks < 3)
        m_ticks = 3;
}

void BlueJewel::swallowed()
{
    if(isAlive())
    {
        getWorld()->increaseScore(50);
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
    setVisible(false);
    setAlive(false);
}

// return random direction
Actor::Direction randDir()
{
    int n = randInt(1, 4);
    switch(n)
    {
        case 1: return Actor::Direction::down; break;
        case 2: return Actor::Direction::up; break;
        case 3: return Actor::Direction::left; break;
        case 4: return Actor::Direction::right; break;
    }
    return Actor::Direction::up;
}

Actor::Direction nextDir(Actor::Direction dir)
{
    switch(dir)
    {
        case Actor::Direction::up: return Actor::Direction::left; break;
        case Actor::Direction::left: return Actor::Direction::down; break;
        case Actor::Direction::down: return Actor::Direction::right; break;
        case Actor::Direction::right: return Actor::Direction::up; break;
    }
    return Actor::Direction::up;
}

bool AngryKleptoBot::shoot()
{
    bool shot = false;
    int x = getX(), y = getY();
    dirChange(x, y, getDirection());
    
    if(getWorld()->isClearX(getY(), getWorld()->getPlayerY(), getX())
       && getX() == getWorld()->getPlayerX()
       && (getDirection() == Direction::up
           || getDirection() == Direction::down))
    {
        if((getWorld()->getPlayerY() > getY() && getDirection() == Direction::up)
           || (getWorld()->getPlayerY() < getY() && getDirection() == Direction::down))
        {
            int x = getX(), y = getY();
            // dirChange(x, y, getDirection());
            getWorld()->add(new Bullet(x, y, getWorld(), getDirection(), this));
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            shot = true;
        }
    }
    
    if(getWorld()->isClearY(getX(), getWorld()->getPlayerX(), getY())
       && getY() == getWorld()->getPlayerY()
       && (getDirection() == Direction::right || getDirection() == Direction::left))
    {
        if((getWorld()->getPlayerX() > getX() && getDirection() == Direction::right)
           || (getWorld()->getPlayerX() < getX() && getDirection() == Direction::left))
        {
            int x = getX(), y = getY();
            // dirChange(x, y, getDirection());
            getWorld()->add(new Bullet(x, y, getWorld(), getDirection(), this));
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            shot = true;
        }
        
    }
    return shot;
}

void AngryKleptoBot::getNewDirection()
{
    switch (getDirection())
    {
        case Direction::up:
            setDirection(Direction::down);
            break;
        case Direction::down:
            setDirection(Direction::up);
            break;
        case Direction::left:
            setDirection(Direction::right);
            break;
        case Direction::right:
            setDirection(Direction::left);
            break;
    }
}

void AngryKleptoBot::doSomething()
{
    if(!isAlive()) return;
    bool moved = false;
    bool shot = false;
    bool goodie = false;
    int x = getX(), y = getY();
    dirChange(x, y, getDirection());
    
    // first check if we are on the move tick
    if(getWorld()->getTicks() % m_ticks == 0)
    {
        if(getWorld()->isGoodie(getX(), getY())
           && eatGoodie()){}                     // attempt to eat a goodie
        else if(shoot()){}                       // attempt to shoot if there was no goodie eaten
        else if(getWorld()->isEmpty(x, y))
            moveInDirection(getDirection());     // attempt to move if it didn't shoot or collect a goodie
        else
            getNewDirection();                   // if way is blocked get a new direction
    }

}

// find new direction in which the KleptoBot can move freely
void KleptoBot::findNewDirection()
{
    m_distBeforeTurn = randInt(1, 6);
    m_ticksToTurn = m_distBeforeTurn;
    int c = 0;
    
    while(c < 4)
    {
        c++;
        setDirection(nextDir(getDirection()));
        int x = getX(), y = getY();
        dirChange(x, y, getDirection());
        if(m_ticksToTurn > 0 && getWorld()->isEmpty(x, y))
        {
            m_ticksToTurn--;
            moveInDirection(getDirection());
            break;
        }
    }
}

void KleptoBot::moveKleptoBot()
{
    int x = getX(), y = getY();
    dirChange(x, y, getDirection());
    
    // attempt to move in the set direction
    if(m_ticksToTurn > 0 && getWorld()->isEmpty(x, y))
    {
        m_ticksToTurn--;
        moveInDirection(getDirection());
    }
    // if direction change is in order then do that
    else
        findNewDirection();
}

bool KleptoBot::eatGoodie()
{
    m_goodie = getWorld()->getGoodie(getX(), getY());
    m_goodie->setVisible(false);
    m_goodie->setAlive(false);
    
    return true;
}

void KleptoBot::doSomething()
{
    if(!isAlive()) return;
    
    // check if KleptoBot can move
    if(getWorld()->getTicks() % m_ticks == 0)
    {
        if(getWorld()->isGoodie(getX(), getY())) eatGoodie();   // if there is a goodie let the eatGoodie() handle it
        else moveKleptoBot();                                   // otherwise move the KleptoBot
    }
}

void KleptoBot::damage()
{
    if(getHitPoints()>0)
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
        setHitPoints(getHitPoints()-1);
    }else
    {
        //Drop goodie
        getWorld()->playSound(SOUND_ROBOT_DIE);
        setVisible(false);
        setAlive(false);
        getWorld()->increaseScore(10);
    }
}

KleptoBot::KleptoBot(int ID, int x, int y, StudentWorld* world)
: Robot(ID, x, y, world, Direction::right)
{
    setVisible(true);
    setHitPoints(5);
    m_ticks = (38 - getWorld()->getLevel()) / 4;
    
    if(m_ticks < 3) m_ticks = 3;
    
    m_distBeforeTurn = randInt(1, 6);
    m_ticksToTurn = m_distBeforeTurn;
}

void SnarlBot::damage()
{
    getWorld()->playSound(SOUND_ROBOT_IMPACT);
    if(getHitPoints() == 1)
    {
        setAlive(false);
        setVisible(false);
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(100);
    }
    if(getHitPoints()>0)
    {
        setHitPoints(getHitPoints()-1);
    }
}

void Factory::doSomething()
{
    if(getWorld()->getKleptoBotAt(getX(), getY()) == nullptr && getWorld()->countKleptoBots(getX(), getY()) < 3)
    {
        int random = randInt(1, 50);
        if(random == 1)
        {
            if(!m_angry)
                getWorld()->add(new KleptoBot(IID_KLEPTOBOT, getX(), getY(), getWorld()));
            else
                getWorld()->add(new AngryKleptoBot(getX(), getY(), getWorld()));
        }
    }
}

// check if SnarlBot can shoot the player
bool SnarlBot::checkShot()
{
    // vertical snarlbots
    if(getWorld()->isClearX(getY(), getWorld()->getPlayerY(), getX())
       && getX() == getWorld()->getPlayerX()
       && (getDirection() == Direction::up || getDirection() == Direction::down))
    {
        if((getWorld()->getPlayerY() > getY() && getDirection() == Direction::up)
           || (getWorld()->getPlayerY() < getY() && getDirection() == Direction::down))
        {
            int x = getX(), y = getY();
            getWorld()->add(new Bullet(x, y, getWorld(), getDirection(), this));
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            return true;
        }
    }
    
    //horizontal snarlbots
    if(getWorld()->isClearY(getX(), getWorld()->getPlayerX(), getY())
       && getY() == getWorld()->getPlayerY()
       && (getDirection() == Direction::right || getDirection() == Direction::left))
    {
        if((getWorld()->getPlayerX() > getX() && getDirection() == Direction::right)
           || (getWorld()->getPlayerX() < getX() && getDirection() == Direction::left))
        {
            int x = getX(), y = getY();
            getWorld()->add(new Bullet(x, y, getWorld(), getDirection(), this));
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            return true;
        }
    }
    return false;
}

void SnarlBot::getNewDirection()
{
    switch (getDirection())
    {
        case Direction::up:
            setDirection(Direction::down);
            break;
        case Direction::down:
            setDirection(Direction::up);
            break;
        case Direction::left:
            setDirection(Direction::right);
            break;
        case Direction::right:
            setDirection(Direction::left);
            break;
    }
}

void SnarlBot::doSomething()
{
    if(!isAlive()) return;
    bool moved = false;
    bool shot = false;
    
    // check if it is time for the SnarlBot to move
    if(getWorld()->getTicks() % m_ticks == 0)
    {
        int x = getX(), y = getY();
        dirChange(x, y, getDirection());
        
        if(checkShot()){}
        else if(getWorld()->isEmpty(x, y))
        {
            moveInDirection(getDirection());
        }
        else
            getNewDirection();
    }
}

void Boulder::damage()
{
    if(getHitPoints() > 0)
        decHitPoints();
    else
    {
        setVisible(false);
        setAlive(false);
    }
}

void Boulder::doSomething()
{
    if(getWorld()->getPlayerX() == getX() && getWorld()->getPlayerY() == getY()) moveInDirection(getWorld()->getPlayerDirection());
}

bool Boulder::pushed(Direction dir)
{
    if(!isAlive()) return true;
    int moveX, moveY;
    moveX = getX();
    moveY = getY();
    switch(dir)
    {
        case Direction::up: moveY++; break;
        case Direction::down: moveY--; break;
        case Direction::left: moveX--; break;
        case Direction::right: moveX++; break;
        case Direction::none: break;
    }
    
    if( getWorld()->getType(getWorld()->getObjAt(moveX, moveY)) == IID_HOLE || this->getWorld()->isEmpty(moveX, moveY) || this->getWorld()->isGoodie(moveX, moveY))
    {
        return true;
    }
    return false;
}

void Player::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    
    int ch;
    if(getWorld()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                setDirection(Direction::left);
                if(getWorld()->isStepable(getX()-1, getY())) moveTo(getX()-1, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(Direction::right);
                if(getWorld()->isStepable(getX()+1, getY())) moveTo(getX()+1, getY());
                break;
            case KEY_PRESS_UP:
                setDirection(Direction::up);
                if(getWorld()->isStepable(getX(), getY()+1)) moveTo(getX(), getY()+1);
                break;
            case KEY_PRESS_DOWN:
                setDirection(Direction::down);
                if(getWorld()->isStepable(getX(), getY()-1)) moveTo(getX(), getY()-1);
                break;
            case KEY_PRESS_SPACE:
                if(getAmmo()>0)
                {
                    getWorld()->add(new Bullet(getX(), getY(), getWorld(), getDirection(), this));
                    decAmmo();
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
        }
    }
}
