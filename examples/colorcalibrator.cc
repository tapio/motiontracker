#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "motiontracker.hh"

using namespace cv;

void mouseHandler(int event, int x, int y, int flags, void *param)
{
	Mat* HSV = (Mat*)param;
	switch(event) {
		/* left button down */
		case CV_EVENT_LBUTTONDOWN:
			std::cout << "Left button down:" << std::endl;
			std::cout << "Hue: " <<  (int)HSV->at<Vec3b>(x,y)[0] << std::endl;
			break;
/*
		// right button down
		case CV_EVENT_RBUTTONDOWN:
			std::cout << "Right button down:" << x << y << std::endl;
			break;

		// mouse move
		case CV_EVENT_MOUSEMOVE:
			break;

			*/
	}
}

int main(int argc, char** argv)
{
	(void)argc; (void)argv; // Suppress warnings
	boost::scoped_ptr<Webcam> cam;
	try {
		std::cout << "Webcam succesfully initialized: ";
		cam.reset(new Webcam);
		std::cout << "yes" << std::endl;
	} catch (std::exception&) {
		std::cout << "no" << std::endl;
		return 1;
	}

	int n_boards = 8;	// Number of pictures taken
	int successes = 0;

	std::vector<int> hues;
	std::vector<int> hue_thresholds;

	namedWindow("Calibration", 1);
	
	Mat img;
	Mat imgHSV;
	Mat bin_image;
	int c;
	setMouseCallback( "Calibration", mouseHandler, (void*)&imgHSV );

	CvFont font = fontQt("Times");

	// Acquire calibration image from webcamera
	while (waitKey(30) < 0) {
		*cam >> img;
		if (!img.empty()) {
			imshow("Calibration", img);
			displayOverlay("Calibration", "Press any key to take a picture", 1);
		}
	}

	addText(img,"Click on a feature detect",Point(10,20),font);
	imshow("Calibration", img);
	cam.reset();
	cv::cvtColor(img, imgHSV, CV_BGR2HSV); // Switch to HSV color space
	waitKey(0);

	cvDestroyAllWindows();
	return 0;
}


