#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <math.h>


///TOTO JE DEMO PROGRAM...AK SI HO NASIEL NA PC V LABAKU NEPREPISUJ NIC,ALE SKOPIRUJ SI MA NIEKAM DO INEHO FOLDERA
/// AK HO MAS Z GITU A ROBIS NA LABAKOVOM PC, TAK SI HO VLOZ DO FOLDERA KTORY JE JASNE ODLISITELNY OD TVOJICH KOLEGOV
/// NASLEDNE V POLOZKE Projects SKONTROLUJ CI JE VYPNUTY shadow build...
/// POTOM MIESTO TYCHTO PAR RIADKOV NAPIS SVOJE MENO ALEBO NEJAKY INY LUKRATIVNY IDENTIFIKATOR
/// KED SA NAJBLIZSIE PUSTIS DO PRACE, SKONTROLUJ CI JE MIESTO TOHTO TEXTU TVOJ IDENTIFIKATOR
/// AZ POTOM ZACNI ROBIT... AK TO NESPRAVIS, POJDU BODY DOLE... A NIE JEDEN,ALEBO DVA ALE BUDES RAD
/// AK SA DOSTANES NA SKUSKU

int evalEnc(int oldDiff)
{
    int newDiff= oldDiff;
    int overflowVal = ENCODER_MAX_VALUE, overflowVal2 = ENCODER_MAX_VALUE/2;

    if(oldDiff > overflowVal2){
        newDiff = oldDiff - overflowVal;
    } else if (oldDiff < -overflowVal2) {
        newDiff = oldDiff + overflowVal;
    }

    return newDiff;
}


double tickToMeter(int encValPrev, int encValCur)
{
    double distance;
    int difference = encValCur - encValPrev;
    distance = (double) TICK_TO_METER * evalEnc(difference);
    return distance;
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //tu je napevno nastavena ip. treba zmenit na to co ste si zadali do text boxu alebo nejaku inu pevnu. co bude spravna
    ipaddress="127.0.0.1"; //192.168.1.11 127.0.0.1
    //ipaddress="192.168.1.11"; //192.168.1.11 127.0.0.1
  //  cap.open("http://192.168.1.11:8000/stream.mjpg");
    ui->setupUi(this);
    datacounter=0;
  //  timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(getNewFrame()));



    datacounter=0;

    x = 0;
    y = 0;
    phi = 0;
    distance = 0;

    posReached = false; orientationReached = false;

    pathClear = true;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    ///prekreslujem obrazovku len vtedy, ked viem ze mam nove data. paintevent sa
    /// moze pochopitelne zavolat aj z inych dovodov, napriklad zmena velkosti okna
    painter.setBrush(Qt::black);//cierna farba pozadia(pouziva sa ako fill pre napriklad funkciu drawRect)
    QPen pero;
    pero.setStyle(Qt::SolidLine);//styl pera - plna ciara
    pero.setWidth(3);//hrubka pera -3pixely
    pero.setColor(Qt::green);//farba je zelena
    QRect rect;
    rect= ui->frame->geometry();//ziskate porametre stvorca,do ktoreho chcete kreslit
    rect.translate(0,15);
    painter.drawRect(rect);

    if(updateLaserPicture==1) ///ak mam nove data z lidaru
    {
        updateLaserPicture=0;

        painter.setPen(pero);
        //teraz tu kreslime random udaje... vykreslite to co treba... t.j. data z lidaru
        //   std::cout<<copyOfLaserData.numberOfScans<<std::endl;
        for(int k=0;k<copyOfLaserData.numberOfScans/*360*/;k++)
        {
            int dist=copyOfLaserData.Data[k].scanDistance/20; ///vzdialenost nahodne predelena 20 aby to nejako vyzeralo v okne.. zmen podla uvazenia
            int xp=rect.width()-(rect.width()/2+dist*2*sin((360.0-copyOfLaserData.Data[k].scanAngle)*3.14159/180.0))+rect.topLeft().x(); //prepocet do obrazovky
            int yp=rect.height()-(rect.height()/2+dist*2*cos((360.0-copyOfLaserData.Data[k].scanAngle)*3.14159/180.0))+rect.topLeft().y();//prepocet do obrazovky
            if(rect.contains(xp,yp))//ak je bod vo vnutri nasho obdlznika tak iba vtedy budem chciet kreslit
                painter.drawEllipse(QPoint(xp, yp),2,2);
        }
    }

}


