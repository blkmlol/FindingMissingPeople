#ifndef PACKET_H
#define PACKET_H
#include<QtGlobal>
#include<QByteArray>
struct Packet
{
    static constexpr quint16 magic_start = 0x67;
    quint16 size;
    QByteArray bytes;
    static constexpr quint16 magic_end=0x68;
    void clearPacket();
};

#endif // PACKET_H
