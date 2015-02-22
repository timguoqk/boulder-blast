#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, Direction dir, StudentWorld *world)
    :GraphObject(imageID, startX, startY, dir), m_world(world){};
    virtual void doSomething() = 0;
    virtual void attacked() = 0;
    virtual bool shouldBeRemoved() const {  return m_dead || m_hitPoints <= 0;  }
    virtual void setShouldBeRemoved() {  m_dead = true;  }
    
    StudentWorld* getWorld() const {  return m_world;  }
    virtual int getTypeID() const = 0;
    int getHitPoints() const {  return m_hitPoints;  };
    virtual void setHitPoints(int hp) {  m_hitPoints = hp;  };
private:
    StudentWorld *m_world;
    int m_hitPoints = 20;
    bool m_dead = false;
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
    virtual int getTypeID() const {  return IID_PLAYER;  }
    int getAmmo() const {  return m_ammo;  }
private:
    int m_ammo = 20;
};

class Boulder : public Actor {
public:
    virtual int getTypeID() const {  return IID_BOULDER;  }
    virtual bool shouldBeRemoved() const;
};

class Bullet : public Actor {
public:
    Bullet(int startX, int startY, Direction dir, StudentWorld *world)
    :Actor(IID_BULLET, startX, startY, dir, world) {  setVisible(true);  };
    virtual void doSomething();
    
    //TODO: repeated behaviors. better solution?
    bool dead() const {  return m_dead;  }
private:
    bool m_dead = false;

};

class Exit : public Actor {
public:
    Exit(int startX, int startY, StudentWorld *world)
    :Actor(IID_EXIT, startX, startY, none, world){  setVisible(false);  };
    virtual void doSomething() {  /* do nothing */  };
    virtual void attacked() {  /* do nothing */  };
    
    virtual int getTypeID() const {  return IID_EXIT;  }
};
#endif // ACTOR_H_