/// toto je slot. niekde v kode existuje signal, ktory je prepojeny. pouziva sa napriklad (v tomto pripade) ak chcete dostat data z jedneho vlakna (robot) do ineho (ui)
/// prepojenie signal slot je vo funkcii  on_pushButton_9_clicked
void  MainWindow::setUiValues(double robotX,double robotY,double robotFi)
{
     ui->lineEdit_2->setText(QString::number(x));
     ui->lineEdit_3->setText(QString::number(y));
     ui->lineEdit_4->setText(QString::number(phi));
}

///toto je calback na data z robota, ktory ste podhodili robotu vo funkcii on_pushButton_9_clicked
/// vola sa vzdy ked dojdu nove data z robota. nemusite nic riesit, proste sa to stane
int MainWindow::processThisRobot(TKobukiData robotdata)
{

///TU PISTE KOD... TOTO JE TO MIESTO KED NEVIETE KDE ZACAT,TAK JE TO NAOZAJ TU. AK AJ TAK NEVIETE, SPYTAJTE SA CVICIACEHO MA TU NATO STRING KTORY DA DO HLADANIA XXX

    ///tu mozete robit s datami z robota
    /// ale nic vypoctovo narocne - to iste vlakno ktore cita data z robota
    ///teraz tu posielam rychlosti na zaklade toho co setne joystick a vypisujeme data z robota(kazdy 5ty krat. ale mozete skusit aj castejsie). vyratajte si polohu. a vypiste spravnu
    /// tuto cast mozete vklude vymazat,alebo znasilnit na vas regulator alebo ake mate pohnutky
    /*if(forwardspeed==0 && rotationspeed!=0)
        robot.setRotationSpeed(rotationspeed);
    else if(forwardspeed!=0 && rotationspeed==0)
        robot.setTranslationSpeed(forwardspeed);
    else if((forwardspeed!=0 && rotationspeed!=0))
        robot.setArcSpeed(forwardspeed,forwardspeed/rotationspeed);
    else
        robot.setTranslationSpeed(0);*/

    getOdometry(robotdata);

    bool newPosCalculated = false;
    Position* newPosition;
    Position currRefPos(0,0,0);
    if(!referencePositions.empty())
    {
        currRefPos=referencePositions.front();
        if(!orientationReached)
        {
            rotationSpeedCtr(currRefPos);
            robot.setRotationSpeed(rotationspeed);
        }
        else
        {
            forwardSpeedCtr(currRefPos);
            double alfa = -phi +atan2(currRefPos.y - y, currRefPos.x- x);
            alfa=alfa>PI ? alfa-(2*PI): alfa< -PI ? alfa+2*PI:alfa;
            if(abs(alfa)>ANGLE_TOLERANCE)
            {
                orientationReached = false;
            }
            if(pathClear)
                robot.setTranslationSpeed(forwardspeed);
            else{
                robot.setTranslationSpeed(0);
            }


        }
        if(orientationReached && posReached)
        {
            referencePositions.erase(referencePositions.begin());
            orientationReached = false;
            posReached = false;
        }

        /*if(!pathClear){
            if(!newPosCalculated){
                *newPosition =calculateShorterPath(currRefPos,rightEdgeAngle,leftEdgeAngle,rightEdgeDis,leftEdgeDis);
                newPosCalculated = true;
                referencePositions.insert(referencePositions.begin(),*newPosition);
            }
        }
        else if(pathClear && newPosCalculated){
            newPosCalculated = false;
        }*/

    }



    if(datacounter%5==0)
    {

        ///ak nastavite hodnoty priamo do prvkov okna,ako je to na tychto zakomentovanych riadkoch tak sa moze stat ze vam program padne
                // ui->lineEdit_2->setText(QString::number(robotdata.EncoderRight));
                //ui->lineEdit_3->setText(QString::number(robotdata.EncoderLeft));
                //ui->lineEdit_4->setText(QString::number(robotdata.GyroAngle));
                /// lepsi pristup je nastavit len nejaku premennu, a poslat signal oknu na prekreslenie
                /// okno pocuva vo svojom slote a vasu premennu nastavi tak ako chcete. prikaz emit to presne takto spravi
                /// viac o signal slotoch tu: https://doc.qt.io/qt-5/signalsandslots.html
        ///posielame sem nezmysli.. pohrajte sa nech sem idu zmysluplne veci
        emit uiValuesChanged(robotdata.EncoderLeft,11,12);
        ///toto neodporucam na nejake komplikovane struktury.signal slot robi kopiu dat. radsej vtedy posielajte
        /// prazdny signal a slot bude vykreslovat strukturu (vtedy ju musite mat samozrejme ako member premmennu v mainwindow.ak u niekoho najdem globalnu premennu,tak bude cistit bludisko zubnou kefkou.. kefku dodam)
        /// vtedy ale odporucam pouzit mutex, aby sa vam nestalo ze budete pocas vypisovania prepisovat niekde inde

    }
    datacounter++;

    return 0;

}

