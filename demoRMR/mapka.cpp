#include "mapka.h"
#include "ui_mapka.h"
#include "globals.h"
#include <cstring> // For std::memcpy
#include <QPainter>
#include <iostream>
#include <cmath>


Mapka::Mapka(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Mapka)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    memset(m_mapData, 0, sizeof(m_mapData));
}

Mapka::~Mapka()
{
    delete ui;
}

void Mapka::setMapData(int mapData[120][120], int edgeMapData[120][120])
{
    memcpy(this->m_mapData, mapData, sizeof(this->m_mapData));
    update(); // Trigger a repaint
}

void Mapka::setLinePoints(const QPoint& start, const QPoint& goal) {
    m_startPoint = start;
    m_goalPoint = goal;
    update();  // Request a repaint
}

void Mapka::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event); // Call base implementation
    QPainter painter(this);
    int cellSize = 5; // Define your cell size
    QPoint idk;

    for (int i = 0; i < 120; i++) {
        for (int j = 0; j < 120; j++) {
            QRect cell(i * cellSize, j * cellSize, cellSize, cellSize);
            if (m_mapData[j][i] == 1) { // Notice the switched indices, adjust based on your data
                if (isEdge(j, i)){
                    painter.fillRect(cell, Qt::red); // Fill cell
                    m_edgeMapData[j][i] = 1;
                }
                else {
                    painter.fillRect(cell, Qt::black); // Fill cell
                    m_edgeMapData[j][i] = 0;
                }
            } else {
                if (m_mapData[j][i] == 0) {
                    painter.fillRect(cell, Qt::white); // Draw empty cell
                    m_edgeMapData[j][i] = 0;
                } else {
                    painter.fillRect(cell, Qt::blue);
                    m_edgeMapData[j][i] = 0;
                }

            }

        }
    }

    idk = nearestEdge();
    QRect cell(idk.y() * cellSize, idk.x() * cellSize, cellSize, cellSize);
    painter.fillRect(cell, Qt::yellow);

    // Draw the line with color change on crossing a black cell
    if (!m_startPoint.isNull() && !m_goalPoint.isNull()) {
        QPoint currentPoint = m_startPoint;
        QPoint endPoint = m_goalPoint;
        bool foundBlackCell = false;



        int robotArmLength = 1; // The length of each arm from the center

        // Define outer start points of the robot cross
        QPoint leftOuter = m_startPoint + QPoint(-robotArmLength, 0);
        QPoint rightOuter = m_startPoint + QPoint(robotArmLength, 0);
        QPoint topOuter = m_startPoint + QPoint(0, -robotArmLength);
        QPoint bottomOuter = m_startPoint + QPoint(0, robotArmLength);

        // Define shifted end points
        QPoint leftGoal = m_goalPoint + QPoint(-robotArmLength, 0);
        QPoint rightGoal = m_goalPoint + QPoint(robotArmLength, 0);
        QPoint topGoal = m_goalPoint + QPoint(0, -robotArmLength);
        QPoint bottomGoal = m_goalPoint + QPoint(0, robotArmLength);

        // Array of outer start and corresponding shifted end points
        QPoint outerStarts[] = {leftOuter, rightOuter, topOuter, bottomOuter};
        QPoint outerEnds[] = {leftGoal, rightGoal, topGoal, bottomGoal};

        allClear = true;
        // Loop through each pair of start and end points to draw lines and check for intersections
        for (int i = 0; i < 4; i++) {
            QPoint currentPoint = outerStarts[i];
            QPoint endPoint = outerEnds[i];
            bool foundBlackCell = false;
            QPoint intersectionPoint;

            // Bresenham's algorithm reused with adjusted endpoints
            int dx = abs(endPoint.x() - currentPoint.x()), sx = currentPoint.x() < endPoint.x() ? 1 : -1;
            int dy = -abs(endPoint.y() - currentPoint.y()), sy = currentPoint.y() < endPoint.y() ? 1 : -1;
            int err = dx + dy, e2;

            while (true) {
                if (m_mapData[currentPoint.y()][currentPoint.x()] == 1) {
                    allClear = false;
                    foundBlackCell = true;
                    intersectionPoint = currentPoint;
                    break;
                }
                if (currentPoint == endPoint) break;
                e2 = 2 * err;
                if (e2 >= dy) { err += dy; currentPoint.setX(currentPoint.x() + sx); }
                if (e2 <= dx) { err += dx; currentPoint.setY(currentPoint.y() + sy); }
            }

            // Drawing the line segments
            painter.setPen(QPen(foundBlackCell ? Qt::red : Qt::green, 1));
            painter.drawLine(outerStarts[i] * cellSize + QPoint(cellSize / 2, cellSize / 2),
                             (foundBlackCell ? intersectionPoint : endPoint) * cellSize + QPoint(cellSize / 2, cellSize / 2));
        }

        // Bresenham's line algorithm to find first black cell intersection
        int dx = abs(endPoint.x() - currentPoint.x()), sx = currentPoint.x() < endPoint.x() ? 1 : -1;
        int dy = -abs(endPoint.y() - currentPoint.y()), sy = currentPoint.y() < endPoint.y() ? 1 : -1;
        int err = dx + dy, e2;

        while (true) { // the loop
            if (m_mapData[currentPoint.y()][currentPoint.x()] == 1) {
                foundBlackCell = true;
                intersectionPoint = currentPoint;
                break;
            }
            if (currentPoint == endPoint) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; currentPoint.setX(currentPoint.x() + sx); }
            if (e2 <= dx) { err += dx; currentPoint.setY(currentPoint.y() + sy); }
        }

        // Draw line segments
        if (foundBlackCell) {
            hasIntersection = true;
            // Green line from start to first black cell
            painter.setPen(QPen(Qt::green, 1));
            painter.drawLine(m_startPoint * cellSize + QPoint(cellSize / 2, cellSize / 2), intersectionPoint * cellSize + QPoint(cellSize / 2, cellSize / 2));

            // Red line from first black cell to goal
            painter.setPen(QPen(Qt::red, 1));
            painter.drawLine(intersectionPoint * cellSize + QPoint(cellSize / 2, cellSize / 2), m_goalPoint * cellSize + QPoint(cellSize / 2, cellSize / 2));
            hasObstacle = true;
        } else {
            hasIntersection = false;
            // No black cells intersected, draw entire line green
            painter.setPen(QPen(Qt::green, 1));
            painter.drawLine(m_startPoint * cellSize + QPoint(cellSize / 2, cellSize / 2), m_goalPoint * cellSize + QPoint(cellSize / 2, cellSize / 2));
            hasObstacle = false;
        }
    }   
}

