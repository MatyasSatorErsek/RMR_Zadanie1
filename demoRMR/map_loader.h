#ifndef MAP_LOADER_H
#define MAP_LOADER_H
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QDebug>


typedef struct point_struct
{

    double x;
    double y;

}Point;

typedef union{
    Point point;
    double suradnice[2];
}TMapPoint;

typedef struct
{
    int numofpoints;
    std::vector<TMapPoint> points;
}TMapObject;

typedef struct
{
   TMapObject wall;
   int numofObjects;
   std::vector<TMapObject> obstacle;
}TMapArea;


class map_loader
{
public:
    map_loader();

    void load_map_new(const QString& filename, TMapArea& mapss);

};

void load_map(char *filename,TMapArea &mapss);

#endif // MAP_LOADER_H