///toto je calback na data z lidaru, ktory ste podhodili robotu vo funkcii on_pushButton_9_clicked
/// vola sa ked dojdu nove data z lidaru
int MainWindow::processThisLidar(LaserMeasurement laserData)
{


    memcpy( &copyOfLaserData,&laserData,sizeof(LaserMeasurement));
    //tu mozete robit s datami z lidaru.. napriklad najst prekazky, zapisat do mapy. naplanovat ako sa prekazke vyhnut.
    // ale nic vypoctovo narocne - to iste vlakno ktore cita data z lidaru
    updateLaserPicture=1;
    update();//tento prikaz prinuti prekreslit obrazovku.. zavola sa paintEvent funkcia
    pathClear = isPathClear(laserData);
    if(!pathClear){
        if(findRightEdge(laserData,&rightEdgeAngle,&rightEdgeDis))
            std::cout<<"Right edge Angle: "<< rightEdgeAngle*(180.0/PI)<<std::endl;
        if(findLeftEdge(laserData,&leftEdgeAngle,&leftEdgeDis))
            std::cout<<"Left edge Angle: "<< leftEdgeAngle*(180.0/PI)<<std::endl;
    }
    else
        std::cout<<"Obstacle not detected"<<std::endl;
    //std::cout<<laserData.Data[0].scanAngle<<std::endl;


    return 0;

}


void MainWindow::on_pushButton_9_clicked() //start button
{

    forwardspeed=0;
    rotationspeed=0;
    //tu sa nastartuju vlakna ktore citaju data z lidaru a robota
    connect(this,SIGNAL(uiValuesChanged(double,double,double)),this,SLOT(setUiValues(double,double,double)));

    ///setovanie veci na komunikaciu s robotom/lidarom/kamerou.. su tam adresa porty a callback.. laser ma ze sa da dat callback aj ako lambda.
    /// lambdy su super, setria miesto a ak su rozumnej dlzky,tak aj prehladnost... ak ste o nich nic nepoculi poradte sa s vasim doktorom alebo lekarnikom...
    robot.setLaserParameters(ipaddress,52999,5299,/*[](LaserMeasurement dat)->int{std::cout<<"som z lambdy callback"<<std::endl;return 0;}*/std::bind(&MainWindow::processThisLidar,this,std::placeholders::_1));
    robot.setRobotParameters(ipaddress,53000,5300,std::bind(&MainWindow::processThisRobot,this,std::placeholders::_1));

    ///ked je vsetko nasetovane tak to tento prikaz spusti (ak nieco nieje setnute,tak to normalne nenastavi.cize ak napr nechcete kameru,vklude vsetky info o nej vymazte)
    robot.robotStart();


}

void MainWindow::on_pushButton_2_clicked() //forward
{
    //pohyb dopredu
    robot.setTranslationSpeed(500);

}

void MainWindow::on_pushButton_3_clicked() //back
{
    robot.setTranslationSpeed(-250);

}

void MainWindow::on_pushButton_6_clicked() //left
{
robot.setRotationSpeed(3.14159/2);

}

void MainWindow::on_pushButton_5_clicked()//right
{
robot.setRotationSpeed(-3.14159/2);

}

void MainWindow::on_pushButton_4_clicked() //stop
{
    robot.setTranslationSpeed(0);

}




