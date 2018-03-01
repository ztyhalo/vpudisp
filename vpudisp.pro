#-------------------------------------------------
#
# Project created by QtCreator 2018-03-01T13:17:30
#
#-------------------------------------------------

QT       -= core

QT       -= gui

TARGET = vpudisp
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /home/zty/Desktop/freecase/audioltib/rootfs/usr/include\
              /home/zty/Desktop/freecase/audioltib/rpm/BUILD/linux/include

LIBS += -L/home/zty/Desktop/freecase/audioltib/rootfs/usr/lib/ -lvpu -lipu -lrt

SOURCES += main.cpp \
    udphdmi/hdmiapp.cpp \
    udpsocket/udp.cpp \
    a9display/dec.c \
    a9display/display.c \
    a9display/fb.c \
    udphdmi/vpu_read.c \
    a9display/utils.c

HEADERS += \
    a9display/vpu_jpegtable.h \
    a9display/vpu_test.h \
    udphdmi/hdmiapp.h \
    udpsocket/udp.h \
    udphdmi/vpu_read.h
