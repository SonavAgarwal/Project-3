#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

// V1

class Actor;
class PlayerAvatar;

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath);
        virtual int init();
        virtual int move();
        virtual void cleanUp();
    
        Board& getBoard();
        bool isWalkable(int screenX, int screenY);
        int emptyBank();
        int getBankCoins() const;
        void changeBankCoins(int delta);
    
        PlayerAvatar* getPlayerWithNumber(int playerNum) const;
        PlayerAvatar* getOtherPlayer(PlayerAvatar* thisPlayer) const;
    
        Actor* getRandomSquare() const;
    
        void removeSquareAt(int rX, int rY);
        void addGridObject(Actor* gridObjectPointer);

        Actor* getOneOverlappingImpactable(Actor* actor1);
    
        ~StudentWorld();

    private:
        
        std::vector<Actor*> m_objects;
        PlayerAvatar* m_peach;
        PlayerAvatar* m_yoshi;
        Board m_board;
    
        int m_bank_coins;
};

#endif // STUDENTWORLD_H_
