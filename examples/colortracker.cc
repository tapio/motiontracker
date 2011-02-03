#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <boost/lexical_cast.hpp>
#include "motiontracker.hh"
#include "utils.hh"

using namespace cv;

struct MyWebcamReceiver: public WebcamListener {
	std::string window;
	FPSCounter counter;

	MyWebcamReceiver(Webcam& webcam, std::string win)
		: WebcamListener(webcam), window(win), counter(5)
	{ }

	void frameEvent(const Mat &frame) {
		Mat img(frame), imgHSV, thresh;
		cvtColor(frame, imgHSV, CV_BGR2HSV); // Switch to grayscale
		//GaussianBlur(imgHSV, imgHSV, Size(15,15), 1.5, 1.5); // Blur to reduce noise

		// Here we have the HSV color limits. This is setup for dark blue. Good luck finding yours.
		inRange(imgHSV, Scalar(80, 120, 120), Scalar(120, 255, 255), thresh);

		// Calculate the moments to estimate the position
		Moments m = moments(thresh, true);
		int x = m.m10 / m.m00;
		int y = m.m01 / m.m00;

		// Add FPS indicator
		putText(img, boost::lexical_cast<std::string>(counter.getFPS()),
			Point(x,y), FONT_HERSHEY_PLAIN, 2, CV_RGB(255,0,255));
		// Show on screen
		imshow(window, img);
		counter(); // Update FPS counter
	}
};

int main(int argc, char** argv)
{
	(void)argc; (void)argv; // Suppress warnings
	boost::scoped_ptr<Webcam> webcam;
	try {
		webcam.reset(new Webcam);
	} catch (std::exception& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}

	std::string window = "video";
	namedWindow(window, 1);

	{
		// Launch a receiver for doing the work whenever a frame is available
		MyWebcamReceiver video(*webcam, window);

		// Rest here
		while (waitKey(30) < 0);
	}

	cvDestroyWindow(window.c_str());
	return 0;
}

