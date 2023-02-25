#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject {
    public:
        Actor(const int imageID, const int startX, const int startY, const int direction, const int depth);
        virtual void doSomething() = 0;
        bool isActive() const;
    private:
        bool active;
};


class Avatar: public Actor {
public:
    Avatar(const int imageID, const int startX, const int startY);
    virtual void doSomething() = 0;
private:
};

class PlayerAvatar: public Avatar {
public:
    PlayerAvatar(const int imageID, const int startX, const int startY, int playerNum);
    virtual void doSomething();
private:
    int m_playerNum;
};



class Square: public Actor {
    public:
        Square(const int imageID, const int startX, const int startY, const int direction);
        virtual void doSomething();
    private:
    
};

class CoinSquare: public Square {
    public:
        CoinSquare(const int startX, const int startY);
};

#endif // ACTOR_H_
