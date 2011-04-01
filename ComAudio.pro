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

# icon information

macx {
    ICON = kidnapster.icns
}

win32 {
    RC_FILE = kidnapster.rc
}

SOURCES += main.cpp\
        mainwindow.cpp \
        thread.cpp \
        component.cpp \
        componentstream.cpp \
        componentvoice.cpp \
        componenttransfer.cpp \
        audioplayer.cpp \
        socket.cpp \
        server.cpp \
        microphone.cpp \
        parseHdr.cpp \
        client.cpp \
        serverstream.cpp

HEADERS  += mainwindow.h \
        socket.h \
        thread.h \
        component.h \
        packet.h \
        componentstream.h \
        componentvoice.h \
        componenttransfer.h \
        audioplayer.h \
        server.h \
        microphone.h \
        parseHdr.h \
        client.h \
        serverstream.h

FORMS    += mainwindow.ui \

RESOURCES += \
    icon.qrc

win32 {
    LIBS += -lws2_32
}

RESOURCES += icon.qrc \
             miscImages.qrc
