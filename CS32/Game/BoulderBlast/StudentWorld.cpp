#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"
#include <vector>
#include <cmath>
#include <string>
#include "GameConstants.h"


using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


// formats a 'number' as a string of length at least 'strLen'
// if the number has more than strLength digits return the number as a string
// otherwise add 'filler' to the right of the 'number' and return that string (addSpaces(123, 5, '0') ->'00123')
string addSpaces(int number, int strLength, char filler)
{
    string result = to_string(number);
    int spaces = strLength-result.size();   // determine the amount of filler needed
    
    if(spaces>0)
    {
        string resultWithSpaces(spaces, filler);    // create string with as much filler as needed
        resultWithSpaces.append(result);            // add the number to the end of the string
        return resultWithSpaces;
    }
    
    return result;  //if the number is big enough just return the number
}

string addZeros(int n, int num){
    return addSpaces(n, num, '0');
}

// returns number of alive ActorType objects in game world
template <class ActorType>
int StudentWorld::count()
{
    int count = 0;
    
    for(int i = 0; i < m_actors.size(); i++)
    {
        if(dynamic_cast<ActorType*>(m_actors[i])
           != nullptr                    // check type
           && m_actors[i]->isAlive())    // check alive
            count++;
    }
    return count;
}

void StudentWorld::increasePlayerAmmo(int amount)
{
    m_p->addAmmo(amount);
}

KleptoBot* StudentWorld::getKleptoBotAt(int x, int y)
{
    return getAt<KleptoBot>(x, y);
}

// format the display bar of the game
string formatDisplay(int score, int level, int lives, int health, int ammo, int bonus)
{
    string s;
    s.append("Score: ");
    s.append(addZeros(score, 7));   // format score to be 7 characters wide
    s.append("  Level: ");
    s.append(addZeros(level, 2));   // format level to be 2 characters wide
    s.append("  Lives:   ");
    s.append(to_string(lives));     // convert lives to string
    s.append("  Health:  ");
    s.append(addSpaces(health*100/20, 3, ' ')); // convert health to percentage
                                                //points to be 3 characters wide
    s.append("%  Ammo:  ");
    s.append(addSpaces(ammo, 2, ' '));          // format ammo to be 2 characters wide
    s.append("  Bonus:  ");
    s.append(addSpaces(bonus, 4, ' '));         // format bonus to be 4 characters wide
    return s;
}

int StudentWorld::countKleptoBots(int x, int y)
{
    return countAt<KleptoBot>(x, y);
}

// return number of actors of type ActorType at point (x, y)
template <class ActorType>
int StudentWorld::countAt(int x, int y) {
    int finalCount = 0;
    // iterate through all objects in the game world
    for(int i = 0; i < m_actors.size(); i++)
    {
        if(m_actors[i]->getX() == x     // check coordinate
           && m_actors[i]->getY() == y  // check coordinate
           && dynamic_cast<ActorType*>(m_actors[i])
           != nullptr                   // check type
           && m_actors[i]->isAlive())   // check alive
            finalCount++;
    }
    return finalCount;
}



int StudentWorld::getType(Actor* a)
{
    Goodie* g = dynamic_cast<Goodie*>(a);
    if(g!=nullptr && g->isAlive()) return 101;
    
    Robot* r = dynamic_cast<Robot*>(a);
    if(r!=nullptr && r->isAlive()) return 102;
    
    Factory* f = dynamic_cast<Factory*>(a);
    if(f!=nullptr) return 102;
    
    Boulder* b = dynamic_cast<Boulder*>(a);
    if(b!=nullptr && b->isAlive()) return IID_BOULDER;
    
    Wall* w = dynamic_cast<Wall*>(a);
    if(w!=nullptr) return IID_WALL;
    
    Hole* h = dynamic_cast<Hole*>(a);
    if(h!=nullptr && h->isAlive()) return IID_HOLE;
    
    return 103;
}

// return true if there are no Actor objects at (x, y)
bool StudentWorld::isEmpty(int x, int y)
{
    return (getAt<Actor>(x, y) == nullptr);
}

