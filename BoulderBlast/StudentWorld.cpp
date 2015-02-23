#include "StudentWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    // Seed the rand generator
    srand((int)time(NULL));
}

int StudentWorld::init() {
    // Init variables
    m_bonus = 1000;
    m_currentJewels = m_totalJewels = 0;
    m_playerWon = false;
    
    string curLevel = (getLevel() < 10) ? "level0"+to_string(getLevel())+".dat" : "level"+to_string(getLevel())+".dat";
    Level lev(assetDirectory());
    auto result = lev.loadLevel(curLevel);
    if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    if (result == Level::load_fail_file_not_found || getLevel() > 99)
        return GWSTATUS_PLAYER_WON;
    //getContentsOf() method takes the column parameter (x) first, then the row parameter (y) second.
    
    //TODO: what if the level is illegal?
    for (int i=0; i<VIEW_WIDTH; i++) {
        for (int j=0; j<VIEW_HEIGHT; j++) {
            switch (lev.getContentsOf(i, j)) {
                case Level::player:
                    m_player = new Player(i, j, this);
                    m_actors.push_back(m_player);
                    break;
                case Level::wall:
                    m_actors.push_back(new Wall(i, j, this));
                    break;
                case Level::boulder:
                    m_actors.push_back(new Boulder(i, j, this));
                    break;
                case Level::exit:
                    m_actors.push_back(new Exit(i, j, this));
                    break;
                case Level::hole:
                    m_actors.push_back(new Hole(i, j, this));
                    break;
                case Level::jewel:
                    m_actors.push_back(new Jewel(i, j, this));
                    m_totalJewels ++;
                    break;
                case Level::extra_life:
                    m_actors.push_back(new ExtraLifeGoodie(i, j, this));
                    break;
                case Level::ammo:
                    m_actors.push_back(new AmmoGoodie(i, j, this));
                    break;
                case Level::restore_health:
                    m_actors.push_back(new RestoreHealthGoodie(i, j, this));
                    break;
                case Level::horiz_snarlbot:
                    m_actors.push_back(new SnarlBot(i, j, true, this));
                    break;
                case Level::vert_snarlbot:
                    m_actors.push_back(new SnarlBot(i, j, false, this));
                    break;
                //TODO: other situations
                case Level::kleptobot_factory:
                    m_actors.push_back(new KleptoBotFactory(i, j, false, this));
                    break;
                case Level::angry_kleptobot_factory:
#warning uncomment  this!
//                    m_actors.push_back(new KleptoBotFactory(i, j, true, this));
                    break;
                default:
                    break;
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // Update the stat text
    ostringstream oss;
    oss.fill('0');
    oss << "Score: " << setw(7) << getScore() << "  ";
    oss << "Level: " << setw(2) << getLevel() << "  ";
    oss.fill(' ');
    oss << "Lives: " << setw(2) << getLives() << "  ";
    oss << "Health: " << setw(3) << m_player->getHitPoints()*5 << "%  ";
    oss << "Ammo: " << setw(3) << m_player->getAmmo() << "  ";
    oss << "Bonus: " << setw(4) << m_bonus;
    setGameStatText(oss.str());
    
    // This code is here merely to allow the game to build, run, and terminate after hitting enter a few times
    for (Actor *a : m_actors) {
        //TODO: OK to assume all actors are active?
        a->doSomething();
        if (m_player->shouldBeRemoved()){
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (playerWon()) {
            
            increaseScore(m_bonus);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    // Remove shouldBeRemoved actors
    for (auto it = m_actors.begin(); it != m_actors.end(); ) {
        if ((*it)->shouldBeRemoved()) {
            delete *it;
            it = m_actors.erase(it);
        }
        else
            it ++;
    }
    
    // Reduce the bonus score
    if (m_bonus > 0)
        m_bonus --;
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    while (!m_actors.empty()) {
        delete m_actors.back();
        m_actors.pop_back();
    }
}

StudentWorld::~StudentWorld() {
    //CAUTION: This code is identical to cleanUp()
    while (!m_actors.empty()) {
        delete m_actors.back();
        m_actors.pop_back();
    }
    delete m_player;
}

pair<int, int> StudentWorld::locationAtDirection(int x, int y, GraphObject::Direction d) {
    pair<int, int> p(x,y);
    switch (d) {
        case GraphObject::up:
            p.second ++;
            break;
        case GraphObject::down:
            p.second --;
            break;
        case GraphObject::left:
            p.first --;
            break;
        case GraphObject::right:
            p.first ++;
            break;
        default:
            break;
    }
    return p;
}

Actor* StudentWorld::getActor(int x, int y) const {
    auto it = find_if(m_actors.begin(), m_actors.end(), [x, y](Actor *a){return a->getX() == x && a->getY() == y;});  // Find actor at (x, y)
    if (it != m_actors.end()) {
        // There're only few situations when more than one actors are in the same place, so here we declare the priority, instead of using a vector
        auto it2 = find_if(it+1, m_actors.end(), [x, y](Actor *a){return a->getX() == x && a->getY() == y;});  // Find next actor at (x, y)
        if (it2 == m_actors.end())
            return *it;  // Only one actor
        // TODO: add more situations, double check
        switch ((*it)->getTypeID()) {
                case IID_JEWEL:
                case IID_EXTRA_LIFE:
                case IID_AMMO:
                case IID_RESTORE_HEALTH:
                    // Do not return goodie, return player
                case IID_HOLE:
                    // Do not return hole
                case IID_EXIT:
                    // Do not return exit, return the player
                case IID_ROBOT_FACTORY:
                    // Do not return factory, return the bot
                    return *it2;

                default:
                    return *it;
        }
    }
    return nullptr;
}

int StudentWorld::countKleptoBots(int x1, int x2, int y1, int y2) const {
    // Coordinates are inclusive
    // Use count_if in algorithm and lambda to count
    return (int)count_if(m_actors.begin(), m_actors.end(), [=](Actor *a){
        return ((a->getTypeID() == IID_KLEPTOBOT || a->getTypeID() == IID_ANGRY_KLEPTOBOT) && a->getX() >= x1 && a->getX() <= x2 && a->getY() >= y1 && a->getY() <= y2);
    });
}

int StudentWorld::randomNumber(int lowerbound, int upperbound) const {
    return rand()%upperbound + lowerbound;
}
