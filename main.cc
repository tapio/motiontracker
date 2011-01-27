#include <iostream>
#include "webcam.hh"

#include "cv.h"
#include "highgui.h"

using namespace cv;
int main(int argc, char** argv)
{
	(void)argc; (void)argv; // Suppress warnings
	boost::scoped_ptr<Webcam> webcam;
	try {
		std::cout << "Webcam succesfully initialized: ";
		webcam.reset(new Webcam);
		std::cout << "yes" << std::endl;
	} catch (std::exception&) {
		std::cout << "no" << std::endl;
		return 1;
	}

	Mat frame;
	Mat edges;
	namedWindow("video",1);
	while (waitKey(30) < 0) {
		*webcam >> frame;
		if (!frame.empty()) {
			cvtColor(frame, edges, CV_BGR2GRAY);
			GaussianBlur(edges, edges, Size(15,15), 1.5, 1.5);
			Canny(edges, edges, 20, 60, 3);
			imshow("video", edges);
		} else
			webcam->render();
	}
	return 0;
}
