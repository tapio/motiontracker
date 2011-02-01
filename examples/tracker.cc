#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "motiontracker.hh"
#include "utils.hh"

using namespace cv;

int main(int argc, char** argv)
{
	(void)argc; (void)argv; // Suppress warnings
	boost::scoped_ptr<Webcam> webcam;
	CameraParameters camparams;
	try {
		webcam.reset(new Webcam);
		camparams = CameraParameters::fromFile("calibration.xml");
	} catch (std::exception const &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}

	MotionTracker tracker(*webcam, camparams);

	// TODO
	(void)tracker;

	return 0;
}

