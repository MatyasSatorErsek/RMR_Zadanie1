#include "map_loader.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"



map_loader::map_loader()
{
}

 void map_loader::load_map(char *filename,TMapArea &mapss)
 {


     FILE *fp=fopen(filename,"r");
     if (fp==NULL)
     {

         printf("zly file\n");
         return ;
     }
     else
     {
         printf("vsetko dobre\n");
     }

     //tu nacitame obvodovu stenu
     char myLine[550];
     fgets(myLine,550,fp);
     printf("%s\n",myLine);
     char *myCopy=(char*)calloc(strlen(myLine)+2,sizeof(char));
     memcpy(myCopy,myLine,sizeof(char)*strlen(myLine));
     char *freeMyCopy;
     freeMyCopy=myCopy;
     myCopy=strtok(myCopy,"[]");
     mapss.wall.numofpoints=(atoi(myCopy));
      printf("num of points %i\n",mapss.wall.numofpoints);
      mapss.wall.points.reserve(mapss.wall.numofpoints);
      for(int i=0;i<mapss.wall.numofpoints;i++)
      {
          TMapPoint temp;
          myCopy=strtok(NULL,"[,");
          temp.point.x=atof(myCopy);
          myCopy=strtok(NULL,"[,");
          temp.point.y=atof(myCopy);
          mapss.wall.points.push_back(temp);
       //   mapss.wall.points[i/2].suradnice[i%2]=atof(myCopy);

      }
      free(freeMyCopy);

      //tu nacitame jednotlive prekazky
       mapss.numofObjects=0;
        mapss.obstacle.clear();
      while( fgets(myLine,550,fp))
      {
          printf("%s\n",myLine);
          myCopy=(char*)calloc(strlen(myLine)+2,sizeof(char));
          memcpy(myCopy,myLine,sizeof(char)*strlen(myLine));

          freeMyCopy=myCopy;
          myCopy=strtok(myCopy,"[]");
          if((atoi(myCopy))==0)
              break;
          TMapObject tempObstacle;
          mapss.numofObjects++;

          tempObstacle.numofpoints=(atoi(myCopy));
          for(int i=0;i< tempObstacle.numofpoints;i++)
           {
              TMapPoint temp;
              myCopy=strtok(NULL,"[,");
              temp.point.x=atof(myCopy);
              myCopy=strtok(NULL,"[,");
              temp.point.y=atof(myCopy);
              tempObstacle.points.push_back(temp);

           }
           free(freeMyCopy);
           mapss.obstacle.push_back(tempObstacle);



      }


 }


 void map_loader::load_map_new(const QString& filename, TMapArea& mapss) {
     QFile file(filename);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         qDebug() << "Error opening file: " << file.errorString();
         return;
     }

     QTextStream in(&file);

     // Read the line containing the number of points for the wall
     QString line = in.readLine();
     qDebug() << line;

     QStringList parts = line.split("[]");
     mapss.wall.numofpoints = parts.isEmpty() ? 0 : parts.first().toInt();
     qDebug() << "num of points" << mapss.wall.numofpoints;

     // Reserve space for wall points
     mapss.wall.points.reserve(mapss.wall.numofpoints);

     // Read wall points
     for (int i = 0; i < mapss.wall.numofpoints; ++i) {
         line = in.readLine();
         parts = line.split("[,");
         if (parts.size() >= 2) {
             TMapPoint temp;
             temp.point.x = parts[0].toDouble();
             temp.point.y = parts[1].toDouble();
             mapss.wall.points.push_back(temp);
         }
     }

     // Read obstacles
     mapss.numofObjects = 0;
     mapss.obstacle.clear();
     while (!in.atEnd()) {
         line = in.readLine();
         qDebug() << line;

         parts = line.split("[]");
         if (parts.isEmpty() || parts.first().toInt() == 0)
             break;

         TMapObject tempObstacle;
         mapss.numofObjects++;

         tempObstacle.numofpoints = parts.first().toInt();
         for (int i = 0; i < tempObstacle.numofpoints; ++i) {
             line = in.readLine();
             parts = line.split("[,");
             if (parts.size() >= 2) {
                 TMapPoint temp;
                 temp.point.x = parts[0].toDouble();
                 temp.point.y = parts[1].toDouble();
                 tempObstacle.points.push_back(temp);
             }
         }
         mapss.obstacle.push_back(tempObstacle);
     }

     file.close();

 }
