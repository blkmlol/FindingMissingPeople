#include "pipe.h"

Pipe::Pipe(QObject *parent) : QObject(parent)
{
    this->socket=new QTcpSocket(this);
    connect(socket,&QTcpSocket::readyRead,this,&Pipe::readyRead);
    connect(socket,&QTcpSocket::disconnected,this,&Pipe::disconnected);
}
