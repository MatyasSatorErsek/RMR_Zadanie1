#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#ifdef _WIN32
#include<windows.h>
#endif
#include<iostream>
//#include<arpa/inet.h>
//#include<unistd.h>
//#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
//#include "ckobuki.h"
//#include "rplidar.h"
#include <cmath>
#include<queue>
#include<windows.h>
#include "robot.h"
#include <algorithm> // Include this header for std::min and std::max
#include <mutex>
#include <mapka.h>

#define TICK_TO_METER 0.000085292090497737556558
#define ANGLE_TOLERANCE PI*0.15
#define ENCODER_MAX_VALUE 65535

namespace Ui {
class MainWindow;
}

typedef struct lidarPoint{
    double posX;
    double posY;
}LidarPoint;

typedef struct obstacle{

    double rightAngle;
    double leftAngle;
    double rightDis;
    double leftDis;

}Obstacle;

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

///toto je trieda s oknom.. ktora sa spusti ked sa spusti aplikacia.. su tu vsetky gombiky a spustania...
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int processThisLidar(LaserMeasurement laserData);

    int processThisRobot(TKobukiData robotdata);

    queue<Position> referencePositions;


private slots:
    void on_pushButton_9_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();
    void getNewFrame();

    void on_pushButton_10_clicked();

    void getObstacles();

private:

    //--skuste tu nic nevymazat... pridavajte co chcete, ale pri odoberani by sa mohol stat nejaky drobny problem, co bude vyhadzovat chyby
    Ui::MainWindow *ui;
     void paintEvent(QPaintEvent *event);// Q_DECL_OVERRIDE;
     int updateLaserPicture;
     LaserMeasurement copyOfLaserData;
     std::string ipaddress;
     Robot robot;
     TKobukiData robotdata;
     int datacounter;
     QTimer *timer;

     double distance;
     double x;
     double y;
     double phi;

     bool movingLinear;

     int mapa[120][120] = {};
     double pointArrayX[1000] = {};
     double pointArrayY[1000] = {};

     int encRight;
     int encLeft;
     signed short gyro;

     bool posReached;
     bool orientationReached;

     void getOdometry(TKobukiData robotdata);

     void forwardSpeedCtr(Position refPos);

     void rotationSpeedCtr(Position refPos);

     double forwardspeed;//mm/s
     double rotationspeed;//omega/s

     Mapka *m;
public slots:
     void setUiValues(double robotX,double robotY,double robotFi);
signals:
     void uiValuesChanged(double newrobotX,double newrobotY,double newrobotFi); ///toto nema telo


};

#endif // MAINWINDOW_H
