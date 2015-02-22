#include "StudentWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init() {
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
//                case Level::wall:
//                    m_actors.push_back(new Wall(i, j, this));
//                    break;
                case Level::boulder:
                    m_actors.push_back(new Boulder(i, j, this));
                    break;
                case Level::exit:
                    m_exitLoc = pair<int, int>(i, j);
                //TODO: other situations
                default:
                    break;
            }
        }
    }
    
    // Init variables
    m_bonus = 1000;
    m_score = 0;
    m_currentJewels = 0;
    //TODO: del this line
    m_totalJewels = 9999;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // Update the stat text
    ostringstream oss;
    oss.fill('0');
    oss << "Score: " << setw(7) << m_score << "  ";
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
            increaseScore(2000+m_bonus);
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
    
    if (shouldShowExit()) {
        Actor *exit = *find_if(m_actors.begin(), m_actors.end(), [](Actor *a){  return a->getTypeID() == IID_EXIT;  });
        // Assume there must be an exit
        exit->setVisible(true);
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
    if (it != m_actors.end())
        return *it;
    return nullptr;
}

bool StudentWorld::playerWon() const {
    if (shouldShowExit() && m_player->getX() == m_exitLoc.first && m_player->getY() == m_exitLoc.second)
        return true;
    return false;
}