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
            // Green line from start to first black cell
            painter.setPen(QPen(Qt::green, 1));
            painter.drawLine(m_startPoint * cellSize + QPoint(cellSize / 2, cellSize / 2), intersectionPoint * cellSize + QPoint(cellSize / 2, cellSize / 2));

            // Red line from first black cell to goal
            painter.setPen(QPen(Qt::red, 1));
            painter.drawLine(intersectionPoint * cellSize + QPoint(cellSize / 2, cellSize / 2), m_goalPoint * cellSize + QPoint(cellSize / 2, cellSize / 2));
            hasObstacle = true;
        } else {
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
        return true;
    }
    if (count == 2) {
        if(m_mapData[m_J+1][m_I] == m_mapData[m_J-1][m_I] || m_mapData[m_J][m_I+1] == m_mapData[m_J][m_I-1]){
            return false;
        } else if(m_startPoint.x() < m_I && m_startPoint.y() > m_J && m_mapData[m_J][m_I-1] == 1 && m_mapData[m_J+1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() > m_I && m_startPoint.y() > m_J && m_mapData[m_J][m_I+1] == 1 && m_mapData[m_J+1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() < m_I && m_startPoint.y() < m_J && m_mapData[m_J][m_I-1] == 1 && m_mapData[m_J-1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() > m_I && m_startPoint.y() < m_J && m_mapData[m_J][m_I+1] == 1 && m_mapData[m_J-1][m_I] == 1) {
            return false;

        } else if(m_startPoint.x() > m_I && m_startPoint.y() < m_J && m_mapData[m_J][m_I-1] == 1 && m_mapData[m_J+1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() < m_I && m_startPoint.y() < m_J && m_mapData[m_J][m_I+1] == 1 && m_mapData[m_J+1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() > m_I && m_startPoint.y() > m_J && m_mapData[m_J][m_I-1] == 1 && m_mapData[m_J-1][m_I] == 1) {
            return false;
        } else if(m_startPoint.x() < m_I && m_startPoint.y() > m_J && m_mapData[m_J][m_I+1] == 1 && m_mapData[m_J-1][m_I] == 1) {
            return false;
        } else {
            return true;
        }
    }
    return false;
}

QPoint Mapka::nearestEdge() const {
    QPoint testPoint;
    QPoint interPoint;
    interPoint = getIntersectionPoint();
    float distance;
    float minDistance = 100000;

    for (int i = 0; i < 120; i++) {
        for (int j = 0; j < 120; j++) {
            if(m_edgeMapData[j][i] == 1) {
                distance = sqrt(pow((i-interPoint.x()), 2)+pow((j-interPoint.y()), 2));
                if (distance < minDistance){
                    minDistance = distance;
                    testPoint.setX(j);
                    testPoint.setY(i);
                }
            }
        }
    }


    return testPoint;
}
