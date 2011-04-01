#-------------------------------------------------
#
# Project created by QtCreator 2011-02-25T14:00:14
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = ComAudio
TEMPLATE = app

# compilation output directories

DESTDIR = ./bin
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
UI_DIR = ./ui

SOURCES += main.cpp\
        mainwindow.cpp \
        thread.cpp \
        component.cpp \
        dispatcher.cpp \
        componentaudio.cpp \
        componentvoice.cpp \
        componentfile.cpp \
        audioplayer.cpp \
        socket.cpp \
        server.cpp \
        componentiterator.cpp \
        microphone.cpp \
    client.cpp

HEADERS  += mainwindow.h \
        socket.h \
        includes.h \
        thread.h \
        component.h \
        packet.h \
        dispatcher.h \
        componentaudio.h \
        componentvoice.h \
        componentfile.h \
        audioplayer.h \
        server.h \
        componentiterator.h \
        microphone.h \
    client.h

FORMS    += mainwindow.ui \

win32 {
    LIBS += -lwsock32
}
