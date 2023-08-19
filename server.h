#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QByteArray>
#include <QDataStream>
#include <iostream>
#include <QString>
#include <memory>
#include <pipe.h>
#include <packet.h>
class Server: public QTcpServer
{
     Q_OBJECT
public:
    explicit Server(quint16 lis_port=2445);
    void sendToAllClients(const QString& str);
private:
    quint16 m_lis_port;
    std::vector<std::unique_ptr<Pipe>> m_clients;
    QByteArray m_data;
public slots:
    void incomingConnection(qintptr socket_descriptor) override;
    void slotReadyRead();
    void slotDisconnect();
signals:
    void packetReady(Packet&packet);
};

#endif // SERVER_H
