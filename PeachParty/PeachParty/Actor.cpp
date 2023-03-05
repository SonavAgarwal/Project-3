#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

#include "GameConstants.h"

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

bool Actor::isImpactable() {
    return false;
}
void Actor::impact() {} // TODO: ASK? EMPTY

// #####################################
// AVATAR : ACTOR

Avatar::Avatar(const int imageID, const int startX, const int startY) : Actor(imageID, startX, startY, right, 0) {
    m_walk_direction = right;
    m_ticks_to_move = 0;
    m_moving = false;
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
    
    // return ceil of ticksToMove / 8
    if (sTMD > ((int) sTMD)) {
        return ((int) (sTMD + 1));
    } else {
        return ((int) sTMD);
    }
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
    m_ticks_to_move -= 1;
    
    if (getTicksToMove() == 0) {
        m_moving = false;
//        m_just_landed = true;
    }
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
        if (canWalkInDirection(testDir)) otherDirectionCount++;
    }
    return otherDirectionCount > 2;
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

bool Avatar::isDirectlyOnTopOfSquare() const {
    return ((getX() % SPRITE_WIDTH == 0) && (getY() % SPRITE_HEIGHT == 0));
}


// #####################################
// PLAYERAVATAR : AVATAR

PlayerAvatar::PlayerAvatar(const int imageID, const int startX, const int startY, int playerNum) : Avatar(imageID, startX, startY) {
    
    m_playerNum = playerNum;
    m_coins = 0;
    m_stars = 0;
    m_has_vortex = false;
    
    m_forced_direction = -1;
    m_just_landed = false;

}

