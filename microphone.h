#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <QObject>
#include <QFile>
#include <QtMultimedia>

class Microphone:public QObject
{
    Q_OBJECT
public:
    /**
     * The microphone class constructor. This is where the audio format is populated.
     *
     * @author Karl Castillo
     */
    Microphone();
    /**
     * Call this function to start recording via microphone.
     *
     * @author Karl Castillo
     */
    void startRecording();
    /**
     * Call this function to stop the recording.
     *
     * @author Karl Castillo
     */
    void stopRecording();

private:
    /**
     * The buffer that will hold the captured raw data from the mic.
     */
    QBuffer *recordFile_;
    /**
     * The array that will hold the raw data that was read from input_.
     */
    QByteArray ba_;
    /**
     * A copy of the recordFile_. This will be where the reading would happen.
     */
    QIODevice *input_;
    /**
     * The input device that will capture the raw data.
     */
    QAudioInput *mic_;

public slots:

private slots:
    /**
     * Call this function to read data from the input_ variable and send it to the GUI
     * for transmission.
     *
     * @author Karl Castillo
     */
    void readData();

signals:
    /**
     * Emit this signal to send the read data to the gui for transmission.
     *
     * @author Karl Castillo
     * @arg mesg - the raw data read from the buffer
     */
    void sendVoice(const char *mesg);
};

#endif // MICROPHONE_H
