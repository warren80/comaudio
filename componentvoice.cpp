#include "componentvoice.h"

ComponentVoice::ComponentVoice() {
    socket_ = new Socket(kUDP);
    ap_ = new AudioPlayer(8000, 1, 8, 4096);
    socket_->serverJoinMCast(inet_addr(MULTICASTIP), htons(MULTICASTPORT));
}

ComponentVoice::~ComponentVoice() {
    delete socket_;
    QThread::wait();
}

void ComponentVoice::run() {
    while (1) {

    }
}
