#include <QCoreApplication>
#include <QApplication>
#include "mainwidget.h"
#include "detectionthread.h"

#include <iostream>
#include <stdio.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    detectionThread *detectionthread = new detectionThread;
//    detectionthread->start();

    mainwidget w;
    w.show();

    return a.exec();
}
