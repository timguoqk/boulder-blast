#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Player::Player(int startX, int startY, StudentWorld *world)
:Actor(IID_PLAYER, startX, startY, right, world){
    setVisible(true);
}

void Player::moveIfPossible(int x, int y) {
    if (x >= 0 && x <= VIEW_WIDTH && y >= 0 && y <= VIEW_HEIGHT)
        /*A player is forbidden from moving onto an adjacent target square under the following circumstances:
         i. The square has a Factory, a Wall, or a Hole
         ii. The square has a robot of any type
         iii. The square has a Boulder and the Boulder cannot be pushed out of the way in the direction the user is trying to move (e.g., there’s no empty square or Hole in the maze where the user is trying to push the boulder).
         If the player is able to move onto a square containing a Boulder by pushing it out of the way, then the Player’s code must adjust the x,y location of the Boulder appropriately by “pushing” the Boulder with a push() method defined in the Boulder class.
         For more information on how and when Boulders can be pushed, please see the Boulder section of this document.*/
        //TODO: implement this!
        if (auto obj = getWorld()->getObject(x, y)) {
            if (obj->getID() == IID_WALL)
                return;
        }
        moveTo(x, y);
};

void Player::doSomething() {
    int ch;
    if (getWorld()->getKey(ch))
        switch (ch) {
            case KEY_PRESS_ESCAPE:
                m_dead = true;
                break;
            case KEY_PRESS_SPACE:
            {
                /*If the user pressed the space bar, then if the Player has any ammunition, the Player will fire a Bullet, which reduces their ammunition count by 1. To fire a Bullet, a new Bullet object must be added at the square immediately in front of the Player’s avatar, facing the same direction as the avatar. For example, if the Player is at x=10,y=7, facing upward, then the Bullet would be created at location x=10, y=8, facing upward. Every time the Player fires a bullet, your game MUST play the sound SOUND_PLAYER_FIRE (see the StudentWorld section of this document for details on how to play a sound).*/
                if (m_ammu <= 0)
                    break;
                m_ammu --;
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                // TODO: add bullet
//                std::pair<int, int> loc = StudentWorld::locationAtDirection(getX(), getY(), getDirection());
//                //Bullet;
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
    if (getWorld()->getObject(getX(), getY()) && getWorld()->getObject(getX(), getY())->getID() == IID_EXIT && getWorld()->exitShown()) {
        m_won = true;
    }
}

void Player::attacked() {
    //TODO: what if negative?
    m_hitPoints -= 2;
    if (m_hitPoints > 0)
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    else {
        getWorld()->playSound(SOUND_PLAYER_DIE);
        m_dead = true;
    }
}