#ifndef EYEDETECTION_H
#define EYEDETECTION_H

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

class Eyedetection
{
public:
    int Blink(Mat eyeroi);

    int bSums(Mat src);
};

#endif // EYEDETECTION_H
