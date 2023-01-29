#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;

int main(int argc, const char * argv[]) {
    
    Mat img = imread("C:/Users/saada/Documents/PRCV/Project1/Project1/cat.jpg", IMREAD_COLOR);
    
    if (img.empty()) {
        std::cout << "could not read the image!" << std::endl;
        return 1;
    }
    
    imshow("Display window", img);
    int k = waitKey(0);
    
    //waits until the user presses "q" to exit the window
    while (k != 'q') {
        k = waitKey(0);
    }
    
    return 0;
}
