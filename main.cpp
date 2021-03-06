#include <QtGui/QApplication>

#ifdef _WIN32
#include <winsock2.h>
#endif

#include "mainwindow.h"

int main(int argc, char *argv[])
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0) {
        return 1;
    }
#endif

    QApplication a(argc, argv);
    MainWindow *mainWindow_ = new MainWindow();
    mainWindow_->show();

    int retCode = a.exec();

#ifdef _WIN32
    WSACleanup();
#endif

    return retCode;
}
