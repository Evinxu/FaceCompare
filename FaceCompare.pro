QT += core\
      network
QT -= gui

QT += widgets

CONFIG += c++11

TARGET = FaceCompare
#CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DESTDIR = bin
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += main.cpp \
    mainwidget.cpp \
    detectionthread.cpp \
    eyedetection.cpp

FORMS += \
    mainwidget.ui

HEADERS += \
    mainwidget.h \
    detectionthread.h \
    eyedetection.h

INCLUDEPATH+=D:\Opencv\install\include\opencv2\
D:\Opencv\opencv\build\include\
D:\Opencv\opencv_mingw\install\include


LIBS+=D:\Opencv\lib\libopencv_*\
      D:\Opencv\opencv_mingw\install\x86\mingw\bin\libopencv_*\
      D:\Opencv\opencv_mingw\install\x86\mingw\lib\libopencv_*.a

#DISTFILES +=

RESOURCES += \
    rotate.qrc
