#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QQuickItem>
#include <QVariant>
#include <aboutproject.h>
#include <point.h>
#include <server.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QStandardItem *item;
    QStandardItemModel *model;
    QQuickItem* obj;
    Server server;
    AboutProject aboutProject;
    QVector<Point> pointData;
    Point droneLocation;
    QList<QStandardItem*> rowItems;
    bool isSetMarkerButtonOn;
    int row;
    int col;
    int ctr;
    int np;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setDronePlacement(Point &droneLocation);

private slots:
    void make_point(Packet&packet);
    void on_exitButton_clicked();

    void on_addPointButton_clicked();

    void on_removePointButton_clicked();

    void on_setMarkerButton_clicked();

    void on_connectDroneButton_clicked();

    void on_aboutProjectButton_clicked();

signals:
    void setMarkerButton(QVariant);
    void clearLastWaypoint();
    void placeDrone();
};
#endif // MAINWINDOW_H