// return true if all jewels in game world have been have collected
bool StudentWorld::jewelsCollected()
{
    for(int i = 0; i < m_actors.size(); i++){
        if(dynamic_cast<BlueJewel*>(m_actors[i])!=nullptr && m_actors[i]->isAlive()) return false;
    }
    return true;
}

void swap(int &a, int &b)
{
    int temp = b;
    b = a;
    a = temp;
}

// returns true if a bullet cannot pass through the specified Actor
// (boulder, robot, factory, wall)
bool StudentWorld::isBulletBarrier(Actor* a)
{
    if(dynamic_cast<Boulder*>(a) != nullptr
       || dynamic_cast<Robot*>(a) != nullptr
       || dynamic_cast<Factory*>(a) != nullptr
       || dynamic_cast<Wall*>(a) != nullptr)
        return true;
    
    return false;
}

bool isAscending(int a, int b, int c)
{
    if(a < b && b < c) return true;
    return false;
}

bool StudentWorld::isClear(int var1, int var2, int const1, bool vertical)
{
    if(var1 > var2)
        swap(var1, var2);   // ordering
    
    int actConst, actVar;   // vary depending on wether we are looking for bariers
                // between (var1, const1), (var2, const1)
                // or (const1, var1), (const1, var2) (i.e. vertical or horizontal)
                // go through all Actors and check if they
                // are a barrier between (x, y1) and (x, y2)
    
    for( int i = 0; i<m_actors.size(); i++)
    {
        if(vertical)        // if looking between (var1, const1), (var2, const1)
        {
            actConst = m_actors[i]->getY();
            actVar   = m_actors[i]->getX();
        }else               // if looking between (const1, var1), (const1, var2)
        {
            actConst = m_actors[i]->getX();
            actVar   = m_actors[i]->getY();
        }
        
        if(isBulletBarrier(m_actors[i])                 // check type
           && isAscending(var1, actVar, var2)           // check if actVar between var1 and var2
           && actConst == const1)                       // check if correct const coordinate
        {
            return false;
        }
    }
    return true;
}

bool StudentWorld::isClearX(int y1, int y2, int x)
{

    return isClear(y1, y2, x, false);
}

bool StudentWorld::isClearY(int x1, int x2, int y)
{
    return isClear(x1, x2, y, true);
}

Goodie* StudentWorld::getGoodie(int x, int y)
{
    return getAt<Goodie>(x, y);
}

// create string from these statistics of the following form:
// Score: 0000100  Level: 03  Lives:  3  Health:  70%  Ammo: 216  Bonus:   34
void StudentWorld::updateDisplayText()
{
    int score = this->getScore();
    int level = this->getLevel();
    unsigned int bonus = this->getBonus();
    
    string s = formatDisplay(score, level, this->getLives(),
                this->m_p->getHealth(), this->m_p->getAmmo(), bonus);
    
    setGameStatText(s);
}

bool StudentWorld::isExit(int x, int y)
{
    return (countAt<Exit>(x, y) > 0);
}

