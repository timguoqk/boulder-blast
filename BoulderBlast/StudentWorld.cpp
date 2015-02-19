#include "StudentWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
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
    if (result == Level::load_fail_bad_format || result == Level::load_fail_file_not_found)
        return GWSTATUS_LEVEL_ERROR;
    //getContentsOf() method takes the column parameter (x) first, then the row parameter (y) second.
    for (int i=0; i<VIEW_WIDTH; i++) {
        for (int j=0; j<VIEW_HEIGHT; j++) {
            switch (lev.getContentsOf(i, j)) {
                case Level::player:
                    m_player = new Player(i, j, this);
                    break;
                case Level::wall:
                    m_actors.push_back(new Wall(i, j, this));
                    break;
                //TODO: other situations
                default:
                    break;
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // This code is here merely to allow the game to build, run, and terminate after hitting enter a few times
    for (Actor *a : m_actors) {
        a->doSomething();
        if (m_player->dead()){
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
    }
    // TODO: won't commit suicide?
    m_player->doSomething();
    if (m_player->won()) {
        //TODO: bonus score
        //increaseScore(2000+());
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    while (!m_actors.empty()) {
        delete m_actors.back();
        m_actors.pop_back();
    }
    delete m_player;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

pair<int, int> StudentWorld::locationAtDirection(int x, int y, GraphObject::Direction d) {
    pair<int, int> p(x,y);
    switch (d) {
        case GraphObject::up:
            p.first ++;
            break;
        case GraphObject::down:
            p.first --;
            break;
        case GraphObject::left:
            p.second --;
            break;
        case GraphObject::right:
            p.second ++;
            break;
        default:
            break;
    }
    return p;
}

Actor* StudentWorld::getActor(int x, int y) {
    auto it = find_if(m_actors.begin(), m_actors.end(), [x, y](Actor *a){return a->getX() == x && a->getY() == y;});
    if (it != m_actors.end())
        return *it;
    return nullptr;
}