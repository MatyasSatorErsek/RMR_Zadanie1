#include "trajectory.h"
#include <fstream>


TrajectoryPlan::TrajectoryPlan(double room_size_, double tile_dim_,char* filename){

    roomSize = room_size_;
    tileDim = tile_dim_;
    numOfTiles = (int) roomSize/tileDim;

    tiles = new int*[numOfTiles];
    for (int i = 0; i < numOfTiles; ++i) {
       tiles[i] = new int[numOfTiles];
    }

    load_map(filename,mapArea);
    //std::cout<<"Hi"<<std::endl;
}

TrajectoryPlan::~TrajectoryPlan(){
    for(int i = 0; i < numOfTiles; i++){
        delete[] tiles[i];
    }
    delete tiles;
}

bool TrajectoryPlan::containsWall(int row , int col)
{
    Point topLeft,topRight,bottomLeft,bottomRight;
    bottomLeft = {row*tileDim,col*tileDim};
    bottomRight = {row*tileDim,(col+1)*tileDim};
    topLeft = {(row+1)*tileDim,(col+0)*tileDim};
    topRight = {(row+1)*tileDim,(col+1)*tileDim};


    for(int i = 0; i < mapArea.wall.numofpoints;  i++){
        Point wallPointA, wallPointB;

        if(i == 0){
            wallPointA = mapArea.wall.points[mapArea.wall.numofpoints -1].point;
        }
        else{
            wallPointA = mapArea.wall.points[i-1].point;
        }
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

void TrajectoryPlan::makeTiles(){
    for(int i = 0; i < numOfTiles; i++){
        for(int j = 0; j <  numOfTiles; j++){
            if(containsWall(i,j)){
                tiles[i][j] = 1;

            }
            else{
                tiles[i][j] = 0;
            }
            //std::cout<<tiles[i][j]<<" ";
        }
        //std::cout<<std::endl;
    }
}


void TrajectoryPlan::printField()
{
    std::ofstream MyFile("C:\\Codes\\RMR\\RMR_Zadanie1\\demoRMR\\myfile.txt");

    for(int i = 0; i < numOfTiles; i++){
        for(int j = 0; j < numOfTiles; j++){
            std::cout<<tiles[j][numOfTiles- i -1]<<" ";
            //std::cout<<tiles[i][j]<<" ";
            MyFile<<tiles[j][numOfTiles- i -1]<<" ";

        }
        std::cout<<std::endl;
        MyFile<<std::endl;
    }
    MyFile.close();
}

void TrajectoryPlan::checkObstacles(TMapObject obstacle)
{
    /*
    for(int i = 0; i < obstacle.numofpoints; i++){
        auto currPoint = obstacle.points.at(i%obstacle.numofpoints);
        auto nextPoint = obstacle.points.at((i+1)%obstacle.numofpoints);

        int tileXPosCurr = (int) currPoint.point.x/tileDim;
        int tileYPosCurr = (int) currPoint.point.y/tileDim;

        int tileXPosNext = (int) nextPoint.point.x/tileDim;
        int tileYPosNext = (int) nextPoint.point.y/tileDim;

        int minx = std::min(tileXPosCurr,tileXPosNext);
        int miny = std::min(tileYPosCurr,tileYPosNext);

        int maxx = std::max(tileXPosCurr,tileXPosNext);
        int maxy = std::max(tileYPosCurr,tileYPosNext);

        if (maxy - miny == maxx- minx){
            for (int x = minx, y = miny; x <= maxx && x <= maxy; x++, y++) {
                tiles[x][y] = 1;
            }
        }
        else if (maxy - miny == minx - maxy) {
            for (int x = minx, y = maxy; x <= maxx && x <= miny; x++, y--) {
                tiles[x][y] = 1;
            }
        }

    }*/

    double minx,miny,maxx,maxy;
    minx = obstacle.points.at(0).point.x;
    miny = obstacle.points.at(0).point.y;
    maxx = obstacle.points.at(0).point.x;
    maxy = obstacle.points.at(0).point.y;

    for(int i = 1; i < obstacle.numofpoints; i++){
        auto x = obstacle.points.at(i).point.x;
        auto y = obstacle.points.at(i).point.y;

        minx = std::min(minx,x);
        miny = std::min(miny,y);
        maxx = std::max(maxx,x);
        maxy = std::max(maxy,y);

    }
    int minXIdx = (int) minx/tileDim;
    int minYIdx = (int) miny/tileDim;
    int maxXIdx = (int) maxx/tileDim;
    int maxYIdx = (int) maxy/tileDim;

    for(int i = minXIdx; i < maxXIdx; i++){
        for(int j = minYIdx; j < maxYIdx; j++){
            tiles[i][j] = 1;
        }
    }

}

void TrajectoryPlan::findObstacles()
{
    for(int i = 0; i < mapArea.numofObjects; i++){
        checkObstacles(mapArea.obstacle.at(i));
    }
}
