#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, Direction dir, StudentWorld *world)
    :GraphObject(imageID, startX, startY, dir), m_world(world){  setVisible(true);  };
    virtual void doSomething() = 0;
    virtual void attacked() = 0;
    virtual bool shouldBeRemoved() const {  return m_shouldBeRemoved || m_hitPoints <= 0;  }
    void setShouldBeRemoved() {  m_shouldBeRemoved = true;  }
    virtual int getTypeID() const = 0;
    
    StudentWorld* getWorld() const {  return m_world;  }
    int getHitPoints() const {  return m_hitPoints;  };
    void setHitPoints(int hp) {  m_hitPoints = hp;  };
private:
    StudentWorld *m_world;
    bool m_shouldBeRemoved = false;
    int m_hitPoints = 20;
};

class Attacker : public Actor {
public:
    Attacker(int imageID, int startX, int startY, Direction dir, StudentWorld *world)
    :Actor(imageID, startX, startY, dir, world) {};
    int getAmmo() const {  return m_ammo;  }
    void setAmmo(int ammo) {  m_ammo = ammo;  }
private:
    int m_ammo = 20;
};

class Wall : public Actor {
public:
    Wall(int startX, int startY, StudentWorld *world)
    :Actor(IID_WALL, startX, startY, none, world){};
    virtual void doSomething() {  /* do nothing */  };
    virtual void attacked() {  /* do nothing */  };
    
    virtual int getTypeID() const {  return IID_WALL;  }
};

class Player : public Attacker {
public:
    Player(int startX, int startY, StudentWorld *world)
    :Attacker(IID_PLAYER, startX, startY, right, world){}
    virtual void doSomething();
    virtual void attacked();
    
    void moveIfPossible(int x, int y);
    virtual int getTypeID() const {  return IID_PLAYER;  }
    
private:
};

class Boulder : public Actor {
public:
    Boulder(int startX, int startY, StudentWorld *world)
    :Actor(IID_BOULDER, startX, startY, none, world) {  setHitPoints(10);  };
    virtual void doSomething() {  /* do nothing */  };
    virtual void attacked() {  /* do nothing */  };
    virtual int getTypeID() const {  return IID_BOULDER;  }
    bool push(Direction dir);

};

class Bullet : public Actor {
public:
    Bullet(int startX, int startY, Direction dir, StudentWorld *world)
    :Actor(IID_BULLET, startX, startY, dir, world){}
    virtual void doSomething();
    virtual void attacked() {  /* do nothing */  };
    virtual int getTypeID() const {  return IID_BULLET;  }
private:
    bool check();
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
