//##############Network automatic face recognition and comparison##############//
//##############################NFRC###################################//


#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>
#include "mainwidget.h"
#include "detectionthread.h"

#include <iostream>
#include <stdio.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mainwidget w;

    QDesktopWidget *desktop = QApplication::desktop();
    w.move((desktop->width()-w.width())/2,(desktop->height()-w.height())/2);

    w.show();

    return a.exec();
}
