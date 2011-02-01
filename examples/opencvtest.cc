#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <boost/lexical_cast.hpp>
#include "motiontracker.hh"
#include "utils.hh"

using namespace cv;

struct MyWebcamReceiver: public FrameReceiver {
	std::string window;
	FPSCounter counter;

	MyWebcamReceiver(Webcam& webcam, std::string win)
		: FrameReceiver(webcam), window(win), counter(5)
	{}

	void frameEvent(const cv::Mat &frame) {
		Mat edges;
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(15,15), 1.5, 1.5);
		Canny(edges, edges, 20, 60, 3);
		putText(edges, boost::lexical_cast<std::string>(counter.getFPS()),
			Point(0,30), FONT_HERSHEY_PLAIN, 2, CV_RGB(255,0,255));
		imshow(window, edges);
		counter();
	}
};


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

	namedWindow("video", 1);

	{
		// Launch a receiver for doing the work whenever a frame is available
		MyWebcamReceiver video(*webcam, "video");

		// Rest here
		while (waitKey(30) < 0);
	}

	cvDestroyWindow("video");
	return 0;
}

