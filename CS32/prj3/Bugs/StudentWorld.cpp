#include "StudentWorld.h"
#include "Field.h"
#include "Compiler.h"
#include "Actor.h"

#include <map>
#include <list>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

const int PHEROMONE_ADD = 256;
const int GAME_TICKS    = 2000;      // WARNING: This must be 2000!
const double PI = 3.1415;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

void StudentWorld::addActor(Actor* actor)
{
    m_actorMap[make_pair(actor->getX(), actor->getY())].push_back(actor);
}

// WARNING: This is very similar to adding pheromone strength! (emitPheromone)
void StudentWorld::addFood(int x, int y, int amount)
{
    Food* food = getAt<Food>(x, y);
    if(food == nullptr)
    {
        food = new Food(this, x, y, amount);
        addActor(food);
    }else food->add(amount);
}

void StudentWorld::readActors()
{

    
    for(int x = 0; x < VIEW_WIDTH; x++)
        for(int y = 0; y < VIEW_HEIGHT; y++)
        {
            Field::FieldItem item = f.getContentsOf(x, y);
            switch(item)
            {
                case Field::FieldItem::empty:       break;
                case Field::FieldItem::anthill0:
                    if(m_anthillNum > 0)
                    m_actorMap[std::pair<int, int>(x, y)].push_back(
                                                                    m_ahArr[0] =
                                                                    new AntHill(this,
                                                                                x, y, 0, m_compArr[0]));
                    break;
                    
                case Field::FieldItem::anthill1:
                    if(m_anthillNum > 1)
                    m_actorMap[std::pair<int, int>(x, y)].push_back(
                                                                    m_ahArr[1] =
                                                                    new AntHill(this,
                                                                                x, y, 1, m_compArr[1]));
                    break;
                    
                case Field::FieldItem::anthill2:
                    if(m_anthillNum > 2)
                    m_actorMap[std::pair<int, int>(x, y)].push_back(
                                                                    m_ahArr[2] =
                                                                    new AntHill(this,
                                                                                x, y, 2, m_compArr[2]));
                    break;
                    
                case Field::FieldItem::anthill3:
                    if(m_anthillNum > 3)
                    m_actorMap[std::pair<int, int>(x, y)].push_back(
                                                                    m_ahArr[3] =
                                                                    new AntHill(this,
                                                                                x, y, 3, m_compArr[3]));
                    break;
                    
                case Field::FieldItem::food:
                    m_actorMap[std::pair<int, int>(x, y)].push_back(new Food(this, x, y,
                                                                             INIT_FOOD_AMOUNT));
                    break;
                    
                case Field::FieldItem::grasshopper:
                    m_actorMap[std::pair<int, int>(x, y)].push_back(new BabyGrasshopper(this, x, y));
                    break;
                    
                case Field::FieldItem::water:
                    m_actorMap[std::pair<int, int>(x, y)].push_back(new Pool(this, x, y));
                    break;
                    
                case Field::FieldItem::rock:
                    m_actorMap[std::pair<int, int>(x, y)].push_back(new Pebble(this, x, y));
                    break;
                    
                case Field::FieldItem::poison:
                    m_actorMap[std::pair<int, int>(x, y)].push_back(new Poison(this, x, y));
                    break;
            }
        }
}
int StudentWorld::compileBugs()
{
    std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
    m_anthillNum = fileNames.size();
    // Check compilers
    for(int i = 0; i < m_anthillNum; i++)
    {
        m_compArr[i] = new Compiler;
        std::string error;
        
        if( ! m_compArr[i]->compile(fileNames[i], error))
        {
            // there's a problem with the compilation
            setError(fileNames[i] + " " + error);
            return GWSTATUS_LEVEL_ERROR;
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::init()
{
    if(compileBugs() == GWSTATUS_LEVEL_ERROR)
        return GWSTATUS_LEVEL_ERROR;
    
    // Load field
    std::string fieldFile = getFieldFilename();
    if (  f.loadField(fieldFile) != Field::load_success)
    {
        return GWSTATUS_LEVEL_ERROR;   // Error reading field file!
    }

    readActors();
    
    m_tickCount = 0;
    return GWSTATUS_CONTINUE_GAME;
}




std::string formlen(int len, int n, char prefix)
{
    string num = to_string(n);
    string s(len - num.length(), prefix);
    s += num;
    return s;
}
std::string StudentWorld::format(int ticks, int* anthills, int winning)
{
    std::string s;
    s += "Ticks:";
    s += formlen(5, GAME_TICKS - ticks, ' ');
        s += " -  ";
    for(int i = 0; i < m_anthillNum; i++)
    {

        s += m_compArr[i]->getColonyName();
        if(i == m_winner)
            s += '*';
        s += ": ";
        if(anthills[i] > 99)
            s += formlen(2, 99, '0');
        else
            s += formlen(2, anthills[i], '0');
        s += " ants  ";
    }
    return s;
}
void StudentWorld::setDisplayText()
{
    std::string s;
    int sorted[4];  // It's OK for everything to stay as it is numberwise in this function
    
    sorted[0] = m_antCounts[0];
    sorted[1] = m_antCounts[1];
    sorted[2] = m_antCounts[2];
    sorted[3] = m_antCounts[3];
    sort(sorted, sorted + 4);
    s = format(m_tickCount, m_antCounts, sorted[3]);
    setGameStatText(s);
}

void StudentWorld::getWinner()
{
    int maxIndex = m_winner;
    int max;
    if(m_winner >= 0) max = m_antCounts[m_winner];
    else max = 0;
    for(int i = 0; i < m_anthillNum; i++)
    {
        if(m_antCounts[i] > 5) m_winnable[i] = true;    // Eligable to win
        if(m_winnable[i] && m_antCounts[i] > max)
        {
            maxIndex = i;
            max = m_antCounts[i];
        }
    }
    m_winner = maxIndex;
}







void StudentWorld::getPosWithinRadius(int &x, int &y, int rad)
{
    double radius = randInt(1, rad);
    double theta = randInt(1, 100)/100.0*2*PI;
    x = (int)round(x + radius*cos(theta));
    y = (int)round(y + radius*sin(theta));
    if(x >= VIEW_WIDTH) x = VIEW_WIDTH - 1;
    if(y >= VIEW_HEIGHT) y = VIEW_HEIGHT - 1;
    if( x < 0) x = 0;
    if( y < 0) y = 0;
}
void StudentWorld::updateContainer()
{
    // Update container
    for(auto it = m_actorMap.begin(); it != m_actorMap.end(); ++it)
        for(auto itIn = (it->second).begin(); itIn != (it->second).end();)
        {
            // Delete from container if dead
            if((*itIn)->isDead())
            {
                delete *itIn;
                itIn = (it->second).erase(itIn);
            } else if((*itIn)->getX() != (it->first).first
                      || (*itIn)->getY() != (it->first).second)     // Change Actor keys if they moved
            {
                m_actorMap[std::pair<int, int>((*itIn)->getX()
                                               , (*itIn)->getY())].push_back(*itIn);
                itIn = it->second.erase(itIn);
            }else ++itIn;                                           // Iterate if nothing is deleted
        }
}
void StudentWorld::askActorsToDoSomething()
{
    for(auto it = m_actorMap.begin(); it != m_actorMap.end(); ++it)
        for(auto itIn = (it->second).begin(); itIn != (it->second).end();)
        {
            // Delete from container if dead
            // WARNING: Something's wrong with this
            if((*itIn)->isDead())
            {
                delete *itIn;
                *itIn = nullptr;
                itIn = (it->second).erase(itIn);
            }
            if(itIn != (it->second).end())
            {
                (*itIn)->doSomething();
                ++itIn;
            }
        }
}

int StudentWorld::move()
{
    m_tickCount++;
    
    askActorsToDoSomething();
    updateContainer();
    getWinner();
    setDisplayText();
    // If simulation over, find winner
    if(m_tickCount == GAME_TICKS)
    {
        if(m_winner != -1)
        {
            setWinner(m_compArr[m_winner]->getColonyName());
            return GWSTATUS_PLAYER_WON;
        }else
            return GWSTATUS_NO_WINNER;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

// TODO: Add const keyword everywhere where needed. Change pass by value to pass by const reference where possible, this can be done better
Pheromone* StudentWorld::getPheromoneAt(int x, int y, int typeId)
{
    auto it = m_actorMap[make_pair(x, y)].begin();
    for(; it != m_actorMap[make_pair(x, y)].end(); ++it)
        if(dynamic_cast<Pheromone*>(*it) && typeId == dynamic_cast<Pheromone*>(*it)->getColonyNum())
            return dynamic_cast<Pheromone*>(*it);
    return nullptr;
}


void StudentWorld::cleanUp()
{
    for(auto it = m_actorMap.begin(); it != m_actorMap.end(); ++it)
        for(auto itIn = (it->second).begin(); itIn != (it->second).end(); itIn = (it->second).erase(itIn))
        {
            delete (*itIn);
        }
    for(int i = 0; i < m_anthillNum; i++)
    {
        if(m_compArr[i] != nullptr)
        {
            delete m_compArr[i];
            m_compArr[i] = nullptr;
        }
    }
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
