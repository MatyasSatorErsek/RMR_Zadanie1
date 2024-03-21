#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <vector>
#include "map_loader.h"

#define ROOM_SIZE 500.0
#define TILE_DIM 20

using std::vector;

class TrajectoryPlan
{
public:
    TrajectoryPlan(double room_size_,double tile_dim_,char* filename);

    bool containsWall(int row, int col);

private:
    double roomSize;
    double tileDim;
    int numOfTiles;

    vector<vector<int>> tiles;
    TMapArea mapArea;


};

#endif // TRAJECTORY_H
