#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <QObject>
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
      Destructor for the microphone.

      @author Nick Huber, Warren Voelkl
      */
    ~Microphone();

    /**
     * Call this function to stop the recording.
     *
     * @author Karl Castillo
     */
    void stopRecording();
public slots:
    /**
     * Call this function to start recording via microphone.
     *
     * @author Karl Castillo
     */
    void startRecording();

private:
    QBuffer *recordFile_; /** The buffer that will hold the captured raw data from the mic. */
    QByteArray *ba_; /**< The array that will hold the raw data that was read from input_. */
    QIODevice *input_; /** A copy of the recordFile_. This will be where the reading would happen. */
    QAudioInput *mic_; /**< * The input device that will capture the raw data. */
    qint64 dataWritten_; /**< The number of bytes written on to the QIODevice. */

private slots:
    /**
     * Call this function to read data from the input_ variable and send it to the GUI
     * for transmission.
     *
     * @author Karl Castillo
     */
    void readData();
    /**
     * Call this function when a data is written to input_.
     *
     * @author Karl Castillo
     */
    void bytesWritten(qint64 data);

signals:
    /**
     * Emit this signal to send the read data to the gui for transmission.
     *
     * @author Karl Castillo
     * @arg mesg - the raw data read from the buffer
     */
    void sendVoice(QByteArray *ba);
};

#endif // MICROPHONE_H
