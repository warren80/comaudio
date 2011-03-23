#ifndef PACKET_H
#define PACKET_H

#define BUFSIZE             1024

#define UDP                 0x00
#define TCP                 0x01
#define AUDIOCOMPONENT      0x02
#define FILECOMPONENT       0x04
#define TEXTCOMPONENT       0x08
#define VOICECOMPONENT      0x10

enum ConnectMode {
    kUDP,       /**< UDP transfer mode. */
    kTCP,       /**< TCP transfer mode. */
    kMulticast, /**< MultiCast Transfer.*/
};

/**
* PacketType is 8 bits
* bit[0] 0 = udp, 1 = tcp ie packetType & 0x01
* bit[1] audio
* bit[2] file
* bit[3] text
* bit[4] voice
*/
struct Packet {
    int length; //equal to size of componetID + data;
    unsigned char componentID;
    char * data;
};

struct Message {
    ConnectMode type; //
    int socketID;
    Packet * payload;
};

#endif // PACKET_H
