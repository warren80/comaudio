#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "includes.h"

#include <QMainWindow>
#include "settings.h"
#include "thread.h"

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
    PSETTINGS settings;


    void initDispatcher();

public slots:

private slots:

signals:
};

#endif // MAINWINDOW_H
