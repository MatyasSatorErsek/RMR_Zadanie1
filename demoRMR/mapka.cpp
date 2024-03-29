#include "mapka.h"
#include "ui_mapka.h"
#include <cstring> // For std::memcpy
#include <QPainter>


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

void Mapka::setMapData(int mapData[120][120])
{
    memcpy(this->m_mapData, mapData, sizeof(this->m_mapData));
    update(); // Trigger a repaint
}

void Mapka::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event); // Call base implementation
    QPainter painter(this);
    int cellSize = 5; // Define your cell size

    for (int i = 0; i < 120; i++) {
        for (int j = 0; j < 120; j++) {
            QRect cell(i * cellSize, j * cellSize, cellSize, cellSize);
            if (m_mapData[j][i] == 1) { // Notice the switched indices, adjust based on your data
                painter.fillRect(cell, Qt::black); // Fill cell
            } else {
                if (m_mapData[j][i] == 0) {
                    painter.fillRect(cell, Qt::white); // Draw empty cell
                } else {
                    painter.fillRect(cell, Qt::blue);
                }

            }
        }
    }
}
