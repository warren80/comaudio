#ifndef PACKET_H
#define PACKET_H

#define BUFSIZE             1024

#define UDP                 0x00
#define TCP                 0x01
#define AUDIOCOMPONENT      0x02
#define FILECOMPONENT       0x04
#define TEXTCOMPONENT       0x08
#define VOICECOMPONENT      0x10

/**
* PacketType is 8 bits
* bit[0] 0 = udp, 1 = tcp ie packetType & 0x01
* bit[1] audio
* bit[2] file
* bit[3] text
* bit[4] voice
*/
struct Packet {
    unsigned char packetType;
    unsigned char data [BUFSIZE - 1];
};


#endif // PACKET_H
