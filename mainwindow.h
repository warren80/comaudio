#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "includes.h"

#include <QMainWindow>
#include "thread.h"
#include "logs.h"
#include "server.h"
#include "audioplayer.h"
#include "microphone.h"

typedef struct Settings {
    Settings():ipAddr(""), alias(""), port(0), logChat(false), isClient(false){}
    QString ipAddr;
    QString alias;
    int port;
    bool logChat;
    bool isClient;
}SETTINGS, *PSETTINGS;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Thread * dispatcherThread_;
    Ui::MainWindow *ui;
    /**
     * Structure that will hold all the settings parameters.
     */
    PSETTINGS settings_;
    /**
     * A log object where the chat log will be saved.
     */
    Logs *chatLog_;
    /**
     * The server object. This object is only created if the user wants
     * the app to be a server.
     */
    Server *appServer_;
    /**
     * The audio player that will play the .wav files.
     */
    AudioPlayer *player_;
    /**
     * The microphone object that will recordthe user's voice.
     */
    Microphone *mic_;
    /**
     * The thread that will hold the microphone process.
     */
    Thread *micThread_;

    void initDispatcher();
    /**
     * Call this function to enable or disable specific widgets. This depends on whether
     * the application is connected or not.
     *
     * @author Karl Castillo
     * @arg connected - true, if the application is connected
     *                  false, if the application is not connected
     */
    void connected(bool connected);
    /**
     * Call this function to get the list of available music files in the server and show them
     * in the file list view.
     *
     * @author Karl Castillo
     */
    void getFileList();
    /**
     * A wrapper function that should be called to print a message on to the chat screen.
     * This method has 2 versions:
     *  - printF(const char *message);
     *  - printF(const QString message);
     *
     * @author Karl Castillo
     * @arg message - the message that is to be printed out
     */
    void printF(const char *message);
    void printF(const QString message);

public slots:

private slots:
    /**
     * Call this function to connect as the user-specified service (client or server).
     * This is where the settings parameters are collected and stored into the settings_
     * variable.
     *
     * @author Karl Castillo
     */
    void appConnect();
    /**
     * Call this function to disconnect the socket and to destroy the objects related to
     * the client or server.
     *
     * @author Karl Castillo
     */
    void appDisconnect();
    /**
     * Call this function to browse for a file that will be sent to the server.
     *
     * @author Karl Castillo
     */
    void browseFile();
    /**
     * Call this function to send the selected file to the server.
     *
     * @author Karl Castillo
     */
    void sendFile();
    /**
     * Call this function to send the text message from the chat widget to the server.
     *
     * @author Karl Castillo
     */
    void sendText();
    /**
     * Call this function to download a selected song.
     *
     * @author Karl Castillo
     */
    void downloadSong();
    /**
     * Call this function to download the current song that is playing.
     *
     * @author Karl Castillo
     */
    void downloadCurrentSong();
    /**
     * Call this function to refresh the server files list.
     *
     * @author Karl Castillo
     */
    void refreshFiles();
    /**
     * Call this function to play the previous song that was being played.
     *
     * @author Karl Castillo
     */
    void previousSong();
    /**
     * Call this function to play the current song.
     *
     * @author Karl Castillo
     */
    void playSong();
    /**
     * Call this function to pause the current song.
     *
     * @author Karl Castillo
     */
    void pauseSong();
    /**
     * Call this function to play the next song.
     *
     * @author Karl Castillo
     */
    void nextSong();
    /**
     * Call this function to send the voice recordings to the server.
     *
     * @author Karl Castillo
     * @arg mesg - the voice chat message that will be sent.
     */
    void sendVoice(const char *mesg);

signals:
};

#endif // MAINWINDOW_H
