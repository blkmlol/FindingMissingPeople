#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSslSocket>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isSetMarkerButtonOn(false)
    , row(0)
    , ctr(300)
    , np(0)
{

    ui->setupUi(this);
    qDebug() << QSslSocket::sslLibraryVersionString();
    const QUrl url(QStringLiteral("qrc:/map.qml"));
    ui->quickWidget->setSource(url);
    ui->quickWidget->show();

    obj = ui->quickWidget->rootObject();
    connect(this, SIGNAL(setMarkerButton(QVariant)), obj, SLOT(setMarkerButton(QVariant)));
    connect(this, SIGNAL(clearLastWaypoint()), obj, SLOT(clearLastWaypoint()));
    connect(this, SIGNAL(placeDrone()), obj, SLOT(placeDrone()));

    ui->tableView->setStyleSheet("QTableCornerButton::section {background-color: transparent;}");
    ui->tableView->setStyleSheet("background-color: transparent;");
    model = new QStandardItemModel;
    const QStringList horizontalHeader = {"ID", "Широта", "Долгота"};
    model->setHorizontalHeaderLabels(horizontalHeader);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->resizeSection(0, 60);
    ui->tableView->horizontalHeader()->resizeSection(1, 370);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    Point temp = {obj->property("startLatitude").toDouble(), obj->property("startLongitude").toDouble(), 100.0};
    pointData.push_back(temp);
    model->appendRow({new QStandardItem(QString::number(row + 1)),
                      new QStandardItem(QString::number(obj->property("startLatitude").toDouble())),
                      new QStandardItem(QString::number(obj->property("startLongitude").toDouble()))});
    row++;

    ui->navigationData->append("Широта: дрон не подключен\nДолгота: дрон не подключен\nБатарея: дрон не подключен");
    ui->photoData->append("Дрон не подключен\nПолученные изображения отсутствуют");
    ui->addPointButton->setEnabled(false);
    ui->removePointButton->setEnabled(false);
    connect(&server,&Server::packetReady,this,&MainWindow::make_point);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_setMarkerButton_clicked()
{
    QStandardItemModel *standardModel = qobject_cast<QStandardItemModel *>(model);
    if (standardModel)
    {
        QStandardItem *firstItem = standardModel->item(0,1);
        firstItem->setData(QString::number(obj->property("startLatitude").toDouble()), Qt::DisplayRole);
        firstItem = standardModel->item(0,2);
        firstItem->setData(QString::number(obj->property("startLongitude").toDouble()), Qt::DisplayRole);
    }
    if(isSetMarkerButtonOn)
    {
        ui->setMarkerButton->setText("Уст. маркер ВЫКЛ");
        isSetMarkerButtonOn = false;
        ui->addPointButton->setEnabled(false);
    }
    else {
        ui->setMarkerButton->setText("Уст. маркер ВКЛ");
        isSetMarkerButtonOn = true;
        ui->addPointButton->setEnabled(true);
    }
    emit setMarkerButton(isSetMarkerButtonOn);
}

void MainWindow::on_addPointButton_clicked()
{
    if (obj->property("isCreated")==true){
        Point temp = {obj->property("latitude").toDouble(), obj->property("longitude").toDouble(), 100.0};
        pointData.push_back(temp);
        model->appendRow({new QStandardItem(QString::number(row + 1)),
                          new QStandardItem(QString::number(obj->property("latitude").toDouble())),
                          new QStandardItem(QString::number(obj->property("longitude").toDouble()))});
        row++;
        ui->removePointButton->setEnabled(true);
        obj->setProperty("isCreated", false);
    }
}

void MainWindow::setDronePlacement(Point &droneLocation)
{
    obj->setProperty("droneLatitude", droneLocation.latitude);
    obj->setProperty("droneLongitude", droneLocation.longitude);
    emit placeDrone();
}

void MainWindow::make_point(Packet &packet)
{
    QDataStream input_data(packet.bytes);
    double pos1,pos2,pos3;
    input_data>>pos1>>pos2>>pos3;
    Point cur_point(pos1,pos2,pos3);
    setDronePlacement(cur_point);
    ui->navigationData->clear();
    ui->navigationData->append("Широта: "+QString::number(pos1)+
                               "\nДолгота: "+QString::number(pos2)+
                               "\nБатарея: "+QString::number(--ctr/3)+"%");
    if (ctr % 7 == 0)
    {
        ui->photoData->append("Фото #"+QString::number(++np)+
                              " Ш: "+QString::number(pos1)+
                              " Д: "+QString::number(pos2));
    }
    qInfo()<<ctr;
}

void MainWindow::on_removePointButton_clicked()
{
    if (row > 0){
        model->removeRows(--row,1);
        pointData.pop_back();
        if (row<2)
        {
            ui->removePointButton->setEnabled(false);
        }
        emit clearLastWaypoint();
    }
}

void MainWindow::on_connectDroneButton_clicked()
{
    setDronePlacement(droneLocation);
    pointData[0] = {obj->property("startLatitude").toDouble(), obj->property("startLongitude").toDouble(), 100.0};
    foreach(Point point, pointData)
    {
        QString coord = QString::number(point.latitude) + "***" + QString::number(point.longitude);
        server.sendToAllClients(coord);
    }
    ui->photoData->clear();
}

void MainWindow::on_aboutProjectButton_clicked()
{
    aboutProject.show();
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}
