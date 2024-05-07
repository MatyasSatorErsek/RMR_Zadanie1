#ifndef MAPKA_H
#define MAPKA_H

#include <QDialog>
#include <QPoint>

namespace Ui {
class Mapka;
}

class Mapka : public QDialog
{
    Q_OBJECT

public:
    explicit Mapka(QWidget *parent = nullptr);
    ~Mapka();

    void setMapData(int mapData[120][120], int edgeMapData[120][120]);
    void setLinePoints(const QPoint& start, const QPoint& goal);
    bool getHasObstacle() const;
    bool canDodgeObstacle() const;
    bool isPathClear(const QPoint& start, const QPoint& end) const;
    QPoint getIntersectionPoint() const;
    bool isEdge(int mapPointJ, int mapPointI) const;
    QPoint nearestEdge();
    bool getInsideRange() const;
    bool getHasIntersection() const;
    bool getAllClear() const;
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Mapka *ui;
    int m_mapData[120][120];
    int m_edgeMapData[120][120];
    QPoint m_startPoint;
    QPoint m_goalPoint;
    bool hasObstacle;
    bool dodgeObstacle;
    QPoint intersectionPoint;
    bool insideRange = false;
    bool hasIntersection = false;
    bool allClear = false;
};

#endif // MAPKA_H
