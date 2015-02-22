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
    
    static std::pair<int, int> locationAtDirection(int x, int y, GraphObject::Direction d);
    Actor* getActor(int x, int y) const;
    bool shouldShowExit() const {  return m_currentJewels == m_totalJewels;  }
    bool playerWon() const;
    
private:
    Player *m_player;
    std::vector<Actor*> m_actors;
    std::pair<int, int> m_exitLoc;
    int m_bonus, m_score, m_currentJewels, m_totalJewels;
};

#endif // STUDENTWORLD_H_
