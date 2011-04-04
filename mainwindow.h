#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include "thread.h"
#include "server.h"
#include "client.h"
#include "audioplayer.h"
#include "microphone.h"
#include "componentvoice.h"
#include "componentstream.h"
#include "serverstream.h"

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
    Ui::MainWindow *ui;   /**< Qt form UI. */
    Server *appServer_;   /**< For when the application is running as server. */
    Client *appClient_;   /**< For when the application is running as client. */
    AudioPlayer *player_; /**< Audioplayer for PCM audio. */
    Microphone *mic_;     /**< Recording device for user's voice through a mic. */
    Thread *micThread_;   /**< Thread the mic runs on */
    ComponentStream *stream_; /**< The stream object that will stream the music to this app */
    QMovie notes_;        /**< The audio animation */
    QMovie cylon_;        /**< Background animation */
    QMovie waiting_;      /**< Animation while transfering files */
    ServerStream* streamServer_; /**< The serverstream object for steaming audio. */
    Thread* streamThread_;/**< The thread that the serverstream runs in */
    QFile* receivedFile_; /**< The file that a file is stored in */

    void initDispatcher();
    /**
     * Call this function to enable or disable specific widgets. This depends on whether
     * the application is connected or not.
     *
     * @author Karl Castillo
     * @arg connected - true, if the application is connected
     *                  false, if the application is not connected
     */
    void clientConnect(bool connected);
    /**
     * Call this function to enable or disable specific widgets. This depends on whether
     * the application is connected or not.
     * @arg connected - true, if the application is connected
     *                  false, if the application is not connected
     */
    void serverConnect(bool connected);
    /**
     * Call this function to get the list of available music files in the server and show them
     * in the file list view.
     *
     * @author Karl Castillo
     */
    void getFileList();

public slots:
    void slotStopStream();
    /******************************************
     * VOICE SLOTS
     ******************************************/
    /**
     * Used to start a client voice session with the server
     *
     * signals the server to start running its voice component
     * then starts the voice component in its own thread
     * @author Warren Voelkl
     */
    void startVoice();
    /**
     * Used to stop a client voice session with the server
     *
     * sends a packet signaling disconnecting the voice component
     * @author Warren Voelkl
     */
    void stopVoice();

private slots:
    /**
     * Call this function to connect as the user-specified service (client or server).
     * This is where the settings parameters are collected and stored into the settings_
     * variable.
     *
     * @author Karl Castillo
     */
    void appConnectClient();
    /**
     * Call this function to disconnect the socket and to destroy the objects related to
     * the client or server.
     *
     * @author Karl Castillo
     */
    void appDisconnectClient();
    /**
     * Call this function to start the server.
     *
     * @author Karl Castillo
     */
    void appStartServer();
    /**
     * Call this function to close the server.
     *
     * @author Karl Castillo
     */
    void appStopServer();

    /******************************************
     * FILE SLOTS
     ******************************************/

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

    /******************************************
     * AUDIO PLAYER SLOTS
     ******************************************/

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

    /******************************************
     * SERVER SLOTS
     ******************************************/

    /**
     * Call this function to broadcast the song selected by the server master.
     * This function can only be used when the application is connected as a server.
     *
     * @author Karl Castillo
     */
    void broadcastSong();
    /**
     * Call this function to refresh the list of songs available on the server.
     *
     * @author Karl Castillo
     */
    void refreshSongList();


    void slotSendFileList(Socket* socket);

    void slotReceiveFileList(char* data, int length);

    void slotClientSongName(QString songname);

    void slotClientSongInfo(WaveHeader* header);

    void slotStartTransmit(QString filename);

    void slotReceiveTransmitData(char* data, int length);

    void slotFinishTransmit();

    void slotStartTransmitCurrent();

    void slotStartTransmitSelected();

signals:
    void playThisSong(QString);
    void signalStopVoiceComponent();
    void stopThisSong();
};

#endif // MAINWINDOW_H
