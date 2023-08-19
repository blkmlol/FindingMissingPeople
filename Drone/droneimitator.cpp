#include "droneimitator.h"
#include "qtimer.h"
#include "constants.h"

DroneImitator::DroneImitator(QObject *parent)
    : QObject{parent},
      client{},
      timer{new QTimer{this}}
{
    client.connectToServer();
}

DroneImitator::DroneImitator(QObject *parent, double x_start_cord, double y_start_cord)
    : DroneImitator{parent}
{
    InitCord();
}

void DroneImitator::SetTarget(const double& x, const double& y)
{
    StopMove();
    disconnect(timer, &QTimer::timeout, this, &DroneImitator::MoveDrone);
    target_x_coord = x;
    target_y_coord = y;
    StartMove();
}

void DroneImitator::StartMove()
{
    connect(timer, &QTimer::timeout, this, &DroneImitator::MoveDrone);
    timer->start(constants::DRONE_UPDATE_TIME);
}

void DroneImitator::StopMove()
{
    timer->stop();
}

void DroneImitator::MoveDrone()
{
    if (client.v_message.empty()) {
        qInfo() << "Error: Waiting for points to move drone.";
        return;
    }
    if (!isInitialized) {
        isInitialized = true;
        InitCord();
    }
    const double dx = target_x_coord - x_coord;
    const double dy = target_y_coord - y_coord;
    const double distance = std::sqrt(dx * dx + dy * dy);
    if (distance <= constants::DRONE_TARGET_THRESHOLD) {
        if (!points.empty()) {
            const Point nextPoint = points.front();
            points.erase(points.begin());
            SetTarget(nextPoint.latitude, nextPoint.longitude);
            qInfo()<<"Next Point Changed. Lat: "<<nextPoint.latitude<<" Long: "<<nextPoint.longitude;
        } else {
            StopMove();
            qInfo()<<"Drone stopped moving";
        }
        return;
    }
    const double x_step = x_drone_speed * dx / distance;
    const double y_step = y_drone_speed * dy / distance;
    x_coord += x_step;
    y_coord += y_step;
    client.sendToServer(x_coord, y_coord, 0);
}

void DroneImitator::InitCord()
{
    for (const QString& str : client.v_message) {
        const QStringList parts = str.split("***");
        const Point temp = {parts[0].toDouble(), parts[1].toDouble(), 0};
        points.push_back(temp);
    }
    x_coord = points[0].latitude;
    y_coord = points[0].longitude;
    qInfo()<<"Init: Starting point -> Lat: "<<x_coord<<" Long: "<<y_coord;
    target_x_coord = points[1].latitude;
    target_y_coord = points[1].longitude;
    qInfo()<<"Init: Starting target point -> Lat: "<<target_x_coord<<" Long: "<<target_y_coord;
    points.erase(points.begin(), points.begin() + 2);
    x_drone_speed = constants::DRONE_DEFAULT_SPEED;
    y_drone_speed = constants::DRONE_DEFAULT_SPEED;
    qInfo()<<"Init: Speed: "<<x_drone_speed;
}
