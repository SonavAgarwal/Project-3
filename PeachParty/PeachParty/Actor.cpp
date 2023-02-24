#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp



#include "GameConstants.h"


Actor::Actor(const int imageID, const int startX, const int startY) : GraphObject(imageID, startX, startY) {
    
}


PlayerAvatar::PlayerAvatar(const int imageID, const int startX, const int startY) : Actor(imageID, startX, startY) {
    
}


void PlayerAvatar::doSomething() {
    //
}


Square::Square(const int imageID, const int startX, const int startY) : Actor(imageID, startX, startY) {
    
}

void Square::doSomething() {
    //
}

CoinSquare::CoinSquare(const int startX, const int startY) : Square(IID_BLUE_COIN_SQUARE, startX, startY) {
    
}
