#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "includes.h"

#include <QMainWindow>
#include "thread.h"
#include "logs.h"

typedef struct Settings {
    Settings():ipAddr(""), alias(""), port(0), isClient(false){}
    QString ipAddr;
    QString alias;
    int port;
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
    PSETTINGS settings_;
    Logs *chatLog_;

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
     * A wrapper function that should be called to print a message on to the chat screen.
     * This function has 2 versions:
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

signals:
};

#endif // MAINWINDOW_H
