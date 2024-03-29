#ifndef MAPKA_H
#define MAPKA_H

#include <QDialog>

namespace Ui {
class Mapka;
}

class Mapka : public QDialog
{
    Q_OBJECT

public:
    explicit Mapka(QWidget *parent = nullptr);
    ~Mapka();

    void setMapData(int mapData[120][120]);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Mapka *ui;
    int m_mapData[120][120];
};

#endif // MAPKA_H