bool Mapka::getHasObstacle() const {
    return hasObstacle;
}

QPoint Mapka::getIntersectionPoint() const {
    return intersectionPoint;
}

bool Mapka::isEdge(int m_J, int m_I) const {
    int count = 0;
    //no. neighbours
    if (m_mapData[m_J+1][m_I] == 1) {
        count++;
    }
    if (m_mapData[m_J-1][m_I] == 1) {
        count++;
    }
    if (m_mapData[m_J][m_I+1] == 1) {
        count++;
    }
    if (m_mapData[m_J][m_I-1] == 1) {
        count++;
    }
    //single point
    if (count == 1) {
        // if (sqrt(pow(m_startPoint.x() -m_I, 2) +pow(m_startPoint.y() -m_J, 2)) > 20.0) {
        //     return false;
        // }
        return true;
    }
    if (count == 2) {
        // if (sqrt(pow(m_startPoint.x() -m_I, 2) +pow(m_startPoint.y() -m_J, 2)) > 20.0) {
        //     return false;
        // }
        if(m_mapData[m_J+1][m_I] == m_mapData[m_J-1][m_I] || m_mapData[m_J][m_I+1] == m_mapData[m_J][m_I-1]){
            return false;
        } else if(m_startPoint.x() <= m_I && m_startPoint.y() >= m_J && m_mapData[m_J][m_I-1] == 1 && m_mapData[m_J+1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() >= m_I && m_startPoint.y() >= m_J && m_mapData[m_J][m_I+1] == 1 && m_mapData[m_J+1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() <= m_I && m_startPoint.y() <= m_J && m_mapData[m_J][m_I-1] == 1 && m_mapData[m_J-1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() >= m_I && m_startPoint.y() <= m_J && m_mapData[m_J][m_I+1] == 1 && m_mapData[m_J-1][m_I] == 1) {
            return false;

        } else if(m_startPoint.x() >= m_I && m_startPoint.y() <= m_J && m_mapData[m_J][m_I-1] == 1 && m_mapData[m_J+1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() <= m_I && m_startPoint.y() <= m_J && m_mapData[m_J][m_I+1] == 1 && m_mapData[m_J+1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() >= m_I && m_startPoint.y() >= m_J && m_mapData[m_J][m_I-1] == 1 && m_mapData[m_J-1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() <= m_I && m_startPoint.y() >= m_J && m_mapData[m_J][m_I+1] == 1 && m_mapData[m_J-1][m_I] == 1) {
            return false;
        } else {
            return true;
        }
    }
    return false;
}

QPoint Mapka::nearestEdge(){
    QPoint testPoint;
    QPoint interPoint;
    interPoint = getIntersectionPoint();
    float distance;
    float minDistance = 100000;

    // for (int i = 0; i < 120; i++) {
    //     for (int j = 0; j < 120; j++) {
    //         if(m_edgeMapData[j][i] == 1) {
    //             distance = sqrt(pow((i-interPoint.x()), 2)+pow((j-interPoint.y()), 2));
    //             if (distance < minDistance){
    //                 minDistance = distance;
    //                 testPoint.setX(j);
    //                 testPoint.setY(i);
    //             }
    //         }
    //     }
    // }
    for (int i = 0; i < 120; i++) {
        for (int j = 0; j < 120; j++) {
            if(m_edgeMapData[j][i] == 1) {
                distance = sqrt(pow((i-m_startPoint.x()), 2)+pow((j-m_startPoint.y()), 2)) + sqrt(pow(i-m_goalPoint.x(), 2)+pow(j-m_goalPoint.y(), 2));
                if (distance < minDistance){
                    minDistance = distance;
                    testPoint.setX(j);
                    testPoint.setY(i);
                }
            }
        }
    }

    // std::cout<<(-(testPoint.x()-60.0) /10.0) + 0.05<<std::endl;
    // std::cout<<(-(59.0 - testPoint.y()) /10.0) + 0.05<<std::endl;
    if (sqrt(pow(m_startPoint.y() -testPoint.x(), 2) +pow(m_startPoint.x() -testPoint.y(), 2)) > 5.0) {
        std::cout<<"mimo range"<<std::endl;
        insideRange = false;
    } else {
        std::cout<<"vnutri"<<std::endl;
        insideRange = true;
    }

    std::cout<<m_startPoint.x()<<std::endl;
    std::cout<<m_startPoint.y()<<std::endl;
    std::cout<<testPoint.y()<<std::endl;
    std::cout<<testPoint.y()<<std::endl;
    std::cout<<sqrt(pow(m_startPoint.y() -testPoint.x(), 2) +pow(m_startPoint.x() -testPoint.y(), 2))<<std::endl;
    return testPoint;
}

bool Mapka::getInsideRange() const{
    return insideRange;
}

bool Mapka::getHasIntersection() const{
    return hasIntersection;
}

bool Mapka::getAllClear() const{
    return allClear;
}
