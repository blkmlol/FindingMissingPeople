#include "packet.h"


void Packet::clearPacket()
{
    this->bytes.clear();
    this->size=0;
}
