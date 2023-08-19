#ifndef PIPE_H
#define PIPE_H
#include <QObject>
#include<QTcpSocket>
#include<packet.h>
#include<socketposition.h>
class Pipe : public QObject
{
    Q_OBJECT
public:
    explicit Pipe(QObject *parent = nullptr);
    QTcpSocket*socket;
    Packet packet;
    SocketPosition pos;
signals:
    void readyRead();
    void disconnected();

};

#endif // PIPE_H
