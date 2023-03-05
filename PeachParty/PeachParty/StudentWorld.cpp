#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

#include <iostream>
#include <sstream>
#include "Actor.h"

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    
    m_peach = nullptr;
    m_yoshi = nullptr;
    m_bank_coins = 0;
    
    string boardPath = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
//    cerr << boardPath << endl;
    m_board.loadBoard(boardPath);
    
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
//            cerr << m_board.getContentsOf(x, y) << endl;
            
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
                    // TODO: I THINK PEACH IS NULL????? OR MAYBE NOT
                    
//                    m_yoshi = new PlayerAvatar(IID_YOSHI, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 2);
//                    addGridObject(m_yoshi);
                    // TODO: BRING HIM BACK
                    
                    break;
                }
                case Board::bowser: {
                    Actor* gop = new CoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true);
                    addGridObject(gop);
                    break;
                }
                case Board::boo: {
                    Actor* gop = new CoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true);
                    addGridObject(gop);
                    break;
                }
                
                    
            }
        }
    }
    
    
    
	startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::addGridObject(Actor* gridObjectPointer) {
    gridObjectPointer->setStudentWorld(this);
    m_objects.push_back(gridObjectPointer);
}

int StudentWorld::move()
{
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    
    // ask all objects to do something
    
    for (vector<Actor*>::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        if ((*it)->isActive()) {
            (*it)->doSomething();
        }
    }
    
    // delete dead
    
    for (vector<Actor*>::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        if (!(*it)->isActive()) {
            delete *it;
            it = m_objects.erase(it); // TODO: OH LORDY LORD
            it--;
        }
    }
    
    // status text update
    
    ostringstream gst;
    
    // P1 Roll: 3 Stars: 2 $$: 15 | Time: 75 | Bank: 9 | P2 Roll: 0 Stars: 1 $$: 22 VOR
    
    gst << "P1 Roll: " << m_peach->getSquaresToMove() << " Stars: " << m_peach->getStars() << " $$: " << m_peach->getCoins();
    
    // VOR
    if (m_peach->hasVortex()) gst << " VOR";
    
//    gst << " | Time: " << timeRemaining() << " | Bank: " << getBankCoins() << " | P2 Roll: " << m_yoshi->getSquaresToMove() << " Stars: " << m_yoshi->getStars() << " $$: " << m_yoshi->getCoins();
//    
//    // VOR
//    if (m_yoshi->hasVortex()) gst << " VOR";
    
    setGameStatText(gst.str());
    
    
    // game out of time?
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
        it--; // TODO: OH LORDY LORD
    }
}


Board& StudentWorld::getBoard() {
    return m_board;
}

bool StudentWorld::isWalkable(int screenX, int screenY) {
    Board::GridEntry ge = m_board.getContentsOf(screenX / SPRITE_WIDTH, screenY / SPRITE_HEIGHT); // should round down properly?? I hope
    
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

void StudentWorld::changeBankCoins(int delta) { // TODO: do we need a safeguard
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
        
        if (m_objects[i]->canMove() && m_objects[i]->isActive()) {
            return m_objects[i];
        }
        
        i++;
        i %= m_objects.size();
    }
    
    return nullptr;
}


StudentWorld::~StudentWorld() {
    cleanUp();
}
