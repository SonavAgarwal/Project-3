#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// V2

class StudentWorld;

class Actor: public GraphObject {
    public:
        Actor(const int imageID, const int startX, const int startY, const int direction, const int depth);
        virtual void doSomething() = 0;
        bool isActive() const;
        void setIsActive(bool newActive);
        void setStudentWorld(StudentWorld* sw);
        StudentWorld* getStudentWorld() const;
        bool isOn(Actor* other) const;
        virtual bool canMove() const = 0;
        virtual bool isImpactable();
        virtual void impact();
        virtual bool overlapsWith(Actor* other) const;
    private:
        bool m_active;
        StudentWorld* m_studentWorld;
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
        bool isDirectlyOnTopOfSquare() const;
        
        int getSquaresToMove() const;
        void rollMove(int maxRoll);
        void handleTurningPoint();
        void move();
        bool getMoving() const;
        void pointInRandomValidDirection();
        void updateSpriteDirection();
        bool isAtFork();
        void setMoving(bool newMoving);
            
        void teleportToRandomSquare();
        
        virtual bool canMove() const;
                
    private:
        int m_ticks_to_move;
        int m_walk_direction;
        bool m_moving;
};

class PlayerAvatar: public Avatar {
    public:
        PlayerAvatar(const int imageID, const int startX, const int startY, int playerNum);
        virtual void doSomething();
        
        int getCoins() const;
        int getStars() const;
        int changeCoins(int delta);
        void changeStars(int delta);
        
        void swapCoins(PlayerAvatar* other);
        void swapStars(PlayerAvatar* other);
        void swapMovement(PlayerAvatar* other);
        
        bool hasVortex() const;
        void setHasVortex(bool newHasVortex);
        
        void setForcedDirection(int newDirection);
        
        void setJustLanded(bool newJustLanded);
        bool justLandedOn(Actor* other) const;
        bool isMovingOver(Actor* other) const;
        
    private:
        int m_playerNum;
        int m_coins;
        int m_stars;
        bool m_has_vortex;
        
        int m_forced_direction;
        bool m_just_landed;

};

class Baddie: public Avatar {
    public:
        Baddie(const int imageID, const int startX, const int startY);
        virtual void doSomething();
        
        int getPauseCounter() const;
        void setPauseCounter(int newPauseCount);
        
        bool getJustActivatedPlayer(int playerNum) const;
        void setJustActivatedPlayer(int playerNum, bool newJustActivated);
        
        virtual bool isImpactable();
        virtual void impact();
    
        virtual int getMaxRoll() const = 0;
        
    private:
        int m_pause_counter;
        bool m_just_activated[2];
    
        virtual void handlePlayer(PlayerAvatar* player) = 0;
        virtual void handleLand() = 0;
};
class Bowser: public Baddie {
    public:
        Bowser(const int startX, const int startY);
        virtual void handlePlayer(PlayerAvatar* player);
        virtual void handleLand();
        virtual int getMaxRoll() const;
};
class Boo: public Baddie {
    public:
        Boo(const int startX, const int startY);
        virtual void handlePlayer(PlayerAvatar* player);
        virtual void handleLand();
        virtual int getMaxRoll() const;
};

class Square: public Actor {
    public:
        Square(const int imageID, const int startX, const int startY, const int direction);
        virtual void doSomething();
        virtual bool canMove() const;
    private:
        virtual void handlePlayer(PlayerAvatar* player) = 0;
};
class CoinSquare: public Square {
    public:
        CoinSquare(const int startX, const int startY, bool adds);
    private:
        virtual void handlePlayer(PlayerAvatar* player);
        int m_delta_coins;
};
class StarSquare: public Square {
    public:
        StarSquare(const int startX, const int startY);
    private:
        virtual void handlePlayer(PlayerAvatar* player);
};
class DirectionalSquare: public Square {
    public:
        DirectionalSquare(const int startX, const int startY, int direction);
    private:
        virtual void handlePlayer(PlayerAvatar* player);
};
class BankSquare: public Square {
    public:
        BankSquare(const int startX, const int startY);
    private:
        virtual void handlePlayer(PlayerAvatar* player);
};
class EventSquare: public Square {
    public:
        EventSquare(const int startX, const int startY);
    private:
        virtual void handlePlayer(PlayerAvatar* player);
};
class DroppingSquare: public Square {
    public:
        DroppingSquare(const int startX, const int startY);
    private:
        virtual void handlePlayer(PlayerAvatar* player);
};

class Vortex : public Actor {
    public:
        Vortex(const int startX, const int startY, const int direction);
        virtual void doSomething();
        virtual bool canMove() const;
    private:
        int m_fire_direction;
};

#endif // ACTOR_H_
