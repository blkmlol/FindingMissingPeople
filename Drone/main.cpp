#include <QCoreApplication>
#include<droneimitator.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DroneImitator m;
    m.StartMove();
    return a.exec();
}
