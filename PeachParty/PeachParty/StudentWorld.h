#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

//#include "GraphObject.h"
class Actor; // TODO: WTF

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath);
        virtual int init();
        virtual int move();
        virtual void cleanUp();
    
        Board& getBoard();
    
        ~StudentWorld(); // TODO: IS THIS NECESSARY

    private:
        
        std::vector<Actor*> objects;
        Actor* peach;
        Actor* yoshi;
        Board board;
        
};

#endif // STUDENTWORLD_H_
