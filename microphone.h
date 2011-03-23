#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <QObject>
#include <QFile>
#include <QtMultimedia>

class Microphone:public QObject
{
    Q_OBJECT
public:
    Microphone();
    ~Microphone();
    void startRecording();
    void stopRecording();

private:
    QFile *recordFile_;
    QAudioInput *mic_;

public slots:

private slots:
    void status();
    void state(QAudio::State state);
};

#endif // MICROPHONE_H
