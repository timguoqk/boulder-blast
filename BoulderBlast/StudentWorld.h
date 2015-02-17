#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <utility>
#include <map>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Player;
class Actor;
class Player;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}

    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    virtual ~StudentWorld();
    void playerDied();
    static std::pair<int, int> locationAtDirection(int x, int y, GraphObject::Direction d);
    GraphObject* getObject(int x, int y);
    
    
private:
    /*Add any private data members to this class required to keep track of Walls as well as the Player object. You may ignore all other items in the maze such as SnarlBots, the Exit, etc. for part #1.*/
    
    Player *m_player;
    std::vector<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
