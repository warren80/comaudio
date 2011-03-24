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
    QBuffer *echoFile_;
    QByteArray ba_;
    QIODevice *input_;
    QIODevice *output_;
    QAudioInput *mic_;
    QAudioOutput *echo_;
    QTimer *recordTimer_;
    QAudioFormat *format_;

public slots:

private slots:
    void status();
    void stateInput(QAudio::State state);
    void readData();
    void dataWritten(qint64 x);

signals:
    void sendVoice(const char *mesg);
};

#endif // MICROPHONE_H
