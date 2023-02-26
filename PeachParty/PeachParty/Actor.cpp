#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


#include "GameConstants.h"
#include <iostream> // TODO: REMOVE

Actor::Actor(const int imageID, const int startX, const int startY, const int direction, const int depth) : GraphObject(imageID, startX, startY, direction, depth) {
    m_active = true;
    m_studentWorld = nullptr;
    
//    setDirection(int d)
}

void Actor::doSomething() {
    //
}

bool Actor::isActive() const {
    return m_active;
}

void Actor::setIsActive(bool newActive) {
    m_active = newActive;
}


void Actor::setStudentWorld(StudentWorld* sw) {
    m_studentWorld = sw;
}

StudentWorld* Actor::getStudentWorld() const {
    return m_studentWorld;
}


Avatar::Avatar(const int imageID, const int startX, const int startY) : Actor(imageID, startX, startY, right, 0) {
    
    m_walk_direction = right; // TODO: WHAT TO INITIALIZE TO
    m_ticks_to_move = 0;
}


void Avatar::doSomething() {
    //
}

int Avatar::getTicksToMove() const {
    return m_ticks_to_move;
}

void Avatar::setTicksToMove(int ticksToMove) {
    m_ticks_to_move = ticksToMove;
}

bool Avatar::canWalkInDirection(int walkDirection) const {
    int sX = getX();
    int sY = getY();
    
    int distance = 16;
    if (walkDirection == left || walkDirection == down) distance = 2;
    
    getPositionInThisDirection(walkDirection, distance, sX, sY); // TODO: IS THERE A BETTER CONSTANT FOR 16
    
//    std::cerr << "SUSSY" << std::endl;
//    std::cerr << sX << " " << sY << std::endl;
    return getStudentWorld()->isWalkable(sX, sY);
    
}

void Avatar::setWalkDirection(int walkDirection) {
    
//    std::cerr << "SETTING DIRECTION TO " << walkDirection << std::endl;
    m_walk_direction = walkDirection;
}

int Avatar::getWalkDirection() const {
    return m_walk_direction;
}

PlayerAvatar::PlayerAvatar(const int imageID, const int startX, const int startY, int playerNum) : Avatar(imageID, startX, startY) {
    
    m_playerNum = playerNum;
    m_waiting_to_roll = true;
    m_die_roll = randInt(1, 10); // TODO: WHAT TO CONSTRUCT WITH
    m_coins = 0;
    m_stars = 0;
}


int PlayerAvatar::getCoins() const {
    return m_coins;
}
int PlayerAvatar::getStars() const {
    return m_stars;
}
void PlayerAvatar::changeCoins(int delta) { // TODO: should these auto cap
    m_coins += delta;
}
void PlayerAvatar::changeStars(int delta) {
    m_coins -= delta;
}
int PlayerAvatar::getDieRoll() const {
    return m_die_roll;
}


void PlayerAvatar::doSomething() {
    
//    std::cerr << "peach gonna do sumn" << std::endl;
        
    if (m_waiting_to_roll) {
        int action = getStudentWorld()->getAction(m_playerNum);
        switch (action) {
            case ACTION_ROLL:
                m_die_roll = randInt(1, 10);
                setTicksToMove(m_die_roll * 8);
                m_waiting_to_roll = false; // WALKING STATE
                break;
            default:
                return;
        }
    }
    
    if (!m_waiting_to_roll) {
        
        // if avatar on directional square
        
        // else if at fork
        
        // else
        if (!canWalkInDirection(getWalkDirection())) {
            if (getWalkDirection() == right || getWalkDirection() == left) {
                if (canWalkInDirection(up)) setWalkDirection(up);
                else setWalkDirection(down); // TODO: VERIFY THIS ASSUMPTION IS OK
            } else { // TODO: ALSO ASSUMPTION HERE only 4 direcs?
                if (canWalkInDirection(right)) setWalkDirection(right);
                else setWalkDirection(left); // TODO: VERIFY THIS ASSUMPTION IS OK
            }
            
            if (getWalkDirection() == left) setDirection(left);
            else setDirection(right);
        }
        
        
        int nX = getX(), nY = getY();
        getPositionInThisDirection(getWalkDirection(), 2, nX, nY);
        
//        std::cerr << "-----" << std::endl;
//        std::cerr << nX << " " << nY << std::endl;
//        std::cerr << "-----" << std::endl;
//        std::cerr << getWalkDirection()<< std::endl;
        
        moveTo(nX, nY);
        setTicksToMove(getTicksToMove() - 1);
        if (getTicksToMove() == 0) m_waiting_to_roll = true;
        
    }
    
}



Square::Square(const int imageID, const int startX, const int startY, const int direction) : Actor(imageID, startX, startY, direction, 1) {
    
}

void Square::doSomething() {
    //
}

CoinSquare::CoinSquare(const int startX, const int startY, bool adds) : Square(IID_BLUE_COIN_SQUARE, startX, startY, right) {
    if (adds) m_delta_coins = 3;
    else m_delta_coins = -3;
}

void CoinSquare::doSomething() {
    if (!isActive()) return;
    
    
}
