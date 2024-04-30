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

void TrajectoryPlan::makeTiles(){
    for(int i = 0; i < numOfTiles; i++){
        for(int j = 0; j <  numOfTiles; j++){

            if(i == 0 || j == 0 || i == (numOfTiles -1) || j == (numOfTiles -1)){
                tiles[i][j] = 1;
            }
            else{
                tiles[i][j] = 0;
            }
        }
    }
}


void TrajectoryPlan::printField()
{

    std::ofstream MyFile("C:\\Codes\\RMR\\RMR_Zadanie1\\demoRMR\\myfile.txt");

    for(int i = 0; i < numOfTiles; i++){
        for(int j = 0; j < numOfTiles; j++){
            //std::cout<<tiles[j][numOfTiles- i -1]<<" ";
            //std::cout<<tiles[i][j]<<" ";
            MyFile<<tiles[j][numOfTiles- i -1]<<" ";

        }
        //std::cout<<std::endl;
        MyFile<<std::endl;
    }
    MyFile.close();
}

void TrajectoryPlan::checkObstacles(TMapObject obstacle)
{

    for(int i = 0; i < obstacle.numofpoints; i++){
        auto currPoint = obstacle.points.at(i%obstacle.numofpoints);
        auto nextPoint = obstacle.points.at((i+1)%obstacle.numofpoints);

        int tileXPosCurr = (int) currPoint.point.x/tileDim;
        int tileYPosCurr = (int) (currPoint.point.y+VERTICAL_SHIFT)/tileDim;

        int tileXPosNext = (int) nextPoint.point.x/tileDim;
        int tileYPosNext = (int) (nextPoint.point.y+VERTICAL_SHIFT)/tileDim;

        int minx = std::min(tileXPosCurr,tileXPosNext);
        int miny = std::min(tileYPosCurr,tileYPosNext);

        int maxx = std::max(tileXPosCurr,tileXPosNext);
        int maxy = std::max(tileYPosCurr,tileYPosNext);

        for(int x = minx; x <= maxx; x++){
            for(int y = miny; y <= maxy; y++){
                tiles[x][y] = 1;
            }
        }

    }


}

void TrajectoryPlan::findObstacles()
{
    for(int i = 0; i < mapArea.numofObjects; i++){
        checkObstacles(mapArea.obstacle.at(i));
    }
}


bool TrajectoryPlan::markStart(double x, double y)
{
    startRow = (int) x/tileDim;
    startCol = (int) (y+VERTICAL_SHIFT)/tileDim;

    startx = x;
    starty = y;

    if(tiles[startRow][startCol] == 0){
        //tiles[startRow][startCol] = START;
        return true;
    }
    else{
        std::cout<< "Unable to mark start";
        return false;
    }

}

bool TrajectoryPlan::markTarget(double x, double y)
{
    targetRow = (int) x/tileDim;
    targetCol = (int) (y+VERTICAL_SHIFT)/tileDim;

    targetx = x;
    targety = y;

    if(tiles[targetRow][targetCol] == 0){
        tiles[targetRow][targetCol] = TARGET;
        return true;
    }
    else{
        std::cout<< "Unable to mark target";
        return false;
    }

}

void TrajectoryPlan::labelTiles(){
    int label = 3;
    while((tiles[startRow][startCol +1] ==0 ) /*|| (tiles[startRow +1][startCol+1] ==0 )*/){
        for(int row = 0; row < numOfTiles; row++){
            for(int col = 0; col < numOfTiles; col++){
                if(tiles[row][col] == label-1){
                    if(tiles[row-1][col] == 0) tiles[row-1][col] = label;
                    if(tiles[row+1][col] == 0) tiles[row+1][col] = label;
                    if(tiles[row][col-1] == 0) tiles[row][col-1] = label;
                    if(tiles[row][col+1] == 0) tiles[row][col+1] = label;
                }
            }
        }
        label++;
    }
    tiles[startRow][startCol] = label;
}

void TrajectoryPlan::makeWallTiles(){

    Point A,B;

    for(int idx = 0; idx < mapArea.wall.numofpoints; idx++){
        if(idx == 0){
            A = mapArea.wall.points[mapArea.wall.numofpoints -1].point;
        }
        else{
            A = mapArea.wall.points[idx-1].point;
        }
        B = mapArea.wall.points[idx].point;

        int minx = (int) std::min(A.x,B.x)/tileDim;
        int miny = (int)std::min(A.y,B.y+ VERTICAL_SHIFT)/tileDim;
        int maxx = (int)std::max(A.x,B.x)/tileDim;
        int maxy = (int)std::max(A.y,B.y+ VERTICAL_SHIFT)/tileDim;

        for(int i = minx; i <= maxx; i++){
            for(int j = miny; j <= maxy; j++){
                tiles[i][j] = 1;
            }
        }
    }


}

void TrajectoryPlan::generateTrajectory(queue<Position>& traj){
    //queue<Position> traj;
    int currRow = startRow, currCol = startCol;
    int updateR = 0, updateC = 1;
    double xp=0,yp=0;
    std::vector<std::vector<bool>> visited(numOfTiles, std::vector<bool>(numOfTiles, false));

    if((tiles[currRow][currCol-1] < tiles[currRow][currCol+1])&& (tiles[currRow][currCol-1]>1))
        updateC = -1;
    else if((tiles[currRow][currCol-1] > tiles[currRow][currCol+1])&& (tiles[currRow][currCol+1]>1))
        updateC = 1;

    while((currRow != targetRow) || (currCol != targetCol)){

        //if((tiles[currRow][currCol] <= tiles[currRow+updateR][currCol+updateC]) && (tiles[currRow+updateR][currCol+updateC] > 1)){
        if(tiles[currRow][currCol] -1 != tiles[currRow+updateR][currCol + updateC]){
            if(updateR == 0 && updateC != 0){
                updateC = 0;
                if((tiles[currRow - 1][currCol] < tiles[currRow + 1][currCol])&& (!visited[currRow - 1][currCol])){
                    updateR = -1;
                }
                else{
                    updateR = 1;
                }
            }
            else if(updateR != 0 && updateC == 0){
                updateR = 0;
                if((tiles[currRow][currCol-1] < tiles[currRow ][currCol+1])&&(!visited[currRow][currCol-1])){
                    updateC = -1;
                }
                else{
                    updateC = 1;
                }
            }
            xp = (currRow*tileDim + tileDim/2)/100;
            yp = (currCol*tileDim + tileDim/2)/100 - VERTICAL_SHIFT/100;
            traj.push(Position(xp,yp,0.0));

            std::cout<<"["<<xp<<","<<yp<<"]"<<std::endl;

        }
        visited[currRow][currCol] = true;
        currRow +=updateR;
        currCol += updateC;
    }
    //std::cout<<"While end"<<std::endl;
    xp = targetx/100;
    yp = targety/100;
    traj.push(Position(xp,yp,0.0));
    std::cout<<"["<<xp<<","<<yp<<"]"<<std::endl;
    //return traj;
}
