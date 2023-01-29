#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "filter.cpp"
#include "filter.h"
#include <stdlib.h>  
#include <map>

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	map<char, int> userInputMap;
	userInputMap['g'] = 1;//grey scale
	userInputMap['h'] = 2;//alternative greyscale
	userInputMap['c'] = 11;//cartoonise
	userInputMap['l'] = 7;//laplasian
	userInputMap['b'] = 3;//blur
	userInputMap['d'] = 12;//horror lights disco
	userInputMap['x'] = 4;//sobelx
	userInputMap['y'] = 5;//sobely
	userInputMap['w'] = 8;//threshold filter
	userInputMap['i'] = 10;//blur quantize     
	userInputMap['r'] = 6;//grainy filter
	userInputMap['m'] = 9;//magnitude
	
	VideoCapture* vid;
	Mat dst;

	//load video
	vid = new       VideoCapture(0);
	if (!vid->isOpened()) {
		printf("Unable to open video for you\n");
		return(-1);
	}
	bool randomFilter = false;

	namedWindow("Video", 1);
	Mat frame;
	Mat sobxdst;
	Mat sobydst;
	int k = 0;

	for (;;) {
		*vid >> frame;

		if (randomFilter) {
			int arr[5] = { 3,5,11, 1,0 };
			k = arr[rand() % 5];
		}

		if (k == 1) {
			cvtColor(frame, dst, COLOR_RGBA2GRAY, 0);
			frame = dst;
		}

		else if (k == 2) {
			int i = AlternateGreyscaleFilter(frame, dst);
			if (i == 0) {
				frame = dst;
			}
		}

		else if (k == 3) {
			BlurFilter(frame, dst);
			frame = dst;
		}

		else if (k == 4) {
			sobelXFilter(frame, dst);
			convertScaleAbs(dst, frame);
		}

		else if (k == 5) {
			sobelYFilter(frame, dst);
			convertScaleAbs(dst, frame);
		}

		else if (k == 6) {
			Scharr(frame, dst, -1, 0, 1);
			frame = dst;
		}

		else if (k == 7) {
			cvtColor(frame, dst, COLOR_RGB2GRAY, 0);
			Laplacian(frame, dst, CV_8U, 3, 1, 0, BORDER_DEFAULT);
			frame = dst;
		}

		else if (k == 8) {
			threshold(frame, dst, 100, 200, THRESH_BINARY);
			frame = dst;
		}

		else if (k == 9) {
			sobelXFilter(frame, sobxdst);
			sobelYFilter(frame, sobydst);
			magnitudeFilter(sobxdst, sobydst, dst);
			convertScaleAbs(dst, frame);
		}

		else if (k == 10) {
			blurQuantizeFilter(frame, dst, 15);
			frame = dst;
		}

		else if (k == 11) {
			cartoonFilter(frame, dst, 15, 10);
			convertScaleAbs(dst, frame);
		}

		else if (k == 12) {
		}



		if (frame.empty()) {
			printf("frame is empty\n");
			break;
		}

		char key = waitKey(10);


		//user key inputs
		if (key == 'q') {
			//quit
			break;
		}
		else if (key == 's') {
			//save
			imwrite("/Users/saada/Desktop/savedimage.jpg", frame);
		}
		else if (key == 'd') {
			randomFilter = true;

		}
		else if (userInputMap.find(key) != userInputMap.end()) {
			k = userInputMap[key];
			randomFilter = false;
		}
		imshow("Video", frame);
	}

	delete vid;
	return 0;
}