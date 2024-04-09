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

        if((wallPointA.x >= topLeft.x && wallPointA.x <= bottomRight.x) && (wallPointA.y >= topLeft.y && wallPointA.y <= bottomRight.y))
            return true;
        if((wallPointB.x >= topLeft.x && wallPointB.x <= bottomRight.x) && (wallPointB.y >= topLeft.y && wallPointB.y <= bottomRight.y))
            return true;


        //If point A is higher than point B
        if(wallPointB.x < wallPointA.x){
            Point t = wallPointB;
            wallPointB = wallPointA;
            wallPointA = t;
        }

        // Check for potential division by zero (vertical line)
        if (abs(wallPointB.x - wallPointA.x) < 1e-6) {  // Tolerance for floating-point precision
            // Line is vertical, check if x-coordinate is within rectangle bounds
            if (wallPointA.x >= topLeft.x && wallPointA.x <= bottomRight.x) {
                return true;  // Line intersects if x is within bounds
            }
            //return false;
        }

        double slope = (wallPointB.y - wallPointA.y)/(wallPointB.x - wallPointA.x);
        double inter = wallPointA.y - slope*wallPointA.x;

        // Top Edge
        if (slope != 0 && inter <= topLeft.y && inter >= topRight.y) {
            double x = (topLeft.y - inter) / slope;
            if (x >= topLeft.x && x <= topRight.x) {
                return true;
            }
        }

        // Right edge
        if (inter <= topRight.y && inter >= bottomRight.y) {
            double y = slope * bottomRight.x + inter;
            if (y >= bottomRight.y && y <= topRight.y) {
                return true;
            }
        }

        // Bottom edge
        if (slope != 0 && inter <= bottomLeft.y && inter >= bottomRight.y) {
            double x = (bottomLeft.y - inter) / slope;
            if (x >= bottomLeft.x && x <= bottomRight.x) {
                return true;
            }
        }

        // Left edge
        if (inter <= topLeft.y && inter >= bottomLeft.y) {
            double y = slope * topLeft.x + inter;
            if (y >= bottomLeft.y && y <= topLeft.y) {
                return true;
            }
        }

        /*bool A1  = ((wallPointB.x > topRight.x) && (wallPointB.y > topRight.y)) && ((wallPointA.x < bottomLeft.x) && (wallPointA.y < bottomLeft.y));
        bool A2  = ((wallPointB.x < topLeft.x) && (wallPointB.y > topLeft.y)) && ((wallPointA.x > bottomRight.x) && (wallPointA.y < bottomRight.y));


        if(A1 || A2)
            return true;*/

    }

    return false;
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
    startCol = (int) y/tileDim;

    if(tiles[startRow][startCol] == 0){
        tiles[startRow][startCol] = START;
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
    targetCol = (int) y/tileDim;

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
    while((tiles[startRow -1 ][startCol -1] ==0 ) || (tiles[startRow +1][startCol+1] ==0 )){
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
        int miny = (int)std::min(A.y,B.y)/tileDim;
        int maxx = (int)std::max(A.x,B.x)/tileDim;
        int maxy = (int)std::max(A.y,B.y)/tileDim;

        for(int i = minx; i <= maxx; i++){
            for(int j = miny; j <= maxy; j++){
                tiles[i][j] = 1;
            }
        }
    }


}

queue<Position> TrajectoryPlan::generateTrajectory(){
    queue<Position> traj;
    int currRow = startRow;
    int currCol = startCol;
    int prevRow = currRow, prevCol = currCol;

    std::vector<std::vector<bool>> visitedTiles(numOfTiles, std::vector<bool>(numOfTiles, false));
    for(int i = 0; i< numOfTiles; i++){
        for(int j = 0; j< numOfTiles; j++){
            visitedTiles[i][j] = false;
        }
    }

    bool vertical = true;
    while((currRow != targetRow) && (currCol != targetCol)){
        if(vertical){
            if((tiles[currRow][currCol+1] < tiles[currRow][currCol]) && !visitedTiles[currRow][currCol+1] && (tiles[currRow][currCol+1] > 1)){
                ++currCol;
            }
            else if((tiles[currRow][currCol-1] < tiles[currRow][currCol]) && !visitedTiles[currRow][currCol-1]&& (tiles[currRow][currCol-1] > 1)){
                --currCol;
            }
            else{
                vertical = true;
                double xp = (currRow*tileDim + tileDim/2)/100;
                double yp = (currCol*tileDim + tileDim/2)/100;
                traj.push(Position(xp,yp,0));
            }
        }
        else{
            if((tiles[currRow+1][currCol] < tiles[currRow][currCol]) && !visitedTiles[currRow+1][currCol]&& (tiles[currRow+1][currCol] > 1)){
                ++currRow;
            }
            else if((tiles[currRow-1][currCol] < tiles[currRow][currCol]) && !visitedTiles[currRow-1][currCol]&& (tiles[currRow-1][currCol] > 1)){
                --currRow;
            }
            else{
                vertical = false;
                double xp = (currRow*tileDim + tileDim/2)/100;
                double yp = (currCol*tileDim + tileDim/2)/100;
                traj.push(Position(xp,yp,0));
            }
        }
        visitedTiles[currRow][currCol] = true;
    }

    double xp = (targetRow*tileDim + tileDim/2)/100;
    double yp = (targetCol*tileDim + tileDim/2)/100;
    traj.push(Position(xp,yp,0));
    return traj;
}
