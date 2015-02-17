#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}

	virtual int init()
	{
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		  // This code is here merely to allow the game to build, run, and terminate after hitting enter a few times 
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	virtual void cleanUp()
	{
	}
    
    void playerDied();
    void addObject();
    static std::pair<int, int> locationAtDirection(int x, int y, GraphObject::Direction d);
    
private:
    /*Add any private data members to this class required to keep track of
     Walls as well as the Player object. You may ignore all other items in
     the maze such as SnarlBots, the Exit, etc. for part #1.*/
    
    //std::unordered_map<std::pair<int, int>, GraphObject> m_map;
};

#endif // STUDENTWORLD_H_
