#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <vector>

#define ROOM_SIZE 5.0
#define TILE_DIM 0.2

using std::vector;

class TrajectoryPlan
{
public:
    TrajectoryPlan(double room_size_,double tile_dim_);

private:
    double roomSize;
    double tileDim;
    int numOfTiles;

    vector<vector<int>> tiles;

};

#endif // TRAJECTORY_H
