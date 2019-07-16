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
#include "eyedetection.h"

#define CV_COLOR_RED cv::Scalar(0, 0, 255)

String face_cascade_name = "D:\\Opencv\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "D:\\Opencv\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";

Mat imageROI;
Mat eyeROI;
int open_eye = 0,close_eye = 0,res_eye = 0;

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

    //加载cascades
    if(!face_cascade.load(face_cascade_name))
    {
        qDebug("--(!)Error loading face cascade\n");
    }
    if(!eyes_cascade.load(eyes_cascade_name))
    {
        qDebug("--(!)Error loading eyes cascade\n");
    }
    //打开视频
    //    capture.open( -1 );   //电脑摄像头
    if (! capture.isOpened())
    {
        qDebug("--(!)Error opening video capture\n");
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
        if( c == 27 )   //按键的ASCII码值
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

    int white_num;

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

        //检测到脸后检测眼睛
        Mat faceROI = frame_gray(faces[i]);
        std::vector<Rect> eyes;
        eyes_cascade.detectMultiScale(faceROI,eyes,1.1,2,0 | CASCADE_SCALE_IMAGE,Size(30,30));

        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center(faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2);
            int radius = cvRound((eyes[j].width + eyes[j].height)*0.25 );
            //circle(frame,eye_center,radius,Scalar(255,0,0 ),4,8,0);

            eyeROI = frame(Rect(eye_center.x-radius,eye_center.y-radius/2*1/4,eyes[j].width,eyes[j].height/2*1/3));
            imwrite("E:\\BaiduNetdiskDownload\\FaceCompare\\savePic\\eyeROI.jpg",eyeROI);

            qDebug()<<"Eyes have been detected";

            Eyedetection *lunkuo = new Eyedetection;
            white_num = lunkuo->Blink(eyeROI);
            qDebug()<<"profile num is:"<<white_num;

            if(white_num < 180 && white_num > 155)
            {
                open_eye = white_num;
            }else if(white_num > 222 && white_num < 240)
            {
                close_eye = white_num;
            }else
            {
                qDebug()<<"Plase retry!";
            }

            res_eye = close_eye - open_eye;

            qDebug()<<"eye_value is:"<<res_eye;

            if(res_eye >= 52 && res_eye <= 65)
            {
                LiveBody = true;
                break;
                isLive = 1;
            }else{
                LiveBody = false;
                isLive = 0;
            }
        }
    }

    imshow(window_name,frame);
    OpenedVideo = true;
    qDebug()<<"already open video";
}
