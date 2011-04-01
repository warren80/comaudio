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
<<<<<<< HEAD
        componentchat.cpp \
components.cpp \
=======
>>>>>>> d3e9433491195babbe069d83eb110f8752745eda
        audioplayer.cpp \
        socket.cpp \
        server.cpp \
        microphone.cpp \
<<<<<<< HEAD
        parseHdr.cpp \
        client.cpp \
=======
        client.cpp
>>>>>>> d3e9433491195babbe069d83eb110f8752745eda

HEADERS  += mainwindow.h \
        socket.h \
        thread.h \
        component.h \
        packet.h \
        componentstream.h \
        componentvoice.h \
        componenttransfer.h \
<<<<<<< HEAD
        componentchat.h \
components.h \
=======
>>>>>>> d3e9433491195babbe069d83eb110f8752745eda
        audioplayer.h \
        server.h \
        microphone.h \
<<<<<<< HEAD
        parseHdr.h \
        client.h \
=======
        client.h

>>>>>>> d3e9433491195babbe069d83eb110f8752745eda

FORMS    += mainwindow.ui \

win32 {
    LIBS += -lws2_32
}

RESOURCES += \
    icon.qrc \
    miscImages.qrc

OTHER_FILES +=
