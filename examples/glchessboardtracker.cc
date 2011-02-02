#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <boost/lexical_cast.hpp>
#include "motiontracker.hh"
#include "utils.hh"

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

	ChessboardTracker tracker(*webcam, CameraParameters::fromFile("calibration.xml"));

	// TODO: Create OpenGL context and initialize

	while (cv::waitKey(30) < 0) {

		// TODO: Draw a GL cude according to tracker's data
		// We don't use a MotionListener, because GL context is in the
		// main thread, so it's just easier to do the stuff here.

	}

	return 0;
}

