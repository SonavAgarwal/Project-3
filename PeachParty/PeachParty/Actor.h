#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject {
    public:
        Actor(const int imageID, const int startX, const int startY);
        virtual void doSomething() = 0;
        bool isActive() const;
    private:
        bool active;
};

class PlayerAvatar: public Actor {
public:
    PlayerAvatar(const int imageID, const int startX, const int startY);
    virtual void doSomething();
};



class Square: public Actor {
    public:
        Square(const int imageID, const int startX, const int startY);
        virtual void doSomething();
    private:
    
};

class CoinSquare: public Square {
    public:
        CoinSquare(const int startX, const int startY);
};

#endif // ACTOR_H_
