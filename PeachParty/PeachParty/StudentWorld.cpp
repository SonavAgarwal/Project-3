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

#include "Actor.h"

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    string boardPath = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    cerr << boardPath << endl;
    board.loadBoard(boardPath);
    
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            cerr << board.getContentsOf(x, y) << endl;
            
            Board::GridEntry ge = board.getContentsOf(x, y);
            
            switch (ge) {
                case Board::empty: {
                    break;
                }
                case Board::blue_coin_square: {
                    Actor* gop = new CoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                    objects.push_back(gop);
                    break;
                }
                case Board::player: {
                    Actor* gop = new CoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                    objects.push_back(gop);
                    peach = new PlayerAvatar(IID_PEACH, x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                    objects.push_back(peach);
                    yoshi = new PlayerAvatar(IID_YOSHI, x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                    objects.push_back(yoshi);
                    break;
                }
                    
            }
//            objects.push_back(new )
        }
    }
    
    // initialize data structure
    
    
	startCountdownTimer(20);  // this placeholder causes timeout after 5 seconds
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    
    // ask all objects to do something
    
    for (vector<Actor*>::iterator it = objects.begin(); it != objects.end(); it++) {
        if (!(*it)->isActive()) {
            (*it)->doSomething();
        }
    }
    
    // delete dead
    
    for (vector<Actor*>::iterator it = objects.begin(); it != objects.end(); it++) {
        if (!(*it)->isActive()) {
            delete *it;
            it = objects.erase(it);
        }
    }
    
    // status text update
    setGameStatText("Game will end in a few seconds");
    
    
    // game out of time?
    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        setFinalScore(6, 9); // TODO: FIX
        return GWSTATUS_PEACH_WON; // TODO: FIX
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}