int PlayerAvatar::getCoins() const {
    return m_coins;
}
int PlayerAvatar::getStars() const {
    return m_stars;
}
int PlayerAvatar::changeCoins(int delta) {
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
    m_just_landed = false;
    
    if (!getMoving()) { // waiting to roll
    
        if (getWalkDirection() == -1) {
            pointInRandomValidDirection();
        }
        
        int action = getStudentWorld()->getAction(m_playerNum);
        switch (action) {
            case ACTION_ROLL:
                rollMove(10);
                break;
            case ACTION_FIRE: {
                if (m_has_vortex) {
                
                    int spawnX = getX(), spawnY = getY();
                    getPositionInThisDirection(getWalkDirection(), 16, spawnX, spawnY);
                    Vortex* vortex = new Vortex(spawnX, spawnY, getWalkDirection());
                    getStudentWorld()->addGridObject(vortex);
                    
                    getStudentWorld()->playSound(SOUND_PLAYER_FIRE);
                    
                    m_has_vortex = false;
                    
                }
                
                break;
            }
            default:
                return;
        }
    }
    if (getMoving()) { // walking
        
        if (m_forced_direction != -1) { // On a directional square
            setWalkDirection(m_forced_direction);
            updateSpriteDirection();
            m_forced_direction = -1;
        } else if (isDirectlyOnTopOfSquare() && isAtFork()) { // At a fork
            
            int action = getStudentWorld()->getAction(m_playerNum);
            if (action == ACTION_NONE) return; // User didn't select a direction
            
            if ((action == ACTION_UP && canWalkInDirection(up) && getWalkDirection() != down) ||
                (action == ACTION_LEFT && canWalkInDirection(left) && getWalkDirection() != right) ||
                (action == ACTION_RIGHT && canWalkInDirection(right) && getWalkDirection() != left) ||
                (action == ACTION_DOWN && canWalkInDirection(down) && getWalkDirection() != up)) {
                // User selected a valid direction
                
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
                
            } else return; // User selected an invalid direction
        } else {
            handleTurningPoint();
        }
        
        move();
        if (getTicksToMove() == 0) m_just_landed = true;
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
    
    // swap x and y
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
    
    setJustLanded(false); // TODO: ASK?
    other->setJustLanded(false);

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

void PlayerAvatar::setJustLanded(bool newJustLanded) {
    m_just_landed = newJustLanded;
}
bool PlayerAvatar::justLandedOn(Actor *other) const {
    return ((!getMoving()) && m_just_landed && isOn(other));
}
bool PlayerAvatar::isMovingOver(Actor *other) const {
    return (getMoving() && isOn(other));
}

// #####################################
// BADDIE : AVATAR

Baddie::Baddie(const int imageID, const int startX, const int startY) : Avatar(imageID, startX, startY) {
    m_pause_counter = 180;
    m_just_activated[0] = false;
    m_just_activated[1] = false;
}

void Baddie::doSomething() {
    
    if (!getMoving()) {
        for (int pN = 1; pN <= 2; pN++) {
            PlayerAvatar* player = getStudentWorld()->getPlayerWithNumber(pN);
            
            if (player != nullptr) {
                if (player->isOn(this) && (!player->getMoving())) {
                    if (getJustActivatedPlayer(pN)) continue; // already activated
                    handlePlayer(player);
                    setJustActivatedPlayer(pN, true);
                } else { // no longer colliding
                    setJustActivatedPlayer(pN, false);
                }
            }
        }
        
        m_pause_counter--;
        
        if (m_pause_counter == 0) {
            rollMove(10);
            pointInRandomValidDirection();
        }
    }
        
    if (getMoving()) {
        if (isDirectlyOnTopOfSquare() && isAtFork()) {
            pointInRandomValidDirection();
        } else if (isDirectlyOnTopOfSquare() && (!canWalkInDirection(getWalkDirection()))) {
            handleTurningPoint();
        }
        
        move();
        
        if (getTicksToMove() == 0) {
            setPauseCounter(180);
            
            handleLand();
        }
    }
    
}

int Baddie::getPauseCounter() const {
    return m_pause_counter;
}
void Baddie::setPauseCounter(int newPauseCount) {
    m_pause_counter = newPauseCount;
}

bool Baddie::getJustActivatedPlayer(int playerNum) const {
    return m_just_activated[playerNum - 1];
}
void Baddie::setJustActivatedPlayer(int playerNum, bool newJustActivated) {
    m_just_activated[playerNum - 1] = newJustActivated;
}

bool Baddie::isImpactable() {
    return true;
}
void Baddie::impact() {
    teleportToRandomSquare();
    setWalkDirection(right);
    setDirection(0);
    setMoving(false);
    setPauseCounter(180);
}

// #####################################
// BOWSER : BADDIE

Bowser::Bowser(const int startX, const int startY) : Baddie(IID_BOWSER, startX, startY) {}

void Bowser::handlePlayer(PlayerAvatar *player) { // TODO: ASK is it 50% chance of ever activating or 50% every tick
    if (randInt(1, 2) == 1) {
        player->changeCoins(-1 * player->getCoins());
        getStudentWorld()->playSound(SOUND_BOWSER_ACTIVATE);
    }
}
void Bowser::handleLand() {
    if (randInt(1, 4) == 1) {
        getStudentWorld()->removeSquareAt(getX(), getY());
        getStudentWorld()->addGridObject(new DroppingSquare(getX(), getY()));
        getStudentWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
    }
}

// #####################################
// BOO : BADDIE

Boo::Boo(const int startX, const int startY) : Baddie(IID_BOO, startX, startY) {
    
}

void Boo::handlePlayer(PlayerAvatar *player) { // TODO: is it 50% chance of ever activating or 50% every tick
    if (randInt(1, 1) == 1) {
        player->swapCoins(getStudentWorld()->getOtherPlayer(player));
    } else {
        player->swapStars(getStudentWorld()->getOtherPlayer(player));
    }
    getStudentWorld()->playSound(SOUND_BOO_ACTIVATE);
}
void Boo::handleLand() {}

// #####################################
// SQUARE : ACTOR

Square::Square(const int imageID, const int startX, const int startY, const int direction) : Actor(imageID, startX, startY, direction, 1) {}

void Square::doSomething() {
    if (!isActive()) return;
    
    for (int pN = 1; pN <= 2; pN++) {
        PlayerAvatar* player = getStudentWorld()->getPlayerWithNumber(pN);
        if (player != nullptr) handlePlayer(player);
    }
}

bool Square::canMove() const {
    return false;
}

// #####################################
// COINSQUARE : SQUARE

CoinSquare::CoinSquare(const int startX, const int startY, bool adds) : Square(adds ? IID_BLUE_COIN_SQUARE : IID_RED_COIN_SQUARE, startX, startY, right) {
    if (adds) m_delta_coins = 3;
    else m_delta_coins = -3;
}

void CoinSquare::handlePlayer(PlayerAvatar *player) {
    if (player->justLandedOn(this)) {
        player->changeCoins(m_delta_coins); // change coins already caps it
        if (m_delta_coins >= 0) getStudentWorld()->playSound(SOUND_GIVE_COIN);
        else getStudentWorld()->playSound(SOUND_TAKE_COIN);
    }
}

// #####################################
// STARSQUARE : SQUARE

StarSquare::StarSquare(const int startX, const int startY) : Square(IID_STAR_SQUARE, startX, startY, right) {}

void StarSquare::handlePlayer(PlayerAvatar *player) {
    //  player just landed on this or player is moving over this
    if (player->justLandedOn(this) || player->isMovingOver(this)) {
        
        if (player->getCoins() < 20) return;
        else {
            player->changeCoins(-20);
            player->changeStars(1);
            getStudentWorld()->playSound(SOUND_GIVE_STAR);
        }
        
    }
    
}

// #####################################
// DIRECTIONALSQUARE : SQUARE

DirectionalSquare::DirectionalSquare(const int startX, const int startY, int direction) : Square(IID_DIR_SQUARE, startX, startY, direction) {}

void DirectionalSquare::handlePlayer(PlayerAvatar *player) {
    if (player->justLandedOn(this) || player->isMovingOver(this)) {
        player->setForcedDirection(getDirection());
    }

}

// #####################################
// BANKSQUARE : SQUARE

BankSquare::BankSquare(const int startX, const int startY) : Square(IID_BANK_SQUARE, startX, startY, right) {}

void BankSquare::handlePlayer(PlayerAvatar *player) {

    if (player->justLandedOn(this)) {
        int bankCoins = getStudentWorld()->emptyBank();
        player->changeCoins(bankCoins);
        getStudentWorld()->playSound(SOUND_WITHDRAW_BANK);
    } else if (player->isMovingOver(this)) {
        int delta = player->changeCoins(-5);
        getStudentWorld()->changeBankCoins(-1 * delta);
        getStudentWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
    
}

// #####################################
// EVENTSQUARE : SQUARE

EventSquare::EventSquare(const int startX, const int startY) : Square(IID_EVENT_SQUARE, startX, startY, right) {}

void EventSquare::handlePlayer(PlayerAvatar *player) {

    if (player->justLandedOn(this)) {
        int option = randInt(1, 3);
        switch (option) {
            case 1:
                player->teleportToRandomSquare();
                getStudentWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            case 2:
                player->swapMovement(getStudentWorld()->getOtherPlayer(player));
                getStudentWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            case 3:
                player->setHasVortex(true);
                getStudentWorld()->playSound(SOUND_GIVE_VORTEX);
                break;
        }
    }
    
}

// #####################################
// DROPPINGSQUARE : SQUARE

DroppingSquare::DroppingSquare(const int startX, const int startY) : Square(IID_DROPPING_SQUARE, startX, startY, right) {}

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

// #####################################
// VORTEX : ACTOR

Vortex::Vortex(const int startX, const int startY, const int fireDirection) : Actor(IID_VORTEX, startX, startY, right, 0) {
    m_fire_direction = fireDirection;
}

void Vortex::doSomething() {
    if (!isActive()) return;
    
    int nX = getX(), nY = getY();
    getPositionInThisDirection(m_fire_direction, 2, nX, nY);
    moveTo(nX, nY);
    
    if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT) {
        setIsActive(false);
        return; // TODO: ASK RETURN?
    }
    
    Actor* overlappingActor = getStudentWorld()->getOneOverlappingImpactable(this);
    
    if (overlappingActor != nullptr) {
        overlappingActor->impact();
        setIsActive(false);
        getStudentWorld()->playSound(SOUND_HIT_BY_VORTEX);
    }
}

bool Vortex::canMove() const {
    return true;
}


