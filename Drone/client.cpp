#include "client.h"

Client::Client(QObject *parent, quint16 con_port)
    : QObject{parent},
      m_socket{new QTcpSocket{this}}
{
    connect(m_socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, &Client::onError);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(this, &Client::packetReady, this, &Client::onPacketReady);
    m_socket->connectToHost("127.0.0.1", con_port);
}

void Client::connectToServer()
{
    m_socket->connectToHost("127.0.0.1", m_socket->peerPort());
}

void Client::sendToServer(double latitude, double longitude, double altitude)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "Socket is not connected";
        return;
    }

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << Packet::magic_start << quint16(0) << latitude << longitude << altitude << Packet::magic_end;
    out.device()->seek(2);
    out << quint16(data.size() - sizeof(quint16) * 3);
    m_socket->write(data);
}

void Client::onConnected()
{
    qInfo() << "Connected to server";
}

void Client::onDisconnected()
{
    qInfo() << "Disconnected from server";
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << Packet::magic_start << quint16(0) << "connection_closed" << Packet::magic_end;
    out.device()->seek(2);
    out << quint16(data.size() - sizeof(quint16) * 3);
    m_socket->write(data);
}

void Client::onError(QAbstractSocket::SocketError error)
{
    qWarning() << "Socket error:" << error << "-" << m_socket->errorString();
}

void Client::onPacketReady(Packet packet)
{
    QDataStream in(packet.bytes);
    in >> message;
    qInfo() << "Received message:" << message;
    v_message.push_back(message);
}

void Client::onReadyRead()
{
    QDataStream in(m_socket);
    static int state = 0;
    static quint16 size = 0;
    static QByteArray buffer;

    while (m_socket->bytesAvailable() > 0) {
        switch (state) {
        case 0: // Start
            if (in.device()->bytesAvailable() < sizeof(quint16)) {
                return;
            }
            quint16 start;
            in >> start;
            if (start != Packet::magic_start) {
                qWarning() << "Invalid start magic:" << start;
                return;
            }
            state = 1;
            break;
        case 1: // Size
            if (in.device()->bytesAvailable() < sizeof(quint16)){
                return;
            }
            in >> size;
            buffer.clear();
            state = 2;
            break;
        case 2: // Data
            while (in.device()->bytesAvailable() > 0 && buffer.size() < size) {
                buffer.append(in.device()->read(1));
            }
            if (buffer.size() < size) {
                return;
            }
            state = 3;
            break;
        case 3: // End
            if (in.device()->bytesAvailable() < sizeof(quint16)) {
                return;
            }
            quint16 end;
            in >> end;
            if (end != Packet::magic_end) {
                qWarning() << "Invalid end magic:" << end;
                return;
            }
            state = 0;
            m_packet.bytes = buffer;
            emit packetReady(m_packet);
            break;
        }
    }
}
