#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

pair<int, int> StudentWorld::locationAtDirection(int x, int y, GraphObject::Direction d) {
    pair<int, int> p(x,y);
    switch (d) {
            //TODO: what if overflows?
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

void StudentWorld::playerDied() {
    // game over, clean it up
    playSound(SOUND_ROBOT_DIE);
}

void StudentWorld::addObject() {
    
}