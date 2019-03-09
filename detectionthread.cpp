#include "detectionthread.h"
#include <QDebug>
#include <QTime>
#include <QApplication>
#include <QString>
#include <QMessageBox>
#include <iostream>
#include <stdio.h>
#include <QString>
#include <string>
//#include <string.h>

#define CV_COLOR_RED cv::Scalar(0, 0, 255)

String face_cascade_name = "D:\\Opencv\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
String window_name = "Capture - Face detection";

Mat imageROI;

QString IUP_GO;

detectionThread::detectionThread(QObject *parent)
{}

//接收IUP（Ip&Username&Passwd）
void detectionThread::ReceiveIupSlot(QString iupstr)
{
    iup = iupstr;

    qDebug()<<"Received signals is:"<<iup;

    IUP_GO = iup;
}

//打开局域网摄像头
void detectionThread::run()
{
    qDebug()<<"detectionThread is:"<<QThread::currentThread();

    VideoCapture capture;
    Mat frame;

    string VSA = IUP_GO.toStdString();

    std::cout<<"VSA IS:"<<VSA<<std::endl;   //不能用QDEBUG打印出

    frame = capture.open(VSA);

    //    frame = capture.open("http://admin:admin@192.168.0.103:8081");
    //加载cascades
    if(!face_cascade.load(face_cascade_name))
    {
        qDebug("--(!)Error loading face cascade\n");
        //        return -1;
    }
    //打开视频
    //    capture.open( -1 );
    if (! capture.isOpened())
    {
        qDebug("--(!)Error opening video capture\n");
        //        return -1;
    }

    while (capture.read(frame))   //读取帧
    {
        if(frame.empty())
        {
            qDebug(" --(!) No captured frame -- Break!");
            break;
        }

        double scale = 0.3;
        Size dsize = Size(frame.cols*scale, frame.rows*scale);
        Mat img2 = Mat(dsize,CV_32S);
        cv::resize(frame,img2,dsize);

        detectAndDisplay(img2);

        char c = (char)waitKey(33);
        if( c == 27 )
        {
            break;
        }
    }

    capture.release();
}

void detectionThread::detectAndDisplay(Mat frame)
{
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor(frame,frame_gray,COLOR_BGR2GRAY );

    equalizeHist(frame_gray,frame_gray);

    //-- Detect faces
    face_cascade.detectMultiScale(frame_gray,faces,1.1,2,0|CASCADE_SCALE_IMAGE, Size(15,15));

    for (size_t i = 0;i < faces.size();i++ )
    {
        //椭圆
        //Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        //ellipse( frame, center, Size(faces[i].width/2,faces[i].height/2 ),0,0,360,Scalar(255,0,255),4,8,0);

        //矩形
        //        rectangle(frame,
        //                  Rect(faces[i].x,faces[i].y,   //左上坐标
        //                       faces[i].width,faces[i].height),   //宽高
        //                  Scalar(0,0,255),-1,8);

        //换脸
        //        cv::resize(logoImage,logoImage,Size(faces[i].width,faces[i].height));
        //        addWeighted(imageROI,0.3,logoImage,1,0,imageROI);

        imageROI= frame(Rect(faces[i].x,faces[i].y,faces[i].width,faces[i].height));
        imwrite("E:\\BaiduNetdiskDownload\\FaceCompare\\savePic\\saveROI.jpg",imageROI);
    }
    imshow(window_name,frame);
    qDebug()<<"already open video";
}
