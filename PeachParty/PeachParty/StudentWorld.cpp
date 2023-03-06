#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

// V2

#include <iostream>
#include <sstream>
#include "Actor.h"

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{}

int StudentWorld::init()
{
    
    m_peach = nullptr;
    m_yoshi = nullptr;
    m_bank_coins = 0;
    
    string boardPath = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult loadResult = m_board.loadBoard(boardPath);
    
    if (loadResult != Board::load_success) return GWSTATUS_BOARD_ERROR;
    
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            
            Board::GridEntry ge = m_board.getContentsOf(x, y);
            
            switch (ge) {
                    
                case Board::empty: {
                    break;
                }
                    
                // SQUARES
                    
                case Board::blue_coin_square: {
                    Actor* gop = new CoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true);
                    addGridObject(gop);
                    break;
                }
                case Board::red_coin_square: {
                    Actor* gop = new CoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, false);
                    addGridObject(gop);
                    break;
                }
                case Board::bank_square: {
                    Actor* gop = new BankSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                    addGridObject(gop);
                    break;
                }
                case Board::down_dir_square: {
                    Actor* gop = new DirectionalSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, Actor::down);
                    addGridObject(gop);
                    break;
                }
                case Board::up_dir_square: {
                    Actor* gop = new DirectionalSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, Actor::up);
                    addGridObject(gop);
                    break;
                }
                case Board::left_dir_square: {
                    Actor* gop = new DirectionalSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, Actor::left);
                    addGridObject(gop);
                    break;
                }
                case Board::right_dir_square: {
                    Actor* gop = new DirectionalSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, Actor::right);
                    addGridObject(gop);
                    break;
                }
                case Board::event_square: {
                    Actor* gop = new EventSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                    addGridObject(gop);
                    break;
                }
                case Board::star_square: {
                    Actor* gop = new StarSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                    addGridObject(gop);
                    break;
                }
                    
                // AVATARS
                
                case Board::player: {
                    Actor* gop = new CoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true);
                    addGridObject(gop);
                    
                    m_peach = new PlayerAvatar(IID_PEACH, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 1);
                    addGridObject(m_peach);
                    
                    m_yoshi = new PlayerAvatar(IID_YOSHI, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 2);
                    addGridObject(m_yoshi);
                    
                    break;
                }
                case Board::bowser: {
                    Actor* gop = new CoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true);
                    addGridObject(gop);
                    
                    gop = new Bowser(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                    addGridObject(gop);
                    
                    break;
                }
                case Board::boo: {
                    Actor* gop = new CoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true);
                    addGridObject(gop);
                    
                    gop = new Boo(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                    addGridObject(gop);
                    
                    break;
                }
                
                    
            }
        }
    }
    
	startCountdownTimer(99);
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::addGridObject(Actor* gridObjectPointer) {
    gridObjectPointer->setStudentWorld(this);
    m_objects.push_back(gridObjectPointer);
}

int StudentWorld::move()
{
    
    // Have all actors do something
    for (vector<Actor*>::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        if ((*it)->isActive()) {
            (*it)->doSomething();
        }
    }
    
    // Delete inactive actors
    for (vector<Actor*>::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        if (!(*it)->isActive()) {
            delete *it;
            it = m_objects.erase(it);
            it--;
        }
    }
    
    // Update status text
    ostringstream gst;
    // Format: P1 Roll: 3 Stars: 2 $$: 15 | Time: 75 | Bank: 9 | P2 Roll: 0 Stars: 1 $$: 22 VOR
    
    gst << "P1 Roll: " << m_peach->getSquaresToMove() << " Stars: " << m_peach->getStars() << " $$: " << m_peach->getCoins();
    // Vortex
    if (m_peach->hasVortex()) gst << " VOR";
    
    gst << " | Time: " << timeRemaining() << " | Bank: " << getBankCoins() << " | P2 Roll: " << m_yoshi->getSquaresToMove() << " Stars: " << m_yoshi->getStars() << " $$: " << m_yoshi->getCoins();
    // Vortex
    if (m_yoshi->hasVortex()) gst << " VOR";
    
    setGameStatText(gst.str());
    
    // Check if game is over
    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        
        int winner = -1; // -1 = undecided, 1 = peach, 2 = yoshi
        if (m_peach->getStars() > m_yoshi->getStars()) {
            winner = 1;
        } else if (m_yoshi->getStars() > m_peach->getStars()) {
            winner = 2;
        } else {
            if (m_peach->getCoins() > m_yoshi->getCoins()) {
                winner = 1;
            } else if (m_yoshi->getCoins() > m_peach->getCoins()) {
                winner = 2;
            } else {
                winner = randInt(1, 2);
            }
        }
        
        if (winner == 1) {
            setFinalScore(m_peach->getStars(), m_peach->getCoins());
            return GWSTATUS_PEACH_WON;
        } else {
            setFinalScore(m_yoshi->getStars(), m_yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (vector<Actor*>::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        delete *it;
        it = m_objects.erase(it);
        it--;
    }
}

Board& StudentWorld::getBoard() {
    return m_board;
}

bool StudentWorld::isWalkable(int screenX, int screenY) {
    Board::GridEntry ge = m_board.getContentsOf(screenX / SPRITE_WIDTH, screenY / SPRITE_HEIGHT);
    
    if (ge == Board::empty) return false;
    else return true;
}

int StudentWorld::emptyBank() {
    int tempBankCoins = m_bank_coins;
    m_bank_coins = 0;
    return tempBankCoins;
}
int StudentWorld::getBankCoins() const {
    return m_bank_coins;
}
void StudentWorld::changeBankCoins(int delta) {
    m_bank_coins += delta;
}

PlayerAvatar* StudentWorld::getPlayerWithNumber(int playerNum) const {
    switch (playerNum) {
        case 1:
            return m_peach;
        case 2:
            return m_yoshi;
        default:
            return nullptr;
    }
}
PlayerAvatar* StudentWorld::getOtherPlayer(PlayerAvatar* thisPlayer) const {
    if (thisPlayer == m_peach) return m_yoshi;
    else return m_peach;
}

Actor* StudentWorld::getRandomSquare() const {
    int i = randInt(0, ((int) m_objects.size()) - 1);
    for (int k = 0; k < m_objects.size(); k++) {
        
        if ((!m_objects[i]->canMove()) && m_objects[i]->isActive()) { // squares cannot move
            return m_objects[i];
        }
        
        i++;
        i %= m_objects.size();
    }
    
    return nullptr;
}

void StudentWorld::removeSquareAt(int rX, int rY) {// flags square for removal
    for (vector<Actor*>::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        Actor* actor = *it;
        if ((!actor->canMove()) && (actor->getX() == rX) && (actor->getY() == rY)) {
            actor->setIsActive(false);
        }
    }
}

Actor* StudentWorld::getOneOverlappingImpactable(Actor* actor1) {
    for (vector<Actor*>::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        Actor* actor2 = *it;
        if (actor2->isImpactable() && actor2->overlapsWith(actor1)) {
            return actor2;
        }
    }
    return nullptr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}
