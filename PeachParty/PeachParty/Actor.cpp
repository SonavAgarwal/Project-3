#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


#include "GameConstants.h"
#include <iostream> // TODO: REMOVE


// #####################################
// ACTOR

Actor::Actor(const int imageID, const int startX, const int startY, const int direction, const int depth) : GraphObject(imageID, startX, startY, direction, depth) {
    m_active = true;
    m_studentWorld = nullptr;
    
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

// #####################################
// AVATAR : ACTOR

Avatar::Avatar(const int imageID, const int startX, const int startY) : Actor(imageID, startX, startY, right, 0) {
    
    m_walk_direction = right; // TODO: WHAT TO INITIALIZE TO
    m_ticks_to_move = 0;
    m_squares_to_move = 0; // TODO: WHAT TO CONSTRUCT WITH
    m_moving = false;
    m_ticks_since_moved = 10000; // infinity TODO: WHAT
    
}

int Avatar::getTicksToMove() const {
    return m_ticks_to_move;
}

void Avatar::setTicksToMove(int ticksToMove) {
    m_ticks_to_move = ticksToMove;
}

bool Avatar::canWalkInDirection(int walkDirection) const {
    
    if (walkDirection == -1) return false;
    
    int sX = getX();
    int sY = getY();
    
    int distance = 16;
    if (walkDirection == left || walkDirection == down) distance = 2;
    
    getPositionInThisDirection(walkDirection, distance, sX, sY);
    
    // TODO: IS THERE A BETTER CONSTANT FOR 16
    
    return getStudentWorld()->isWalkable(sX, sY);
    
}

void Avatar::setWalkDirection(int walkDirection) {
    
    m_walk_direction = walkDirection;
}

int Avatar::getWalkDirection() const {
    return m_walk_direction;
}


int Avatar::getSquaresToMove() const {
    return m_squares_to_move;
}


void Avatar::rollMove(int maxRoll) {
    m_squares_to_move = randInt(1, maxRoll);
    setTicksToMove(m_squares_to_move * 8);
    m_moving = true;
}

void Avatar::move() {
    int nX = getX(), nY = getY();
    getPositionInThisDirection(getWalkDirection(), 2, nX, nY);
    
    moveTo(nX, nY);
    setTicksToMove(getTicksToMove() - 1);
    
    if (getTicksToMove() == 0) m_moving = false;
}

void Avatar::handleTurningPoint() {
    if (!canWalkInDirection(getWalkDirection())) {
        if (getWalkDirection() == right || getWalkDirection() == left) {
            if (canWalkInDirection(up)) setWalkDirection(up);
            else setWalkDirection(down); // TODO: VERIFY THIS ASSUMPTION IS OK
        } else { // TODO: ALSO ASSUMPTION HERE only 4 direcs?
            if (canWalkInDirection(right)) setWalkDirection(right);
            else setWalkDirection(left); // TODO: VERIFY THIS ASSUMPTION IS OK
        }
        
        updateSpriteDirection();
    }
}

bool Avatar::getMoving() const {
    return m_moving;
}

void Avatar::setMoving(bool newMoving) {
    m_moving = newMoving;
}

void Avatar::pointInRandomValidDirection() {
    
    int testDirection = randInt(0, 3) * 90;
    while (!canWalkInDirection(testDirection)) {
        testDirection += 90;
        testDirection %= 360;
    }
    
    setWalkDirection(testDirection);
    
    updateSpriteDirection();
}

void Avatar::updateSpriteDirection() {
    if (getWalkDirection() == left) setDirection(left);
    else setDirection(right);
}

bool Avatar::isAtFork() {
    int otherDirectionCount = 0;
    for (int testDir = 0; testDir < 360; testDir += 90) {
        if (getWalkDirection() == testDir) continue;
        
        if (canWalkInDirection(testDir)) otherDirectionCount++;
    }
    
    return otherDirectionCount > 1;
}


// #####################################
// PLAYERAVATAR : AVATAR

PlayerAvatar::PlayerAvatar(const int imageID, const int startX, const int startY, int playerNum) : Avatar(imageID, startX, startY) {
    
    m_playerNum = playerNum;
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

void PlayerAvatar::doSomething() {
            
    if (!getMoving()) { // waiting to roll
        
        if (!canWalkInDirection(getWalkDirection())) {
            pointInRandomValidDirection();
        }
        
        int action = getStudentWorld()->getAction(m_playerNum);
        switch (action) {
            case ACTION_ROLL:
                rollMove(10);
                break;
            case ACTION_FIRE:
                // TODO: CODE FIRING
                break;
            default:
                return;
        }
    }
    
    if (getMoving()) {
        
        // if avatar on directional square // TODO: do in directional square
        
        // else if at fork
        
        // else
        if (isAtFork()) {
            int action = getStudentWorld()->getAction(m_playerNum);
            if (action == ACTION_NONE) return;
            if ((action == ACTION_UP && canWalkInDirection(up) && getWalkDirection() != down) ||
                (action == ACTION_LEFT && canWalkInDirection(left) && getWalkDirection() != right) ||
                (action == ACTION_RIGHT && canWalkInDirection(right) && getWalkDirection() != left) ||
                (action == ACTION_DOWN && canWalkInDirection(down) && getWalkDirection() != up)) {
                
                // valid direction
                
                // get new direction
                
                int newDir = right;
                switch (action) {
                    case ACTION_UP:
                        newDir = up;
                        break;
                    case ACTION_DOWN:
                        newDir = down;
                        break;
                    case ACTION_LEFT:
                        newDir = left;
                        break;
                    case ACTION_RIGHT:
                        newDir = right;
                        break;
                }
                
                setWalkDirection(newDir);
                updateSpriteDirection();
                
                
            } else return;
            
        } else {
            handleTurningPoint();
        }
        
        move();
        
//        if (getTicksToMove() == 0) m_waiting_to_roll = true; // TODO: IN BOO AND BOWSER NEED EXTRA STUFF AFTER
        
    }
    
}

void PlayerAvatar::swapCoins(PlayerAvatar* other) {
    int tempCoins = other->m_coins;
    other->m_coins = m_coins;
    m_coins = tempCoins;
}

void PlayerAvatar::swapStars(PlayerAvatar* other) {
    int tempStars = other->m_stars;
    other->m_stars = m_stars;
    m_stars = tempStars;
}

void PlayerAvatar::swapMovement(PlayerAvatar* other) {
    
    // swap ticks to move
    int tempTTM = other->getTicksToMove();
    other->setTicksToMove(getTicksToMove());
    setTicksToMove(tempTTM);
    
    // swap state
    bool tempMoving = other->getMoving();
    other->setMoving(getMoving());
    setMoving(tempMoving);
    
    // swap walk direction
    int tempWalkDirection = other->getWalkDirection();
    other->setWalkDirection(getWalkDirection());
    setWalkDirection(tempWalkDirection);
    
    // swap sprite direction
    int tempSpriteDirection = other->getDirection();
    other->setDirection(getDirection());
    setDirection(tempSpriteDirection);

}



// #####################################
// SQUARE : Monster


// #####################################
// SQUARE : ACTOR

Square::Square(const int imageID, const int startX, const int startY, const int direction) : Actor(imageID, startX, startY, direction, 1) {
    
}

//void Square::doSomething() {
//    //
//}

// #####################################
// COINSQUARE : ACTOR

CoinSquare::CoinSquare(const int startX, const int startY, bool adds) : Square(adds ? IID_BLUE_COIN_SQUARE : IID_RED_COIN_SQUARE, startX, startY, right) {
    if (adds) m_delta_coins = 3;
    else m_delta_coins = -3;
}

void CoinSquare::doSomething() {
    if (!isActive()) return;
    
    
}

// #####################################
// STARSQUARE : ACTOR

StarSquare::StarSquare(const int startX, const int startY) : Square(IID_STAR_SQUARE, startX, startY, right) {
}

void StarSquare::doSomething() {
    if (!isActive()) return;
    
    
}

// #####################################
// DIRECTIONALSQUARE : ACTOR

DirectionalSquare::DirectionalSquare(const int startX, const int startY, int direction) : Square(IID_DIR_SQUARE, startX, startY, direction) {
}

void DirectionalSquare::doSomething() {
    if (!isActive()) return;
    
    
}

// #####################################
// BANKSQUARE : ACTOR

BankSquare::BankSquare(const int startX, const int startY) : Square(IID_BANK_SQUARE, startX, startY, right) {
}

void BankSquare::doSomething() {
    if (!isActive()) return;
    
    
}

// #####################################
// COINSQUARE : ACTOR

EventSquare::EventSquare(const int startX, const int startY) : Square(IID_EVENT_SQUARE, startX, startY, right) {
}

void EventSquare::doSomething() {
    if (!isActive()) return;
    
    
}

// #####################################
// COINSQUARE : ACTOR

DroppingSquare::DroppingSquare(const int startX, const int startY) : Square(IID_DROPPING_SQUARE, startX, startY, right) {
}

void DroppingSquare::doSomething() {
    if (!isActive()) return;
    
    
}
