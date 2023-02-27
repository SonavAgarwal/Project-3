#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor: public GraphObject {
    public:
        Actor(const int imageID, const int startX, const int startY, const int direction, const int depth);
        virtual void doSomething() = 0;
        bool isActive() const;
        void setIsActive(bool newActive);
        void setStudentWorld(StudentWorld* sw);
        StudentWorld* getStudentWorld() const;
    private:
        bool m_active;
        StudentWorld* m_studentWorld; // TODO: IS THIS OK
};


class Avatar: public Actor {
public:
    Avatar(const int imageID, const int startX, const int startY);
    virtual void doSomething() = 0;
    int getTicksToMove() const;
    void setTicksToMove(int ticksToMove);
    bool canWalkInDirection(int walkDirection) const;
    int getWalkDirection() const;
    void setWalkDirection(int walkDirection);
private:
    int m_ticks_to_move;
    int m_walk_direction;
};

class PlayerAvatar: public Avatar {
public:
    PlayerAvatar(const int imageID, const int startX, const int startY, int playerNum);
    virtual void doSomething();
    
    int getCoins() const;
    int getStars() const;
    void changeCoins(int delta);
    void changeStars(int delta);
    int getDieRoll() const;
    
private:
    int m_playerNum;
    bool m_waiting_to_roll;
    int m_die_roll;
    int m_coins;
    int m_stars;
};



class Square: public Actor {
    public:
        Square(const int imageID, const int startX, const int startY, const int direction);
        virtual void doSomething(); // = 0; TODO: SHOULD THIS BE PURE VIRTUAL
    private:
    
};

class CoinSquare: public Square {
    public:
        CoinSquare(const int startX, const int startY, bool adds);
        virtual void doSomething();
    private:
        int m_delta_coins;
};

#endif // ACTOR_H_
