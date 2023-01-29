#ifndef FILTER_H
#define FILTER_H

using namespace cv;

int AlternateGreyscaleFilter(Mat& src, Mat& dst);

int BlurFilter(Mat& src, Mat& dst);

int sobelXFilter(Mat& src, Mat& dst);

int sobelYFilter(Mat& src, Mat& dst);

int magnitudeFilter(Mat& sx, Mat& sy, Mat& dst);

int blurQuantizeFilter(Mat& src, Mat& dst, int levels);

int cartoonFilter(Mat& src, Mat& dst, int levels, int magThreshold);

#endif 



