#include <iostream>
#include "webcam.hh"

#include "cv.h"
//#include "ml.h"
//#include "cxcore.hpp"
#include "highgui.h"
#include "utils.hh"

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
		return 1;
	}

	VideoCapture cap;
	if (argc > 1) cap.open(string(argv[1]));
	else cap.open(0);

	Mat frame;
	Mat edges;
	namedWindow("video",1);
	for (;;) {
		cap >> frame;
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(15,15), 1.5, 1.5);
		Canny(edges, edges, 20, 60, 3);
		imshow("video", edges);
		if(waitKey(30) >= 0) break;
	}
	return 0;
}
