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
    void print(const char *);
    ~MainWindow();

private:
    Ui::MainWindow *ui;   /**< Qt form UI. */
    Server *appServer_;   /**< For when the application is running as server. */
    Client *appClient_;   /**< For when the application is running as client. */
    Microphone *mic_;     /**< Recording device for user's voice through a mic. */
    Thread *micThread_;   /**< Thread the mic runs on */
    ComponentStream *stream_; /**< The stream object that will stream the music to this app */
    QMovie notes_;        /**< The audio animation */
    QMovie cylon_;        /**< Background animation */
    QMovie waiting_;      /**< Animation while transfering files */
    ServerStream* streamServer_; /**< The serverstream object for steaming audio. */
    Thread* streamThread_;/**< The thread that the serverstream runs in */
    QFile* receivedFile_; /**< The file that a file is stored in */
    AudioPlayer* localPlayer_; /**< Local playback of audio files. */

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
    /**
     * Signal this slot to print out a message to the debug form.
     *
     * @author Karl Casitllo
     * @arg debug - the message to be printed out
     */
    void printF(QString str);

    /**
     * Stops the streaming audio
     * by calling reset on the audiostream component
     */
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
     * AUDIO PLAYER SLOTS
     ******************************************/
    /**
     * Call this function to play a local song.
     *
     * @author Karl Castillo
     * @author Nick Huber
     */
    void playSong();
    /**
     * Call this function to stop the song being played.
     *
     * @author Karl Castillo
     * @author Nick Huber
     */
    void stopSong();
    /**
     * Call this function to refresh the local list of songs.
     *
     * @author Karl Castillo
     */
    void refreshLocalList();

    /******************************************
     * FILE SLOTS
     ******************************************/
    /**
     * Call this function to get an updated list of all the server files.
     *
     * @author Karl Castillo
     */
    void refreshServerFilesList();

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

    /**
     * Transmits a list of files to the client from the server for file transfer.
     *
     * takes all the files from the song directory and puts them all in a string.
     * then transmits that string over the socket.
     * @author Nick Huber
     */
    void slotSendFileList(Socket* socket);

    /**
     * Recieves a list of files from the server for selecting a file to download.
     *
     * parses a string from the socket to be displayed in the GUI.
     * @author Nick Huber
     */
    void slotReceiveFileList(char* data, int length);
    /** Sets the text of the current song streaming from the server. */
    void slotClientSongName(QString songname);
    /** Sets the song info of the current song streaming from the server. */
    void slotClientSongInfo(WaveHeader* header);
    /** initiates the transmission of a file to the client */
    void slotStartTransmit(QString filename);
    /** writes the recieved data to a file. */
    void slotReceiveTransmitData(char* data, int length);
    /** Closes the file on compleation of a file transmission*/
    void slotFinishTransmit();
    /** Sets the text for the transmitted file */
    void slotStartTransmitCurrent();
    /** shows the selected file transmited in the gui.*/
    void slotStartTransmitSelected();
     /** sends the filename of the current song being played.*/
    void slotSendFileName(Socket* socket);

signals:
    void playThisSong(QString);
    void signalStopVoiceComponent();
    void stopThisSong();
    void signalStartComponentVoice();
    void signalPrintF(QString);

};

#endif // MAINWINDOW_H
