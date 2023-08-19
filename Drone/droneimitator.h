#ifndef DRONEIMITATOR_H
#define DRONEIMITATOR_H

#include <QObject>
#include <vector>
#include <cmath>
#include "client.h"
#include "point.h"

class DroneImitator : public QObject
{
    Q_OBJECT

public:
    explicit DroneImitator(QObject *parent = nullptr);
    DroneImitator(QObject* parent, double x_start_cord, double y_start_cord);
    void SetTarget(const double& x, const double& y);

public slots:
    void StartMove();
    void StopMove();

private slots:
    void MoveDrone();

private:
    Client client;
    QTimer* timer;
    double x_drone_speed;
    double y_drone_speed;
    double x_coord;
    double y_coord;
    double target_x_coord;
    double target_y_coord;
    std::vector<Point> points;
    bool isInitialized = false;
    void InitCord();
};

#endif // DRONEIMITATOR_H
