#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <vector>
#include "map_loader.h"
#include <iostream>
#include <queue>


#define ROOM_SIZE 750.0
#define TILE_DIM 25
#define START -1
#define TARGET 2
#define VERTICAL_SHIFT 100.0


using std::vector;
using std::queue;

typedef struct position
{
    double x;
    double y;
    double phi;

    position(double xRef,double yRef, double phiRef)
    {
        x = xRef;
        y = yRef;
        phi = phiRef;

    }

}Position;

class TrajectoryPlan
{
public:
    TrajectoryPlan(double room_size_,double tile_dim_,char* filename);
    ~TrajectoryPlan();

    bool containsWall(int row, int col);
    void makeTiles();
    void printField();
    void checkObstacles(TMapObject obstacle);
    void findObstacles();
    bool markStart(double x, double y);
    bool markTarget(double x, double y);
    void labelTiles();
    void makeWallTiles();
    void generateTrajectory(queue<Position>& q);

    int** tiles;
    int numOfTiles;


private:
    double roomSize;
    double tileDim;


    int startRow;
    int startCol;
    int targetRow;
    int targetCol;

    double startx;
    double starty;
    double targetx;
    double targety;


    TMapArea mapArea;


};



#endif // TRAJECTORY_H
