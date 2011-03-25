#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "includes.h"

#include <QMainWindow>
#include "thread.h"
#include "logs.h"
#include "server.h"
#include "client.h"
#include "audioplayer.h"
#include "microphone.h"

struct Settings {
    Settings():ipAddr(""), alias(""), port(0), logChat(false), isClient(false){}
    QString ipAddr;
    QString alias;
    int port;
    bool logChat;
    bool isClient;
};

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
    Ui::MainWindow *ui;   /**< Qt form UI. */
    Settings* settings_;  /**< Current settings for the window. */
    Logs *chatLog_;       /**< Chat log. */
    Server *appServer_;   /**< For when the application is running as server. */
    Client *appClient_;   /**< For when the application is running as client. */
    AudioPlayer *player_; /**< Audioplayer for PCM audio. */
    Microphone *mic_;     /**< Recording device for user's voice through a mic. */
    Thread *micThread_;   /**< Thread the mic runs on */

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
