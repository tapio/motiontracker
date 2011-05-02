/**
 * Tool for calibrating the color values of the reference object.
 */

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "motiontracker.hh"

using namespace cv;

std::vector<int> hues;

void mouseHandler(int event, int x, int y, int flags, void *param)
{
	(void) flags;
	Mat* HSV = (Mat*)param;
	switch(event) {
		// Left button down
		case CV_EVENT_LBUTTONDOWN:
			std::cout << "Left button down:" << HSV->depth() << " " << std::endl;
			std::cout << "Hue: " <<  (int)HSV->at<Vec3b>(y,x)[0] << std::endl;
			std::cout << "Saturation: " <<  (int)HSV->at<Vec3b>(y,x)[1] << std::endl;
			std::cout << "Value: " <<  (int)HSV->at<Vec3b>(y,x)[2] << std::endl;
			if (hues.size() < 4)
				hues.push_back((int)HSV->at<Vec3b>(y,x)[0]);
			break;
	}
}

int main(int argc, char** argv)
{
	(void)argc; (void)argv; // Suppress warnings
	boost::scoped_ptr<Webcam> cam;
	try {
		cam.reset(new Webcam);
	} catch (std::exception&) {
		std::cout << "Error: Could not initialize webcam" << std::endl;
		return EXIT_FAILURE;
	}

	namedWindow("Calibration", 1);

	Mat img;
	Mat imgHSV;
	Mat bin_image;

	int hue;
	int dH = 0;
	int hue_switch_value = 10;
	int satvall_switch_value = 120;
	int satvalh_switch_value = 255;

	std::vector<int> hue_thresholds;
	std::vector<int> satval_l;
	std::vector<int> satval_h;

	CvFont font = fontQt("Times");

	// Acquire calibration image from webcamera
	while (waitKey(30) < 0) {
		*cam >> img;
		if (!img.empty()) {
			imshow("Calibration", img);
			displayOverlay("Calibration", "Press any key to take a picture", 1);
		}
	}

	addText(img, "Click on 4 feature points and press any key", Point(10,20), font);
	imshow("Calibration", img);
	cam.reset();
	cv::cvtColor(img, imgHSV, CV_BGR2HSV); // Switch to HSV color space
	setMouseCallback("Calibration", mouseHandler, (void*)&imgHSV);
	waitKey(0);

	if (hues.size() == 4) {
		namedWindow("Thresholded image",1);
		for (int i = 0; i < 4; ++i) {
			hue = hues.at(i);
			cv::inRange(imgHSV, cv::Scalar(hue - dH, 120, 120), cv::Scalar(hue + dH, 255, 255), bin_image);
			imshow("Thresholded image", bin_image);
			createTrackbar( "Hue", "Thresholded image", &hue_switch_value, 50);
			createTrackbar( "Sat/Val low", "Thresholded image", &satvall_switch_value, 255);
			createTrackbar( "Sat/Val high", "Thresholded image", &satvalh_switch_value, 255);
			while (waitKey(30) < 0) {
				dH = hue_switch_value;
				cv::inRange(imgHSV, cv::Scalar(hue - dH, satvall_switch_value, satvall_switch_value), cv::Scalar(hue + dH, satvalh_switch_value, satvalh_switch_value), bin_image);
				imshow("Thresholded image", bin_image);
			}
			hue_thresholds.push_back(dH);
			satval_l.push_back(satvall_switch_value);
			satval_h.push_back(satvalh_switch_value);
		}
	} else {
		std::cout << "Error: Expected 4 hues, got " << hues.size() << std::endl;
		return EXIT_FAILURE;
	}

	CalibrationParameters(Mat(3,3,CV_64F, 0.0f), Mat(1,4,CV_64F, 0.0f), Mat(hues), Mat(hue_thresholds), Mat(satval_l), Mat(satval_h)).saveToFile("calibration.xml");
	std::cout << "Parameters written to calibration.xml" << std::endl;
	return EXIT_SUCCESS;
}


