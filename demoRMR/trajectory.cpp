#include "trajectory.h"

TrajectoryPlan::TrajectoryPlan(double room_size_, double tile_dim_){

    roomSize = room_size_;
    tileDim = tile_dim_;
    numOfTiles = (int) roomSize/tileDim;

    // Reserve the size of the vector
    tiles.reserve(numOfTiles);
    for(int i = 0; i < numOfTiles; i++){
        tiles[i].reserve(numOfTiles);
    }

}
