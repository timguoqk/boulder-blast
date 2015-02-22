#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//TODO: destructors?

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, Direction dir, StudentWorld *world)
    :GraphObject(imageID, startX, startY, dir), m_world(world){  setVisible(true);  }
    virtual void doSomething() = 0;
    virtual void attacked() = 0;
    virtual bool shouldBeRemoved() const {  return m_shouldBeRemoved || m_hitPoints <= 0;  }
    void setShouldBeRemoved() {  m_shouldBeRemoved = true;  }
    virtual int getTypeID() const = 0;
    
    StudentWorld* getWorld() const {  return m_world;  }
    int getHitPoints() const {  return m_hitPoints;  }
    void setHitPoints(int hp) {  m_hitPoints = hp;  }
private:
    StudentWorld *m_world;
    bool m_shouldBeRemoved = false;
    int m_hitPoints = 20;
};

class Attacker : public Actor {
public:
    Attacker(int imageID, int startX, int startY, Direction dir, StudentWorld *world)
    :Actor(imageID, startX, startY, dir, world) {}
    int getAmmo() const {  return m_ammo;  }
    void setAmmo(int ammo) {  m_ammo = ammo;  }
private:
    int m_ammo = 20;
};

class Wall : public Actor {
public:
    Wall(int startX, int startY, StudentWorld *world)
    :Actor(IID_WALL, startX, startY, none, world){}
    virtual void doSomething() {  /* do nothing */  }
    virtual void attacked() {  /* do nothing */  }
    
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
    :Actor(IID_BOULDER, startX, startY, none, world) {  setHitPoints(10);  }
    virtual void doSomething() {  /* do nothing */  }
    virtual void attacked() {  /* do nothing */  }
    virtual int getTypeID() const {  return IID_BOULDER;  }
    bool push(Direction dir);

};

class Bullet : public Actor {
public:
    Bullet(int startX, int startY, Direction dir, StudentWorld *world)
    :Actor(IID_BULLET, startX, startY, dir, world){}
    virtual void doSomething();
    virtual void attacked() {  /* do nothing */  }
    virtual int getTypeID() const {  return IID_BULLET;  }
private:
    bool check();
};

class Exit : public Actor {
public:
    Exit(int startX, int startY, StudentWorld *world)
    :Actor(IID_EXIT, startX, startY, none, world){  setVisible(false);  }
    virtual void doSomething();
    virtual void attacked() {  /* do nothing */  }
    
    virtual int getTypeID() const {  return IID_EXIT;  }
private:
    bool m_exitShown = false;
};

class Hole : public Actor {
public:
    Hole(int startX, int startY, StudentWorld *world)
    :Actor(IID_HOLE, startX, startY, none, world){}
    virtual void doSomething();
    virtual void attacked() {  /* do nothing */  }
    
    virtual int getTypeID() const {  return IID_HOLE;  }
};

class Goodie : public Actor {
public:
    Goodie(int imageID, int startX, int startY, StudentWorld *world)
    :Actor(imageID, startX, startY, none, world){}
    virtual void doSomething();
    virtual void attacked() {  /* Goodies can’t be attacked */  }
    virtual void goodieEffects() const = 0;  // Abstract methods, to be implemented by subclass
    
    virtual int getTypeID() const = 0;
};


class Jewel : public Goodie {
public:
    Jewel(int startX, int startY, StudentWorld *world)
    :Goodie(IID_JEWEL, startX, startY, world){}
    virtual void goodieEffects() const;
    
    virtual int getTypeID() const {  return IID_JEWEL;  }
};

class RestoreHealthGoodie : public Goodie {
public:
    RestoreHealthGoodie(int startX, int startY, StudentWorld *world)
    :Goodie(IID_RESTORE_HEALTH, startX, startY, world){}
    virtual void goodieEffects() const;
    
    virtual int getTypeID() const {  return IID_RESTORE_HEALTH;  }
};

class AmmoGoodie : public Goodie {
public:
    AmmoGoodie(int startX, int startY, StudentWorld *world)
    :Goodie(IID_AMMO, startX, startY, world){}
    virtual void goodieEffects() const;
    
    virtual int getTypeID() const {  return IID_AMMO;  }
};

class ExtraLifeGoodie : public Goodie {
public:
    ExtraLifeGoodie(int startX, int startY, StudentWorld *world)
    :Goodie(IID_EXTRA_LIFE, startX, startY, world){}
    virtual void goodieEffects() const;
    
    virtual int getTypeID() const {  return IID_EXTRA_LIFE;  }
};
#endif // ACTOR_H_
