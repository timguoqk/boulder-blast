#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject{
public:
    Actor(int imageID, int startX, int startY, Direction dir, StudentWorld *world)
    :GraphObject(imageID, startX, startY, dir), m_world(world){};
    virtual void doSomething() = 0;
    virtual void attacked() = 0;
    
    StudentWorld* getWorld() const {  return m_world;  }
    virtual int getTypeID() const = 0;
private:
    StudentWorld *m_world;
};

class Wall : public Actor {
public:
    Wall(int startX, int startY, StudentWorld *world)
    :Actor(IID_WALL, startX, startY, none, world){  setVisible(true);  };
    virtual void doSomething() {  /* do nothing */  };
    virtual void attacked() {  /* do nothing */  };
    
    virtual int getTypeID() const {  return IID_WALL;  }
};

class Player : public Actor {
public:
    Player(int startX, int startY, StudentWorld *world);
    virtual void doSomething();
    virtual void attacked();
    
    void moveIfPossible(int x, int y);
    bool dead() const {  return m_dead;  }
    bool won() const {  return m_won;  }
    virtual int getTypeID() const {  return IID_PLAYER;  }
private:
    int m_hitPoints = 20, m_ammu = 20;
    bool m_dead = false, m_won = false;
};

class Boulder : public Actor {
public:
    virtual int getTypeID() const {  return IID_BOULDER;  }
};

class Bullet {
    
};
#endif // ACTOR_H_
