#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::moveIfPossible(int x, int y) {
    // Move by default, return if cannot move
    if (x <= 0 && x >= VIEW_WIDTH && y <= 0 && y >= VIEW_HEIGHT)
        return;
    
    if (auto ptr = getWorld()->getActor(x, y))
        switch (ptr->getTypeID()) {
            case IID_ROBOT_FACTORY:
            case IID_WALL:
            case IID_HOLE:
            case IID_SNARLBOT:
            case IID_ANGRY_KLEPTOBOT:
            case IID_KLEPTOBOT:
                return;
            
            case IID_BOULDER:
                if (!(dynamic_cast<Boulder *>(ptr)->push(getDirection())))
                    return;
                
            default:
                break;
        }
    
    moveTo(x, y);
};

void Player::doSomething() {
    int ch;
    if (getWorld()->getKey(ch))
        switch (ch) {
            case KEY_PRESS_ESCAPE:
                setShouldBeRemoved();
                break;
            case KEY_PRESS_SPACE:
            {
                if (getAmmo() <= 0)
                    break;
                setAmmo(getAmmo()-1);
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                auto loc = StudentWorld::locationAtDirection(getX(), getY(), getDirection());
                getWorld()->addActor(new Bullet(loc.first, loc.second, getDirection(), getWorld()));
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                break;
            }
            case KEY_PRESS_UP:
                setDirection(up);
                moveIfPossible(getX(), getY()+1);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                moveIfPossible(getX(), getY()-1);
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                moveIfPossible(getX()-1, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                moveIfPossible(getX()+1, getY());
                break;
            default:
                break;
        }
}

void Player::attacked() {
    if (getHitPoints() > 0)
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    else {
        getWorld()->playSound(SOUND_PLAYER_DIE);
        setShouldBeRemoved();
    }
}

bool Bullet::check() {
    // TODO: When there're multiple actors at the same place
    Actor *a = getWorld()->getActor(getX(), getY());
    if (a) {
        // There's something in the next block
        switch (a->getTypeID()) {
            case IID_BOULDER:
            case IID_ANGRY_KLEPTOBOT:
            case IID_KLEPTOBOT:
            case IID_SNARLBOT:
            case IID_PLAYER:
                a->setHitPoints(a->getHitPoints() - 2);
                a->attacked();
                
            case IID_ROBOT_FACTORY:
            case IID_WALL:
                // Remove but do no damage
                //TODO: Note: If a Bullet finds itself on a square with both a robot and a Factory, then the Bullet must damage the robot.
                setShouldBeRemoved();
                return false;
                
            default:
                break;
        }
    }
    return true;
}

void Bullet::doSomething() {
    if (shouldBeRemoved())
        return;
    if (check()) {
        auto loc = StudentWorld::locationAtDirection(getX(), getY(), getDirection());
        moveTo(loc.first, loc.second);
        check();
    }
}

bool Boulder::push(Direction dir) {
    auto loc = StudentWorld::locationAtDirection(getX(), getY(), dir);
    // TODO: When there're multiple actors at the same place
    Actor *a = getWorld()->getActor(loc.first, loc.second);
    if (!a || a->getTypeID() == IID_HOLE) {
        moveTo(loc.first, loc.second);
        return true;
    }
    // Cannot push, return false
    return false;
}
