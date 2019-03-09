#ifndef DETECTIONTHREAD_H
#define DETECTIONTHREAD_H

#include <QThread>
#include <QDebug>
#include <QCoreApplication>
#include <QString>
#include <QTimerEvent>
#include <QBasicTimer>
#include <QTimer>
#include "mainwidget.h"

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>

using namespace cv;
using namespace std;
using namespace ml;

class detectionThread : public QThread
{
    Q_OBJECT
public:
    explicit detectionThread(QObject *parent = 0);
    void run();

    void detectAndDisplay(Mat frame);

    QString iup;

signals:
    void detectionchange(QString);

private slots:
   void ReceiveIupSlot(QString iupstr);


};

#endif // DETECTIONTHREAD_H
