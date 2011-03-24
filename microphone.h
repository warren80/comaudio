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
    QBuffer *recordFile_;
    QAudioInput *mic_;
    QAudioOutput *echo_;
    QTimer *recordTimer_;
    QAudioFormat *format_;

public slots:

private slots:
    void status();
    void stateInput(QAudio::State state);
    void stateOutput(QAudio::State state);

signals:
    void sendVoice(const char *mesg);
};

#endif // MICROPHONE_H
