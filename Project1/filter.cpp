#include <iostream>
#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include "filter.h"
using namespace cv;


//alternate greyscale 
int AlternateGreyscaleFilter(Mat& src, Mat& dst) {

	dst.create(src.size(), src.type());
	for (int i = 0; i < src.rows; i++) {
		Vec3b* ptr = src.ptr<      Vec3b>(i);

		for (int j = 0; j < src.cols; j++) {
			int blue = ptr[j][0];
			int green = ptr[j][1];
			int red = ptr[j][2];

			int avgOfColors = (blue + green + red) / 3;
			ptr[j][0] = avgOfColors;
			ptr[j][1] = avgOfColors;
			ptr[j][2] = avgOfColors;
		}
	}

	return 0;
}

//image blur 
int BlurFilter(Mat& src, Mat& dst) {


	Mat intermed;
	intermed.create(src.size(), src.type());
	dst.create(src.size(), src.type());

	Vec3i result = { 0, 0, 0 };
	Vec3i resultCol = { 0, 0, 0 };

	for (int i = 2; i < src.rows - 2; i++) {
		for (int j = 2; j < src.cols - 2; j++) {
			for (int c = 0; c < 3; c++) {
				result[c] = src.at<      Vec3b>(i - 2, j - 2)[c] + src.at<      Vec3b>(i - 2, j - 1)[c] * 2
					+ src.at<      Vec3b>(i - 2, j)[c] * 4 + src.at<      Vec3b>(i - 2, j + 1)[c] * 2
					+ src.at<      Vec3b>(i - 2, j + 2)[c];

				result[c] /= 10;
			}
			intermed.at<      Vec3b>(i, j) = result;
		}
	}

	for (int i = 2; i < intermed.rows - 2; i++) {
		for (int j = 2; j < intermed.cols - 2; j++) {
			for (int c = 0; c < 3; c++) {
				result[c] = intermed.at<      Vec3b>(i - 2, j - 2)[c] + intermed.at<      Vec3b>(i - 1, j - 2)[c] * 2
					+ intermed.at<      Vec3b>(i, j - 2)[c] * 4 + intermed.at<      Vec3b>(i + 1, j - 2)[c] * 2
					+ intermed.at<      Vec3b>(i + 2, j - 2)[c];

				result[c] /= 10;
			}
			dst.at<      Vec3b>(i, j) = result;
		}
	}
	return 0;
}


//sobelx
int sobelXFilter(Mat& src, Mat& dst) {
	Mat intermed;
	intermed.create(src.size(), CV_16SC3);
	dst.create(src.size(), CV_16SC3);
	Vec3s result = { 0, 0, 0 };

	for (int i = 1; i < src.rows - 1; i++) {
		for (int j = 1; j < src.cols - 1; j++) {
			for (int c = 0; c < 3; c++) {
				result[c] = src.at<      Vec3b>(i - 1, j - 1)[c] * -1 + src.at<      Vec3b>(i - 1, j)[c] * 0 + src.at<      Vec3b>(i - 1, j + 1)[c];
			}
			intermed.at<      Vec3s>(i, j) = result;
		}
	}

	for (int i = 1; i < intermed.rows - 1; i++) {
		for (int j = 1; j < intermed.cols - 1; j++) {
			for (int c = 0; c < 3; c++) {
				result[c] = intermed.at<      Vec3s>(i - 1, j - 1)[c] + intermed.at<      Vec3s>(i, j - 1)[c] * 2 + intermed.at<      Vec3s>(i + 1, j - 1)[c];
				result[c] /= 4;
			}
			dst.at<      Vec3s>(i, j) = result;
		}
	}
	return 0;
}

//sobely
int sobelYFilter(Mat& src, Mat& dst) {

	Mat intermed;
	intermed.create(src.size(), CV_16SC3);
	dst.create(src.size(), CV_16SC3);
	Vec3s result = { 0, 0, 0 };
	for (int i = 1; i < src.rows - 1; i++) {
		for (int j = 1; j < src.cols - 1; j++) {
			for (int c = 0; c < 3; c++) {
				result[c] = src.at<      Vec3b>(i - 1, j - 1)[c] + src.at<      Vec3b>(i - 1, j)[c] * 2
					+ src.at<      Vec3b>(i - 1, j + 1)[c];

				result[c] /= 4;
			}
			intermed.at<      Vec3s>(i, j) = result;
		}
	}


	for (int i = 1; i < intermed.rows - 1; i++) {
		for (int j = 1; j < intermed.cols - 1; j++) {
			for (int c = 0; c < 3; c++) {
				result[c] = intermed.at<      Vec3s>(i - 1, j - 1)[c] + intermed.at<      Vec3s>(i, j - 1)[c] * 0
					+ intermed.at<      Vec3s>(i + 1, j - 1)[c] * -1;
			}
			dst.at<      Vec3s>(i, j) = result;
		}
	}
	return 0;
}

//magnitude
int magnitudeFilter(Mat& sx, Mat& sy, Mat& dst) {

	dst.create(sx.size(), sx.type());
	Vec3s result = { 0, 0, 0 };
	for (int i = 0; i < sx.rows; i++) {
		for (int j = 0; j < sx.cols; j++) {
			for (int c = 0; c < 3; c++) {
				result[c] = sqrt(pow(sx.at<      Vec3s>(i, j)[c], 2) + pow(sy.at<      Vec3s>(i, j)[c], 2));
			}
			dst.at<      Vec3s>(i, j) = result;
		}
	}

	return 0;
}

//blur quantized
int blurQuantizeFilter(Mat& src, Mat& dst, int levels) {

	Mat intermed;
	intermed.create(src.size(), src.type());
	dst.create(src.size(), src.type());
	Vec3i result = { 0, 0, 0 };
	BlurFilter(src, intermed);
	int buckets = 255 / levels;
	for (int i = 0; i < intermed.rows; i++) {
		for (int j = 0; j < intermed.cols; j++) {
			for (int c = 0; c < 3; c++) {
				result[c] = intermed.at<      Vec3b>(i, j)[c] / buckets;
				result[c] = result[c] * buckets;
			}
			dst.at<      Vec3b>(i, j) = result;
		}
	}
	return 0;
}

//cartoon
int cartoonFilter(Mat& src, Mat& dst, int levels, int magThreshold) {
	Mat xPic;
	xPic.create(src.size(), CV_16SC3);
	Mat yPic;
	yPic.create(src.size(), CV_16SC3);
	Mat magImg;
	magImg.create(src.size(), CV_16SC3);
	Mat intermed;
	intermed.create(src.size(), src.type());
	dst.create(src.size(), CV_16SC3);

	sobelXFilter(src, xPic);
	sobelYFilter(src, yPic);
	magnitudeFilter(xPic, yPic, magImg);

	blurQuantizeFilter(src, intermed, levels);

	Vec3s resultIntermed = { 0, 0, 0 };
	Vec3s resultMagThresh = { 0, 0, 0 };

	for (int i = 0; i < intermed.rows; i++) {
		for (int j = 0; j < intermed.cols; j++) {
			for (int c = 0; c < 3; c++) {
				resultIntermed[c] = intermed.at<      Vec3b>(i, j)[c];
				resultMagThresh[c] = magImg.at<      Vec3s>(i, j)[c];
			}
			int avgnum = resultMagThresh[0] + resultMagThresh[1] + resultMagThresh[2];
			avgnum /= 3;
			if (avgnum > magThreshold) {
				resultIntermed = { 0,0,0 };
			}

			dst.at<      Vec3s>(i, j) = resultIntermed;
		}
	}

	return 0;
}