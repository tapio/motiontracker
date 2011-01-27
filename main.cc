#include <iostream>
#include "webcam.hh"

#include "cv.h"
#include "ml.h"
#include "cxcore.hpp"
#include "highgui.h"

using namespace cv;
int main(int argc, char** argv)
{
	(void)argc; (void)argv; // Suppress warnings
	std::cout << "Webcam succesfully initialized: ";
	try {
		Webcam webcam;
		std::cout << "yes" << std::endl;
	} catch (std::exception& e) {
		std::cout << "no" << std::endl;
	}

        VideoCapture cap;
        if (argc > 1) cap.open(string(argv[1]));
        else cap.open(0);
        Mat frame;
        namedWindow("video",1);
        for (;;) {
            cap >> frame;
            if(!frame.data) break;
            GaussianBlur(frame,frame,Size(25,25),6,6);
            imshow("video", frame);
            if(waitKey(30) >= 0) break;
        }
        return 0;
}
