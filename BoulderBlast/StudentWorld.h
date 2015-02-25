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
class Goodie;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld();
    
    static std::pair<int, int> locationAtDirection(int x, int y, GraphObject::Direction d);
    Actor* getActor(int x, int y) const;
    void addActor(Actor *a) {  m_pendingActors.push_back(a);  }
    bool shouldShowExit() const {  return m_currentJewels == m_totalJewels;  }
    bool playerWon() const {  return m_playerWon;  }
    void setPlayerWon() {  m_playerWon = true;  }
    void incCurrentJewels() {  m_currentJewels ++;  }
    Player* getPlayer() const {  return m_player;  }
    int countKleptoBots(int x1, int x2, int y1, int y2) const;
    // It's necessary to have a rand generator in studentworld class, since it can guarantee to seed rand only one
    int randomNumber(int lowerbound, int upperbound) const;
    Goodie* getGoodie(int x, int y) const;
private:
    Player *m_player;
    std::vector<Actor*> m_actors, m_pendingActors;
    int m_bonus, m_currentJewels, m_totalJewels;
    bool m_playerWon;
};

#endif // STUDENTWORLD_H_
