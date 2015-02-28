#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <set>
#include <algorithm>
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

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
                // Note: If a Bullet finds itself on a square with both a robot and a Factory, then the Bullet must damage the robot.
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
    Actor *a = getWorld()->getActor(loc.first, loc.second);
    if (!a || a->getTypeID() == IID_HOLE) {
        moveTo(loc.first, loc.second);
        return true;
    }
    // Cannot push, return false
    return false;
}

void Exit::doSomething() {
    if (!m_exitShown && getWorld()->shouldShowExit()) {
        // Show it if not shown already
        setVisible(true);
        m_exitShown = true;
        getWorld()->playSound(SOUND_REVEAL_EXIT);
    }
    if (getWorld()->shouldShowExit() && getWorld()->getActor(getX(), getY())->getTypeID() == IID_PLAYER) {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000);
        getWorld()->setPlayerWon();
        
    }
}

void Hole::doSomething() {
    if (shouldBeRemoved())
        return;
    Actor *a = getWorld()->getActor(getX(), getY());
    if (a->getTypeID() == IID_BOULDER) {
        // On the same location with a boulder
        setShouldBeRemoved();
        a->setShouldBeRemoved();
    }
}

void Goodie::doSomething() {
    if (shouldBeRemoved())
        return;
    if (getWorld()->getActor(getX(), getY())->getTypeID() == IID_PLAYER) {
        goodieEffects();
        setShouldBeRemoved();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}

void Jewel::goodieEffects() const {
    getWorld()->increaseScore(50);
    getWorld()->incCurrentJewels();
}

void ExtraLifeGoodie::goodieEffects() const {
    getWorld()->increaseScore(1000);
    getWorld()->incLives();
}

void RestoreHealthGoodie::goodieEffects() const {
    getWorld()->increaseScore(500);
    getWorld()->getPlayer()->setHitPoints(20);
}

void AmmoGoodie::goodieEffects() const {
    getWorld()->increaseScore(100);
    Player *p = getWorld()->getPlayer();
    p->setAmmo(p->getAmmo()+20);
}

Bot::Bot(int imageID, int startX, int startY, Direction dir, int hitPoints,  StudentWorld *world)
:Actor(imageID, startX, startY, dir, world) {
    setHitPoints(hitPoints);
    m_ticks = MAX(3, (28-getWorld()->getLevel())/4);
    m_ticksCount = 0;
}

bool Bot::shouldAct() {
    // Automatically increment the counter
    m_ticksCount ++;
    if (m_ticksCount == m_ticks) {
        // Should act
        m_ticksCount = 0;
        return true;
    }
    return false;
}

bool Bot::fireIfPossible() {
    Player *p = getWorld()->getPlayer();
    // Determine if in the right position
    std::pair<int, int> start, end;
    if (p->getX() == getX()) {
        // .x...
        // .....
        // .y...
        if ((p->getY() < getY() && getDirection() == down) || (p->getY() > getY() && getDirection() == up)) {
            // Set start, end
            start.first = end.first = getX();
            start.second = MIN(getY(), p->getY()) + 1;
            end.second = MAX(getY(), p->getY()) - 1;
        }
        else
            return false;
    }
    else if (p->getY() == getY()) {
        // .x..y..
        if ((p->getX() < getX() && getDirection() == left) || (p->getX() > getX() && getDirection() == right)) {
            // Set start, end
            start.second = end.second = getY();
            start.first = MIN(getX(), p->getX()) + 1;  // Exclude the actor itself, of course
            end.first = MAX(getX(), p->getX()) - 1;
        }
        else
            return false;
    }
    else
        return false;  // Dosen't match at all
        
    // Determine no obstacle
    for (int i = start.first; i <= end.first; i ++) {
        for (int j = start.second; j <= end.second; j ++) {
            Actor *a = getWorld()->getActor(i, j);
            if (!a)
                continue; // No actors
            
            switch (a->getTypeID()) {
                    // Walls, Boulders, robots, or robot factories
                case IID_WALL:
                case IID_BOULDER:
                case IID_SNARLBOT:
                case IID_KLEPTOBOT:
                case IID_ANGRY_KLEPTOBOT:
                case IID_ROBOT_FACTORY:
                    return false;
                default:
                    break;
            }
        }
    }
    // Return before this point if cannot fire
    
    auto loc = StudentWorld::locationAtDirection(getX(), getY(), getDirection());
    getWorld()->addActor(new Bullet(loc.first, loc.second, getDirection(), getWorld()));
    getWorld()->playSound(SOUND_ENEMY_FIRE);
    return true;
}

bool Bot::moveIfPossible() {
    auto loc = StudentWorld::locationAtDirection(getX(), getY(), getDirection());
    if (loc.first <= 0 && loc.first >= VIEW_WIDTH && loc.second <= 0 && loc.second >= VIEW_HEIGHT)
        return false;
    
    if (auto ptr = getWorld()->getActor(loc.first, loc.second))
        switch (ptr->getTypeID()) {
            case IID_ROBOT_FACTORY:
            case IID_WALL:
            case IID_HOLE:
            case IID_SNARLBOT:
            case IID_ANGRY_KLEPTOBOT:
            case IID_KLEPTOBOT:
            case IID_BOULDER:
            case IID_PLAYER:
                return false;
                
            default:
                break;
        }
    moveTo(loc.first, loc.second);
    return true;
}

void Bot::doSomething() {
    if (shouldBeRemoved() || !shouldAct())
        return;
    action();
}

void Bot::attacked() {
    if (!shouldBeRemoved()) // Hp >= 0
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    else {
        // The shouldBeRemoved method handles the logic of comparing hp with 0
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(getBonus());
        afterDeathAction();
    }
}

void SnarlBot::action() {
    if (!fireIfPossible()) {
        // Can't fire, try to move
        if (!moveIfPossible()) {
            // Can't move to the next loc
            // Reverse direction
            switch (getDirection()) {
                case GraphObject::up:
                    setDirection(down);
                    break;
                case GraphObject::down:
                    setDirection(up);
                    break;
                case GraphObject::left:
                    setDirection(right);
                    break;
                case GraphObject::right:
                    setDirection(left);
                    break;
                default:
                    break;
            }
        }
    }

}
KleptoBot::KleptoBot(int startX, int startY, StudentWorld *world)
:Bot(IID_KLEPTOBOT, startX, startY, right, 5, world) {
    m_distanceBeforeTurning = getWorld()->randomNumber(1, 6);
}

void KleptoBot::action() {
    Goodie *a = getWorld()->getGoodie(getX(), getY());
    if (a && getWorld()->randomNumber(1, 10) == 1) {
        // 1 in 10 chance when there's a goodie
        m_goodieType = a->getTypeID();
        a->setShouldBeRemoved();
        getWorld()->playSound(SOUND_ROBOT_MUNCH);
        return; // Do nothing more
    }
    
    m_count ++;
    // Note: add 1 because we increment m_count prematurely
    if (m_count == m_distanceBeforeTurning+1 || !moveIfPossible()){
        // The third situation, since the bot move already if possible
        m_count = 0;
        // Reset distanceBeforeTurning
        m_distanceBeforeTurning = getWorld()->randomNumber(1, 6);
        std::set<int> candidates;
        // Four directions map to 1-4 in the Direction enum
        candidates.insert(1);
        candidates.insert(2);
        candidates.insert(3);
        candidates.insert(4);
        int d = getWorld()->randomNumber(1, 4);
        // Since we'll delete one candidate at the end of the loop, we need to guarantee size() > 1
        int current_d = d;
        while (candidates.size() > 1) {
            auto loc = StudentWorld::locationAtDirection(getX(), getY(), static_cast<Direction>(current_d));
            bool canMove = true;
            if (auto ptr = getWorld()->getActor(loc.first, loc.second))
                switch (ptr->getTypeID()) {
                    case IID_ROBOT_FACTORY:
                    case IID_WALL:
                    case IID_HOLE:
                    case IID_SNARLBOT:
                    case IID_ANGRY_KLEPTOBOT:
                    case IID_KLEPTOBOT:
                    case IID_BOULDER:
                        canMove = false;
                        break;
                        
                    default:
                        break;
                }
            if (canMove) {
                setDirection(static_cast<Direction>(current_d));
                return;
            }
            
            candidates.erase(current_d);
            // Find next random number in candidates set
            auto it = candidates.begin();
            advance(it, getWorld()->randomNumber(0, (int)candidates.size()));
            current_d = *it;
        }
        // There're obstacles in all directions, use the first d
        setDirection(static_cast<Direction>(d));
    }
}

void KleptoBot::afterDeathAction() {
    Goodie *g;
    switch (m_goodieType) {
        case IID_RESTORE_HEALTH:
            g = new RestoreHealthGoodie(getX(), getY(), getWorld());
            break;
        case IID_AMMO:
            g = new AmmoGoodie(getX(), getY(), getWorld());
            break;
        case IID_EXTRA_LIFE:
            g = new ExtraLifeGoodie(getX(), getY(), getWorld());
            break;
        default:
            // Holds no goodie, return!
            return;
    }
    getWorld()->addActor(g);
}

void AngryKleptoBot::action() {
    // Fire if possible, if angry
    if (fireIfPossible())
        return;  // Fire already, end of the move
    
    // Ordinary actions
    KleptoBot::action();
}

void KleptoBotFactory::doSomething() {
    int numInSquare = getWorld()->countKleptoBots(getX()-3, getX()+3, getY()-3, getY()+3);
    if (numInSquare < 3 && getWorld()->countKleptoBots(getX(), getX(), getY(), getY()) == 0) {
        // 1 in 50 chance
        if (getWorld()->randomNumber(1, 50) == 1) {
            Actor *a;
            if (m_isAngry)
                a = new AngryKleptoBot(getX(), getY(), getWorld());  // Angry bot
            else
                a = new KleptoBot(getX(), getY(), getWorld());
            getWorld()->playSound(SOUND_ROBOT_BORN);
            getWorld()->addActor(a);
        }
    }
}
