#include "trajectory.h"

TrajectoryPlan::TrajectoryPlan(double room_size_, double tile_dim_,char* filename){

    roomSize = room_size_;
    tileDim = tile_dim_;
    numOfTiles = (int) roomSize/tileDim;

    // Reserve the size of the vector
    tiles.reserve(numOfTiles);
    for(int i = 0; i < numOfTiles; i++){
        tiles[i].reserve(numOfTiles);
    }

    load_map(filename,mapArea);

}

bool TrajectoryPlan::containsWall(int row , int col)
{
    Point topLeft,topRight,bottomLeft,bottomRight;
    bottomLeft = {row*tileDim,col*tileDim};
    bottomRight = {row*tileDim,(col+1)*tileDim};
    topLeft = {(row+1)*tileDim,(col+0)*tileDim};
    topRight = {(row+1)*tileDim,(col+1)*tileDim};


    for(int i = 1; i < mapArea.wall.numofpoints;  i++){
        Point wallPointA, wallPointB;
        wallPointA = mapArea.wall.points[i-1].point;
        wallPointB = mapArea.wall.points[i].point;

        //If point A is higher than point B
        if(wallPointB.y < wallPointA.y){
            Point t = wallPointB;
            wallPointB = wallPointA;
            wallPointA = t;
        }

        bool A1  = ((wallPointB.x > topRight.x) && (wallPointB.y > topRight.y)) && ((wallPointA.x < bottomLeft.x) && (wallPointA.y < bottomLeft.y));
        bool A2  = ((wallPointB.x < topLeft.x) && (wallPointB.y > topLeft.y)) && ((wallPointA.x > bottomRight.x) && (wallPointA.y < bottomRight.y));

        if(A1 || A2)
            return true;

    }

    return false;
}
