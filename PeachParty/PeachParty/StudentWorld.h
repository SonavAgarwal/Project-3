#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

//#include "GraphObject.h"
class GraphObject; // TODO: WTF

class StudentWorld : public GameWorld
{
    public:
      StudentWorld(std::string assetPath);
      virtual int init();
      virtual int move();
      virtual void cleanUp();

    private:
        
        std::vector<GraphObject*> objects;
        Board board;
        
};

#endif // STUDENTWORLD_H_