void MainWindow::on_pushButton_clicked()
{


    ui->pushButton->setText("use laser");

}

void MainWindow::getNewFrame()
{

}

void MainWindow::getOdometry(TKobukiData robotData)
{
    double leftWheel,rightWheel,lrk,deltaa;
    short newEncLeft,newEncRight;

    newEncLeft = (int) robotData.EncoderLeft;
    newEncRight = (int) robotData.EncoderRight;


    if(datacounter == 0)
    {
        x = 0;
        y = 0;
        phi = 0;
        distance = 0;
    }
    else
    {
        leftWheel = tickToMeter(encLeft,newEncLeft);
        rightWheel = tickToMeter(encRight,newEncRight);
        lrk = (leftWheel + rightWheel)/2;

        deltaa  = (rightWheel - leftWheel)/0.23;

        //deltaa = robotData.GyroAngle * (3.1415926/180) + gyro;

        phi += deltaa;
        distance += lrk;

        x = x + lrk*cos(phi);
        y  = y + lrk * sin(phi);

        //x = distance * cos(phi);
        //y = distance * sin(phi);

    }

    encLeft = newEncLeft;
    encRight = newEncRight;
    gyro = robotData.GyroAngle;
  }


  void MainWindow::forwardSpeedCtr(Position refPos) {
      double distErr;
      double Kp = 200;
      double maxForwardSpeed = 300; // Set maximum forward speed
      double rampRate = 8; // Set ramp rate

      distErr = sqrt(pow(refPos.x - x, 2) + pow(refPos.y - y, 2));

      if (distErr < 0.05) {
          // If the error is small, stop forward motion
          forwardspeed = 0;
          posReached = true;
      } else {
          // Calculate the desired forward speed with proportional control
          double desiredForwardSpeed = Kp * distErr;

          // Apply ramping to the desired forward speed
          if ((desiredForwardSpeed - forwardspeed) > rampRate) {
              if (desiredForwardSpeed > forwardspeed)
                  forwardspeed += rampRate; // Increase forward speed gradually
              else
                  forwardspeed -= rampRate; // Decrease forward speed gradually
          } else {
              forwardspeed = desiredForwardSpeed; // Apply the desired forward speed
          }

          // Limit the forward speed to the maximum value
          forwardspeed = min(forwardspeed, maxForwardSpeed);
      }
  }

  void MainWindow::rotationSpeedCtr(Position refPos) {
      double rotErr;
      double Kp = 1.3;
      double maxRotationSpeed = 2.2; // Set maximum rotation speed
      double rampRate = 0.2; // Set ramp rate

      rotErr = -phi + atan2(refPos.y - y, refPos.x - x);
      rotErr=rotErr>PI ? rotErr-(2*PI): rotErr< -PI ? rotErr+2*PI:rotErr;

      if (abs(rotErr) < PI / 90) {
          // If the error is small, stop rotation
          rotationspeed = 0;
          orientationReached = true;
      } else {
          // Calculate the desired rotation speed with proportional control
          double desiredRotationSpeed = Kp * rotErr;

          // Apply ramping to the desired rotation speed
          if ((desiredRotationSpeed - rotationspeed) > rampRate) {
              if (desiredRotationSpeed > rotationspeed)
                  rotationspeed += rampRate; // Increase rotation speed gradually
              else
                  rotationspeed -= rampRate; // Decrease rotation speed gradually
          } else {
              rotationspeed = desiredRotationSpeed; // Apply the desired rotation speed
          }

          // Limit the rotation speed to the maximum value
          rotationspeed = min(rotationspeed, maxRotationSpeed);
          rotationspeed = max(rotationspeed, -maxRotationSpeed);
      }
  }


void MainWindow::on_pushButton_10_clicked()
{
    double x_ref = ui->lineEdit_5->text().toDouble();
    double y_ref = ui->lineEdit_6->text().toDouble();

    referencePositions.insert(referencePositions.begin(),Position(x_ref,y_ref,0.0));
}

