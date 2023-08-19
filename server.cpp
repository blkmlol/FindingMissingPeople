#include "server.h"

Server::Server(quint16 lis_port) : m_lis_port(lis_port) {
    if (listen(QHostAddress::Any, m_lis_port)) {
        std::cout << "Listening on port: " << m_lis_port << "\n";
    } else {
        std::cerr << "Error listening on port\n";
    }
}

void Server::sendToAllClients(const QString& str){
    m_data.clear();
    QDataStream out_data(&m_data, QIODevice::WriteOnly);
    out_data << Packet::magic_start << quint16(0) << str << Packet::magic_end;
    out_data.device()->seek(2);
    out_data << quint16(m_data.size() - sizeof(quint16) * 3);
    for (const auto& client : m_clients) {
        client->socket->write(m_data);
    }
    qInfo()<<"Send to client: "<<str;
}

void Server::incomingConnection(qintptr socket_descriptor) {
    auto client = std::make_unique<Pipe>(this);
    client->socket->setSocketDescriptor(socket_descriptor);
    connect(client.get(), &Pipe::readyRead, this, &Server::slotReadyRead);
    connect(client.get(), &Pipe::disconnected, this, &Server::slotDisconnect);
    m_clients.push_back(std::move(client));
}

void Server::slotReadyRead() {
    auto client = static_cast<Pipe*>(sender());
    QTcpSocket* socket = client->socket;
    SocketPosition& m_socket_position = client->pos;
    Packet& m_packet = client->packet;
    QDataStream input_data(socket);
    quint64 bytes = socket->bytesAvailable();
    quint64 counter = 0;
    if (input_data.status() == QDataStream::Ok) {
        while (!input_data.atEnd() && counter < bytes) {
            if (m_socket_position.size == m_socket_position.bytesRead) {
                if (socket->bytesAvailable() >= 2) {
                    quint16 value;
                    input_data >> value;
                    counter += sizeof(quint16);
                    if (value == m_packet.magic_start) {
                        if (socket->bytesAvailable() >= 2) {
                            quint16 size;
                            input_data >> size;
                            counter += sizeof(quint16);
                            m_packet.size = size;
                            m_socket_position.size = size;
                            while (socket->bytesAvailable() >= 1 && m_socket_position.bytesRead != m_socket_position.size) {
                                quint8 one_byte_data;
                                input_data >> one_byte_data;
                                counter += sizeof(quint8);
                                m_socket_position.bytesRead += sizeof(quint8);
                                m_packet.bytes.append(one_byte_data);
                            }
                            if (socket->bytesAvailable() >= 2) {
                                input_data >> value;
                                counter += sizeof(quint16);
                                if (value == m_packet.magic_end) {
                                    m_socket_position.size = 0;
                                    m_socket_position.bytesRead = 0;
                                    emit packetReady(m_packet);
                                    m_packet.clearPacket();
                                }
                            }
                        }
                    } else if (value == m_packet.magic_end) {
                        m_socket_position.size = 0;
                        m_socket_position.bytesRead = 0;
                        emit packetReady(m_packet);
                        m_packet.clearPacket();
                    } else {
                        while (socket->bytesAvailable() >= 1 && m_socket_position.bytesRead != m_socket_position.size) {
                            quint8 one_byte_data;
                            input_data >> one_byte_data;
                            counter += sizeof(quint8);
                            m_socket_position.bytesRead += sizeof(quint8);
                            m_packet.bytes.append(one_byte_data);
                        }
                        if (socket->bytesAvailable() >= 2)
                        {
                            input_data >> value;
                            counter += sizeof(quint16);
                            if (value == m_packet.magic_end)
                            {
                                m_socket_position.size = 0;
                                m_socket_position.bytesRead = 0;
                                emit packetReady(m_packet);
                                m_packet.clearPacket();
                            }
                        }
                    }
                }
            }
        }
        qInfo()<<"Message received. Ready to send";
    } else {
        std::cerr << "DataStream error\n";
    }
}

void Server::slotDisconnect() {
    auto client = static_cast<Pipe*>(sender());
    m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(), [&](const auto& c) { return c.get() == client; }), m_clients.end());
}
