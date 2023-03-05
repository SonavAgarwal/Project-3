#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


#include "GameConstants.h"
#include <iostream> // TODO: REMOVE
//#include <cmath>


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

bool Actor::isOn(Actor *other) const {
    return (getX() == other->getX()) && (getY() == other->getY());
}

// #####################################
// AVATAR : ACTOR

Avatar::Avatar(const int imageID, const int startX, const int startY) : Actor(imageID, startX, startY, right, 0) {
    
    m_walk_direction = right; // TODO: WHAT TO INITIALIZE TO
    m_ticks_to_move = 0;
//    m_squares_to_move = 0; // TODO: WHAT TO CONSTRUCT WITH
    m_moving = false;
    m_just_landed = false; // infinity TODO: WHAT
    
}

void Avatar::doSomething() {
    m_just_landed = false;;
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
    
    if (sX % SPRITE_WIDTH != 0) {
        if (walkDirection == up || walkDirection == down) return false;
    } else if (sY % SPRITE_HEIGHT != 0) {
        if (walkDirection == left || walkDirection == right) return false;
    }
    
    int distance = 16;
    if (walkDirection == left || walkDirection == down) distance = 2;
    
    getPositionInThisDirection(walkDirection, distance, sX, sY);
    
    if ((sX < 0) || (sY < 0)) return false;
    if ((sX >= VIEW_WIDTH) || (sY >= VIEW_HEIGHT)) return false;
    
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
    
    double sTMD = getTicksToMove() / 8.0;
    
    if (sTMD > ((int) sTMD)) {
        return ((int) (sTMD + 1))   ;
    } else {
        return ((int) sTMD);
    }
    
//    return std::ceil(getTicksToMove() / 8.0);
}


void Avatar::rollMove(int maxRoll) {
    int squaresToMove = randInt(1, maxRoll);
    setTicksToMove(squaresToMove * 8);
    m_moving = true;
}

void Avatar::move() {
    int nX = getX(), nY = getY();
    getPositionInThisDirection(getWalkDirection(), 2, nX, nY);
    
    moveTo(nX, nY);
    setTicksToMove(getTicksToMove() - 1);
    
    if (getTicksToMove() == 0) {
        m_moving = false;
        m_just_landed = true;
    }
}

void Avatar::handleTurningPoint() {
    std::cerr << "turning point" << std::endl;
    if (!canWalkInDirection(getWalkDirection())) {
        if (getWalkDirection() == right || getWalkDirection() == left) {
            if (canWalkInDirection(up)) setWalkDirection(up);
            else setWalkDirection(down); // TODO: VERIFY THIS ASSUMPTION IS OK
        } else { // TODO: ALSO ASSUMPTION HERE only 4 direcs?
            if (canWalkInDirection(right)) setWalkDirection(right);
            else setWalkDirection(left); // TODO: VERIFY THIS ASSUMPTION IS OK
        }
        
        std::cerr << "new direction: " << getWalkDirection() << std::endl;
        updateSpriteDirection();
    } else std::cerr << "can continue" << std::endl;
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
//        std::cerr << "isAtFork" << std::endl;
    int otherDirectionCount = 0;
//    std::cerr << getX() << std::endl;
//    std::cerr << getY() << std::endl;
    for (int testDir = 0; testDir < 360; testDir += 90) {
//        if (getWalkDirection() == testDir) continue; // TODO: WTF HELP ME
        
//            std::cerr << testDir << std::endl;
        if (canWalkInDirection(testDir)) otherDirectionCount++;
    }
    
    
//        std::cerr << otherDirectionCount << std::endl;
    return otherDirectionCount > 2;
}

bool Avatar::getJustLanded() const {
    return m_just_landed;
}

void Avatar::setJustLanded(bool newJustLanded) {
    m_just_landed = newJustLanded;
}

bool Avatar::justLandedOn(Actor *other) const {
    return ((!m_moving) && m_just_landed && isOn(other)); // TODO: m_moving should always be false, NOT SURE IF STILL WORKS IF PLAYER MOVES INSTANTANEOUSLY
}

bool Avatar::isMovingOver(Actor *other) const {
    return (m_moving && isOn(other));
}


bool Avatar::canMove() const {
    return true;
}

void Avatar::teleportToRandomSquare() {
    Actor* newSquare = getStudentWorld()->getRandomSquare();
    if (newSquare == nullptr) return;
    moveTo(newSquare->getX(), newSquare->getY());
    setWalkDirection(-1);
}


// #####################################
// PLAYERAVATAR : AVATAR

PlayerAvatar::PlayerAvatar(const int imageID, const int startX, const int startY, int playerNum) : Avatar(imageID, startX, startY) {
    
    m_playerNum = playerNum;
    m_coins = 0;
    m_stars = 0;
    m_has_vortex = false;
    
    m_forced_direction = -1;
}



