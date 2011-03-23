#ifndef PACKET_H
#define PACKET_H

#define UDP                 0
#define TCP                 1
#define AUDIOCOMPONENT      252
#define FILECOMPONENT       253
#define TEXTCOMPONENT       254
#define VOICECOMPONENT      255
#define COMPONENTLIMIT      251

enum ConnectMode {
    kUDP,       /**< UDP transfer mode. */
    kTCP,       /**< TCP transfer mode. */
    kMulticast, /**< MultiCast Transfer.*/
};

struct Packet {
    int length; //equal to size of componetID + data;
    unsigned char componentID;
    char * data;
};

struct Message {
    ConnectMode type; //
    int socketID;
    char idAddr[16];
    Packet * payload;
};

#endif // PACKET_H
