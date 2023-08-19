#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include "packet.h"

class Client : public QObject
{
    Q_OBJECT

public slots:
    void onPacketReady(Packet packet);

public:
    explicit Client(QObject *parent = nullptr, quint16 con_port = 2445);
    void connectToServer();
    void sendToServer(double latitude, double longitude, double altitude);
    QString message;
    QVector<QString> v_message;

signals:
    void packetReady(Packet packet);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
    void onReadyRead();

private:
    QTcpSocket* m_socket;
    Packet m_packet;
};

#endif // CLIENT_H
