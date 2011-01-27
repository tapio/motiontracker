#include <iostream>
#include "webcam.hh"

#include "cv.h"
#include "highgui.h"
#include "utils.hh"
#include <boost/lexical_cast.hpp>

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
	FPSCounter counter(5);
	while (waitKey(30) < 0) {
		*webcam >> frame;
		if (!frame.empty()) {
			cvtColor(frame, edges, CV_BGR2GRAY);
			GaussianBlur(edges, edges, Size(15,15), 1.5, 1.5);
			Canny(edges, edges, 20, 60, 3);
			putText(edges, boost::lexical_cast<std::string>(counter.getFPS()),
				Point(0,30), FONT_HERSHEY_PLAIN, 2, CV_RGB(255,0,255));
			imshow("video", edges);
		} else
			webcam->render();
		counter();
	}
	return 0;
}
