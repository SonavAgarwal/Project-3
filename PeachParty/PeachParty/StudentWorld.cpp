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
                case Board::blue_coin_square: {
                    Actor* gop = new CoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true);
                    addGridObject(gop);
                    break;
                }
                case Board::player: {
                    Actor* gop = new CoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true);
                    addGridObject(gop);
                    
                    m_peach = new PlayerAvatar(IID_PEACH, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 1);
                    addGridObject(m_peach);
                    // TODO: I THINK PEACH IS NULL????? OR MAYBE NOT
                    
                    m_yoshi = new PlayerAvatar(IID_YOSHI, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 2);
                    addGridObject(m_yoshi);
                    // TODO: BRING HIM BACK
                    
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
    
    gst << "P1 Roll: " << m_peach->getDieRoll() << " Stars: " << m_peach->getStars() << " $$: " << m_peach->getCoins();
    // VOR
    gst << " | Time: " << timeRemaining() << " | Bank: " << getBankCoins() << " | P2 Roll: " << m_yoshi->getDieRoll() << " Stars: " << m_yoshi->getStars() << " $$: " << m_yoshi->getCoins();
    // VOR
    
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

int StudentWorld::getBankCoins() const {
    return m_bank_coins;
}
void StudentWorld::changeBankCoins(int delta) { // TODO: do we need a safeguard
    m_bank_coins += delta;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}
