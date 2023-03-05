#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
//class Square;

class Actor: public GraphObject {
    public:
        Actor(const int imageID, const int startX, const int startY, const int direction, const int depth);
        virtual void doSomething() = 0;
        bool isActive() const;
        void setIsActive(bool newActive);
        void setStudentWorld(StudentWorld* sw);
        StudentWorld* getStudentWorld() const;
        bool isOn(Actor* other) const;
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
    
    int getSquaresToMove() const;
    void rollMove(int maxRoll);
    void handleTurningPoint();
    void move();
    bool getMoving() const;
    void pointInRandomValidDirection();
    void updateSpriteDirection(); // TODO: MAKE SOME THINGS PRIVATE
    bool isAtFork();
    void setMoving(bool newMoving);
    
    bool getJustLanded() const; // TODO: UNUSED
    void setJustLanded(bool newJustLanded);
    bool justLandedOn(Actor* other) const;
    bool isMovingOver(Actor* other) const;
    
    // TODO: for teleporting what happens to landing
    void teleportToRandomSquare();
    
//    bool keyDirectionOpposesCurrentDirection();
        
private:
    int m_ticks_to_move;
    int m_walk_direction;
    int m_squares_to_move;
    bool m_moving;
    bool m_just_landed;
};

class PlayerAvatar: public Avatar {
public:
    PlayerAvatar(const int imageID, const int startX, const int startY, int playerNum);
    virtual void doSomething();
    
    int getCoins() const;
    int getStars() const;
    int changeCoins(int delta);
    void changeStars(int delta);
    int getDieRoll() const;
    
    void swapCoins(PlayerAvatar* other);
    void swapStars(PlayerAvatar* other);
    // TODO: get a vortex
    void swapMovement(PlayerAvatar* other);
    
    bool hasVortex() const;
    void setHasVortex(bool newHasVortex);
    
private:
    int m_playerNum;
//    bool m_waiting_to_roll;
    int m_coins;
    int m_stars;
    bool m_has_vortex;
};



class Square: public Actor {
    public:
        Square(const int imageID, const int startX, const int startY, const int direction);
        virtual void doSomething(); // = 0; // TODO: SHOULD THIS BE PURE VIRTUAL
        virtual void handlePlayer(PlayerAvatar* player) = 0;
    private:
        
};

class CoinSquare: public Square {
    public:
        CoinSquare(const int startX, const int startY, bool adds);
//        virtual void doSomething();
        virtual void handlePlayer(PlayerAvatar* player);
    private:
        int m_delta_coins;
};


class StarSquare: public Square {
    public:
        StarSquare(const int startX, const int startY);
//        virtual void doSomething();
        virtual void handlePlayer(PlayerAvatar* player);
};


class DirectionalSquare: public Square {
    public:
        DirectionalSquare(const int startX, const int startY, int direction);
//        virtual void doSomething();
        virtual void handlePlayer(PlayerAvatar* player);
    private:
//        int forcingDirection;
};


class BankSquare: public Square {
    public:
        BankSquare(const int startX, const int startY);
        virtual void handlePlayer(PlayerAvatar* player);
//        virtual void doSomething();
};


class EventSquare: public Square {
    public:
        EventSquare(const int startX, const int startY);
        virtual void handlePlayer(PlayerAvatar* player);
//        virtual void doSomething();
};



class DroppingSquare: public Square {
    public:
        DroppingSquare(const int startX, const int startY);
        virtual void handlePlayer(PlayerAvatar* player);
//        virtual void doSomething();
};


#endif // ACTOR_H_
