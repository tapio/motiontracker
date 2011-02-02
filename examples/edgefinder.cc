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
		Mat edges;
		cvtColor(frame, edges, CV_BGR2GRAY); // Switch to grayscale
		GaussianBlur(edges, edges, Size(15,15), 1.5, 1.5); // Blur to reduce noise
		Canny(edges, edges, 20, 60, 3); // Detect edges
		// Add FPS indicator
		putText(edges, boost::lexical_cast<std::string>(counter.getFPS()),
			Point(0,30), FONT_HERSHEY_PLAIN, 2, CV_RGB(255,0,255));
		// Show on screen
		imshow(window, edges);
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

