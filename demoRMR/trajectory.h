#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <vector>
#include "map_loader.h"
#include <iostream>>

#define ROOM_SIZE 600.0
#define TILE_DIM 10



using std::vector;

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


private:
    double roomSize;
    double tileDim;
    int numOfTiles;

    int** tiles;
    TMapArea mapArea;


};

#endif // TRAJECTORY_H