int PlayerAvatar::getCoins() const {
    return m_coins;
}
int PlayerAvatar::getStars() const {
    return m_stars;
}
int PlayerAvatar::changeCoins(int delta) { // TODO: should these auto cap
    if (m_coins + delta < 0) { // coins can't go negative
        
        int finalDelta = 0 - m_coins;
        m_coins = 0;
        
        return finalDelta;
        
    } else {
        m_coins += delta;
        return delta;
    }
}

void PlayerAvatar::changeStars(int delta) {
    m_stars += delta;
    if (m_stars < 0) m_stars = 0;
}

void PlayerAvatar::doSomething() {
    Avatar::doSomething();
    
    
//        std::cerr << "======" << std::endl;
            
    if (!getMoving()) { // waiting to roll
//        std::cerr << "A" << std::endl;
        
        if (getWalkDirection() == -1) {
//            std::cerr << "B" << std::endl;
            pointInRandomValidDirection();
        }
        
        int action = getStudentWorld()->getAction(m_playerNum);
        switch (action) {
            case ACTION_ROLL:
//                std::cerr << "C" << std::endl;
                rollMove(3); // TODO restore to 10
                break;
            case ACTION_FIRE:
                // TODO: CODE FIRING
                break;
            default:
//                std::cerr << "D" << std::endl;
                return;
        }
    }
    
    if (getMoving()) {
//        std::cerr << "E" << std::endl;
        
        // if avatar on directional square // TODO: do in directional square
        
        if (m_forced_direction != -1) {
            
                std::cerr << "being forced" << std::endl;
            setWalkDirection(m_forced_direction);
            updateSpriteDirection();
            m_forced_direction = -1;
        } else if (isAtFork()) {
            
//            std::cerr << "turning point" << std::endl;
//            std::cerr << "F" << std::endl;
            
            std::cerr << "a" << std::endl;
            int action = getStudentWorld()->getAction(m_playerNum);
            
            if (action == ACTION_NONE) return;
            
            std::cerr << "b" << std::endl;
            
            
            
            std::cerr << "current dir: " << getWalkDirection() << std::endl;
            std::cerr << "new dir: ";
            
            
            switch (action) {
                case ACTION_UP:
                    std::cerr << "up";
                    break;
                case ACTION_DOWN:
                    std::cerr << "down";
                    break;
                case ACTION_LEFT:
                    std::cerr << "left";
                    break;
                case ACTION_RIGHT:
                    std::cerr << "right";
                    break;
            } std::cerr << std::endl;
            
            
            if ((action == ACTION_UP && canWalkInDirection(up) && getWalkDirection() != down) ||
                (action == ACTION_LEFT && canWalkInDirection(left) && getWalkDirection() != right) ||
                (action == ACTION_RIGHT && canWalkInDirection(right) && getWalkDirection() != left) ||
                (action == ACTION_DOWN && canWalkInDirection(down) && getWalkDirection() != up)) {
                
                
                std::cerr << "c" << std::endl;
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
                
                std::cerr << "new dir: " << newDir << std::endl;
                
                setWalkDirection(newDir);
                updateSpriteDirection();
                
                
            } else return;
            
        } else {
            std::cerr << "MEOWOWOWOWOWWOW" << std::endl;
            handleTurningPoint();
        }
        
//            std::cerr << "H" << std::endl;
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

void PlayerAvatar::swapMovement(PlayerAvatar* other) { // TODO: think if swap just landed
    
    // swap x // TODO: CHECK THAT THIS WORKS
    int tempX = other->getX();
    int tempY = other->getY();
    other->moveTo(getX(), getY());
    moveTo(tempX, tempY);
    
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
    
    setJustLanded(false); // WHICH ONE IS THE CURRENT ONE NOW HMMMM actually nvm
    other->setJustLanded(false); // TODO: CHECK WITH SOMEONE

}

bool PlayerAvatar::hasVortex() const {
    return m_has_vortex;
}

void PlayerAvatar::setHasVortex(bool newHasVortex) {
    m_has_vortex = newHasVortex;
}

void PlayerAvatar::setForcedDirection(int newDirection) {
    m_forced_direction = newDirection;
}



// #####################################
// SQUARE : Monster


// #####################################
// SQUARE : ACTOR

Square::Square(const int imageID, const int startX, const int startY, const int direction) : Actor(imageID, startX, startY, direction, 1) {
    
}

void Square::doSomething() {
    if (!isActive()) return; // TODO: DONT NEED?
    
    for (int pN = 1; pN <= 2; pN++) {
        PlayerAvatar* player = getStudentWorld()->getPlayerWithNumber(pN);
        
        if (player != nullptr) handlePlayer(player);
    }
}

bool Square::canMove() const {
    return true;
}

// #####################################
// COINSQUARE : ACTOR

CoinSquare::CoinSquare(const int startX, const int startY, bool adds) : Square(adds ? IID_BLUE_COIN_SQUARE : IID_RED_COIN_SQUARE, startX, startY, right) {
    if (adds) m_delta_coins = 3;
    else m_delta_coins = -3;
}

//void CoinSquare::doSomething() { // TODO: SEE IF CAN REFACTOR WITH PARENT DOSOMETHING ALWAYS DOING STUFF FOR BOTH OR SOMETHING
//    if (!isActive()) return; // TODO: DONT NEED?
//
//    for (int pN = 1; pN <= 2; pN++) {
//        PlayerAvatar* player = getStudentWorld()->getPlayerWithNumber(pN);
//
//        if (player->justLandedOn(this)) {
//            player->changeCoins(m_delta_coins); // change coins already caps it
//            if (m_delta_coins >= 0) getStudentWorld()->playSound(SOUND_GIVE_COIN);
//            else getStudentWorld()->playSound(SOUND_TAKE_COIN);
//        }
//
//    }
//
//}

void CoinSquare::handlePlayer(PlayerAvatar *player) {
    if (player->justLandedOn(this)) {
        player->changeCoins(m_delta_coins); // change coins already caps it
        if (m_delta_coins >= 0) getStudentWorld()->playSound(SOUND_GIVE_COIN);
        else getStudentWorld()->playSound(SOUND_TAKE_COIN);
    }
}

// #####################################
// STARSQUARE : ACTOR

StarSquare::StarSquare(const int startX, const int startY) : Square(IID_STAR_SQUARE, startX, startY, right) {
    
    
}

void StarSquare::handlePlayer(PlayerAvatar *player) {
    //  player just landed on this or  player is moving over this
    if (player->justLandedOn(this) || player->isMovingOver(this)) { // TODO: check 2nd condition
        
        if (player->getCoins() < 20) return;
        else {
            player->changeCoins(-20);
            player->changeStars(1);
            getStudentWorld()->playSound(SOUND_GIVE_STAR);
        }
        
    }
    
}

// #####################################
// DIRECTIONALSQUARE : ACTOR

DirectionalSquare::DirectionalSquare(const int startX, const int startY, int direction) : Square(IID_DIR_SQUARE, startX, startY, direction) {
}

void DirectionalSquare::handlePlayer(PlayerAvatar *player) {
    
    if (player->justLandedOn(this) || player->isMovingOver(this)) {
        player->setForcedDirection(getDirection()); // TODO: double check is right direction
    }

}

// #####################################
// BANKSQUARE : ACTOR

BankSquare::BankSquare(const int startX, const int startY) : Square(IID_BANK_SQUARE, startX, startY, right) {
}

void BankSquare::handlePlayer(PlayerAvatar *player) {

    if (player->justLandedOn(this)) {
        int bankCoins = getStudentWorld()->emptyBank();
        player->changeCoins(bankCoins);
        getStudentWorld()->playSound(SOUND_WITHDRAW_BANK);
    } else if (player->isMovingOver(this)) {
        int delta = player->changeCoins(-5); // TODO verify this is correct negative and stuff
        getStudentWorld()->changeBankCoins(-1 * delta);
        getStudentWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
    
}

// #####################################
// COINSQUARE : ACTOR

EventSquare::EventSquare(const int startX, const int startY) : Square(IID_EVENT_SQUARE, startX, startY, right) {
}

void EventSquare::handlePlayer(PlayerAvatar *player) {

    if (player->justLandedOn(this)) {
        int option = randInt(1, 3);
//        int option = 1;
        switch (option) {
            case 1:
//                std::cerr << "TP" << std::endl;
//                std::cerr << player->getX() << ", " << player->getY() << std::endl;
                
                player->teleportToRandomSquare();
//                std::cerr << player->getX() << ", " << player->getY() << std::endl;
                getStudentWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            case 2:
                player->swapMovement(getStudentWorld()->getOtherPlayer(player));
                getStudentWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            case 3:
                player->setHasVortex(true); // TODO: if player alr has vortex is it still an option
                getStudentWorld()->playSound(SOUND_GIVE_VORTEX);
                break;
        }
    }
    
}

// #####################################
// COINSQUARE : ACTOR

DroppingSquare::DroppingSquare(const int startX, const int startY) : Square(IID_DROPPING_SQUARE, startX, startY, right) {
}

void DroppingSquare::handlePlayer(PlayerAvatar *player) {
        
    if (player->justLandedOn(this)) {
        int option = randInt(1, 2);
        switch (option) {
            case 1:
                player->changeCoins(-10);
                break;
            case 2:
                player->changeStars(-1);
                break;
        }
        getStudentWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);

    }
}
