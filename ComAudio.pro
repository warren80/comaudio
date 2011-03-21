#-------------------------------------------------
#
# Project created by QtCreator 2011-02-25T14:00:14
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = ComAudio
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        socket.cpp \
        settings.cpp \
        thread.cpp \
        component.cpp \
        dispatcher.cpp \
        componentaudio.cpp \
        componenttext.cpp \
        componentvoice.cpp \
        componentfile.cpp \
        audioplayer.cpp

HEADERS  += mainwindow.h \
        socket.h \
        settings.h \
        includes.h \
        thread.h \
        component.h \
        packet.h \
        dispatcher.h \
        componentaudio.h \
        componenttext.h \
        componentvoice.h \
        componentfile.h \
        audioplayer.h

FORMS    += mainwindow.ui \
         settings.ui

win32 {
    LIBS += -lwsock32
}