bool MainWindow::isPathClear(LaserMeasurement laserData)
{
    double b,angle,dis;
    int i = 0;
    angle = laserData.Data[i].scanAngle * (PI/180.0);
    dis = laserData.Data[i].scanDistance/1000;
    b = dis*cos(angle);
    while(angle <= SAFE_ANGLE || angle >= (2*PI - SAFE_ANGLE)){
        if(b < CRITICAL_DISTANCE)
            return false;

        i++;
        angle = laserData.Data[i].scanAngle * (PI/180.0);
        dis = laserData.Data[i].scanDistance/1000;
        b = dis*cos(angle);
    }

    return true;
}

bool MainWindow::findRightEdge(LaserMeasurement laserData,double* re,double* red){

    double angle,dis;
    int idx = 0;
    angle = laserData.Data[idx].scanAngle*(PI/180.0);
    dis = laserData.Data[idx].scanDistance/1000;
    double rightEdge,rightEdgeDis;

    while(angle < 75.0*(PI/180.0)){
        if(dis > CRITICAL_DISTANCE+SAFE_ZONE){
            rightEdge = angle;
            rightEdgeDis = dis;
            break;
        }
        idx++;
        angle = laserData.Data[idx].scanAngle*(PI/180.0);
        dis = laserData.Data[idx].scanDistance/1000;
    }
    if(angle >= 75.0*(PI/180.0))
        return false;

    while(angle-rightEdge < SAFE_ANGLE){
        if(dis < CRITICAL_DISTANCE+SAFE_ZONE){
            return false;
        }
        idx++;
        angle = laserData.Data[idx].scanAngle*(PI/180.0);
        dis = laserData.Data[idx].scanDistance/1000;
    }
    *re = 2*PI - rightEdge;
    *red = rightEdgeDis;
    return true;

}

bool MainWindow::findLeftEdge(LaserMeasurement laserData,double* le,double *led){
    double angle,dis;
    int idx = laserData.numberOfScans;
    angle = laserData.Data[idx].scanAngle*(PI/180.0);
    dis = laserData.Data[idx].scanDistance/1000;
    double leftEdge,leftEdgeDis;

    while(angle > (360.0-75.0)*(PI/180.0)){
        if(dis > CRITICAL_DISTANCE+SAFE_ZONE){
            leftEdge = angle;
            leftEdgeDis = dis;
            break;
        }
        idx--;
        angle = laserData.Data[idx].scanAngle*(PI/180.0);
        dis = laserData.Data[idx].scanDistance/1000;
    }
    if(angle <= (360.0-75.0)*(PI/180.0))
        return false;

    while(angle-leftEdge > SAFE_ANGLE){
        if(dis < CRITICAL_DISTANCE+SAFE_ZONE){
            return false;
        }
        idx--;
        angle = laserData.Data[idx].scanAngle*(PI/180.0);
        dis = laserData.Data[idx].scanDistance/1000;
    }
    *le = 2*PI - leftEdge;
    *led = leftEdgeDis;
    return true;
}

Position MainWindow::calculateShorterPath(Position goal, double rea, double lea, double red, double led){
    double leftRelx,leftRely;
    double rightRelx,rightRely;

    rea += SAFE_ANGLE/2;
    lea += SAFE_ANGLE/2;

    leftRelx = led * cos(lea);
    leftRely = led * sin(lea);
    rightRelx = red * cos(rea);
    rightRely = red * sin(rea);

    double leftGlobx,leftGloby;
    double rightGlobx, rightGloby;

    leftGlobx = x + leftRelx*cos(phi) - leftRely*sin(phi);
    leftGloby = y + leftRelx*sin(phi) + leftRely*cos(phi);
    rightGlobx = x + rightRelx*cos(phi) - rightRely*sin(phi);
    rightGloby = y + rightRelx*sin(phi) + rightRely*cos(phi);

    double lr,ll;

    ll = sqrt(pow(x - leftGlobx,2) + pow(y - leftGloby,2))+ sqrt(pow(goal.x - leftGlobx,2) + pow(goal.y - leftGloby,2));
    lr = sqrt(pow(x - rightGlobx,2) + pow(y - rightGloby,2))+ sqrt(pow(goal.x - rightGlobx,2) + pow(goal.y - rightGloby,2));
    if(ll > lr){
        return Position(leftGlobx,leftGloby,0.0);
    }
    else{
        return Position(rightGlobx,rightGloby,0.0);
    }

}