int StudentWorld::move()
{
    
    m_tickCount++;          // iterate tick count
    updateDisplayText();
    m_p->doSomething();     // make player do their thing
    
    if(getPlayer()->getHitPoints() <= 0)
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    if(isExit(m_p->getX(), m_p->getY()))
        return GWSTATUS_FINISHED_LEVEL;    // if player on exit return approaprietly
    
    for(int i = 0; i < m_actors.size(); i++){
        m_actors[i]->doSomething();        // make all actors do their thing
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::restorePlayerHealth()
{
    m_p->restoreHealth();
}

// returns true if player can occupy (x, y) on next move
bool StudentWorld::isStepable(int x, int y)
{
    Actor* a = getObjAt(x, y);
    
    if(isEmpty(x, y)) return true;
    
    // return true if (x, y) is occupied with a movable boulder
    // otherwise false
    if(a->isAlive() && getType(a) == IID_BOULDER)
    {
        return (dynamic_cast<Boulder*>(a)->pushed(m_p->getDirection()));
    }
    
    if(getAt<Exit>(x, y) != nullptr)
        return true;
    //return true if (x, y) is occupied with goodie (with no boulder)
    if(isGoodie(x, y))
    {
        dynamic_cast<Goodie*>(a)->swallowed();
        return true;
    }
    
    return false;
}

// clean up memory after end of level
void StudentWorld::cleanUp()
{
    delete m_p;
    while(!m_actors.empty())
    {
        delete m_actors.back(), m_actors.pop_back();
        std::cout<<"Cleaning up"<<std::endl;
    }
}

Robot* StudentWorld::getRobotAt(int x, int y)
{
    return getAt<Robot>(x, y);
}

Boulder* StudentWorld::getBoulderAt(int x, int y)
{
    return getAt<Boulder>(x, y);
}

Actor* StudentWorld::getObjAt(int x, int y)
{
    return getAt<Actor>(x, y);
}

bool StudentWorld::isGoodie(int x, int y)
{
    return (getAt<Goodie>(x, y) != nullptr);
}

// Create string to access level data (3 -> 'level03.dat')
string StudentWorld::getLevelString(int level){
    string result;
    
    result.append("level");
    result.append(addSpaces(level, 2, '0'));
    result.append(".dat");
    return result;
}

Player* StudentWorld::getPlayer()
{
    return m_p;
}

// add new Actor to game world
void StudentWorld::add(Actor* a)
{
    m_actors.push_back(a);
    m_nActors++;
}

int StudentWorld::loadLevel(int level)
{
    string curLevel = getLevelString(level);
    Level lev(assetDirectory());
    Level::LoadResult result = lev.loadLevel(curLevel);
    
    if (result == Level::load_fail_file_not_found ||
        result == Level::load_fail_bad_format)
        return -1;  //Something bad happened
    
    // otherwise the load was successful and you can access the
    // contents of the level
    
    
    for(int x = 0; x < VIEW_WIDTH; x++)
    {
        for(int y = 0; y < VIEW_HEIGHT; y++)
        {
            switch(lev.getContentsOf(x, y))
            {
                case Level::MazeEntry::empty:
                    break;
                case Level::MazeEntry::player:
                    m_p = new Player(x, y, this);
                    break;
                case Level::MazeEntry::ammo:
                   add(new AmmoGoodie(x, y, this));
                    break;
                case Level::MazeEntry::boulder:
                  add(new Boulder(x, y, this));
                    break;
                case Level::MazeEntry::exit:
                  add(new Exit(x, y, this));
                    break;
                case Level::MazeEntry::hole:
                 add(new Hole(x, y, this));
                    break;
                case Level::MazeEntry::extra_life:
                  add(new ExtraLifeGoodie(x, y, this));
                    break;
                case Level::MazeEntry::horiz_snarlbot:
                  add(new SnarlBot(IID_SNARLBOT, x, y, this, Actor::Direction::left));
                    break;
                case Level::MazeEntry::angry_kleptobot_factory:
                  add(new Factory(x, y, this, true));
                    break;
                case Level::MazeEntry::jewel:
                  add(new BlueJewel(x, y, this));
                    break;
                case Level::MazeEntry::kleptobot_factory:
                  add(new Factory(x , y, this, false));
                    break;
                case Level::MazeEntry::vert_snarlbot:
                    add(new SnarlBot(IID_SNARLBOT, x, y, this, Actor::Direction::down));
                    break;
                case Level::MazeEntry::wall:
                   add(new Wall(x, y, this));
                    break;
                case Level::MazeEntry::restore_health:
                   add(new RestoreHealthGoodie(x, y, this));
                    break;
                default: return -2; break;      // weird character in level file
            }
        }
    }
    return 0;   //All is good
}

// initialize new level
int StudentWorld::init()
{
    int level = getLevel();
    
    if(level > 99) return GWSTATUS_PLAYER_WON;
    
    int res = loadLevel(level);
    
    // Handle errors
    if(res == -1) return GWSTATUS_PLAYER_WON;
    if(res == -2) return GWSTATUS_LEVEL_ERROR;
    
    return GWSTATUS_CONTINUE_GAME;
}

