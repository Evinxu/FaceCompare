#include "eyedetection.h"
#include <QDebug>

//连通区域的变化统计个数
int Eyedetection::Blink(Mat eyeroi)
{

    Mat gray;
    cvtColor(eyeroi,gray,CV_BGR2GRAY);

    Mat two_value;
    threshold(gray,two_value,0,255,CV_THRESH_OTSU+CV_THRESH_BINARY);

    Mat element = getStructuringElement(MORPH_RECT,Size(17,3));
    morphologyEx(two_value,two_value,MORPH_CLOSE,element);

    vector< vector< Point> > contours;
    findContours(two_value,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

    int i;
    //    for(i=0;i<contours.size();i++){}

    imwrite("E:\\BaiduNetdiskDownload\\FaceCompare\\savePic\\eye_roi.jpg",two_value);

    //    i = contours.size();

    i = bSums(two_value);   //白色像素点个数

    return i;

}

//白色像素点个数
int Eyedetection::bSums(Mat src)
{

    int counter = 0;
    //迭代器访问像素点
    Mat_<uchar>::iterator it = src.begin<uchar>();
    Mat_<uchar>::iterator itend = src.end<uchar>();
    for (; it!=itend; ++it)
    {
        if((*it)>0) counter+=1;//二值化后，像素点是0或者255
    }
    return counter;
}
