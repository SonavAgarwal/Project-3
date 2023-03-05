#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

//#include "GraphObject.h"
class Actor;
class PlayerAvatar;
class Square;

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath);
        virtual int init();
        virtual int move();
        virtual void cleanUp();
    
        Board& getBoard();
        bool isWalkable(int screenX, int screenY); // CANT BE CONST BC OF STUPID BOARD CLASS
        int emptyBank();
        int getBankCoins() const;
        void changeBankCoins(int delta);
    
        PlayerAvatar* getPlayerWithNumber(int playerNum) const;
        PlayerAvatar* getOtherPlayer(PlayerAvatar* thisPlayer) const;
    
        Actor* getRandomSquare() const;
    
        ~StudentWorld(); // TODO: IS THIS NECESSARY

    private:
        
        std::vector<Actor*> m_objects;
        PlayerAvatar* m_peach;
        PlayerAvatar* m_yoshi;
        Board m_board;
    
        int m_bank_coins;
        
        void addGridObject(Actor* gridObjectPointer);

    
};

#endif // STUDENTWORLD_H_
