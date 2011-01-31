#include <iostream>
#include "webcam.hh"

#include "cv.h"
#include "highgui.h"
#include "utils.hh"
#include <boost/lexical_cast.hpp>

using namespace cv;

void cb_calibrateButton(int n, void* webcamPtr) {
	Webcam* cam = (Webcam*)webcamPtr;
	int n_boards = 3; // Number of pictures taken
	const int board_dt = 20;
	int board_w = 6; // Board with in squares
	int board_h = 9; // Board height in squares
	int board_n = board_w * board_h;
	Size board_size = Size(board_w, board_h);

	Mat image_points(n_boards*board_n,2,CV_32FC1);
	Mat object_points(n_boards*board_n,2, CV_32FC1);
	Mat point_counts(n_boards, 1, CV_32SC1);
	Mat intrinsic_matrix(3,3,CV_32FC1);
	Mat distortion_coeffs(5,1,CV_32FC1);

	vector<Point2f> corners;
	int corner_count;
	int successes = 0;

	namedWindow("Calibration",1);
	Mat frame;
	Mat cornerImg;
	int c;
	while (successes < n_boards) {
		while (waitKey(30) < 0) {
			*cam >> frame;
			if (!frame.empty()) {
				imshow("Calibration", frame);
			} else
				cam->render();
		}
		bool patternFound = findChessboardCorners(frame,board_size,corners);

		drawChessboardCorners(frame, board_size, Mat(corners), patternFound);
		imshow("Calibration", frame);

		if(patternFound)
			successes++;
		waitKey(0);

	}
	destroyWindow("Calibration");
	return;
}

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
	createButton("Calibrate",cb_calibrateButton,webcam.get(),CV_CHECKBOX);
	FPSCounter counter(5);
	while (waitKey(5) < 0) {
		*webcam >> frame;
		if (!frame.empty()) {
			cvtColor(frame, edges, CV_BGR2GRAY);
			GaussianBlur(edges, edges, Size(15,15), 1.5, 1.5);
			Canny(edges, edges, 20, 60, 3);
			putText(edges, boost::lexical_cast<std::string>(counter.getFPS()),
				Point(0,30), FONT_HERSHEY_PLAIN, 2, CV_RGB(255,0,255));
			putText(edges, boost::lexical_cast<std::string>(webcam->getFPS()),
				cv::Point(0,60), cv::FONT_HERSHEY_PLAIN, 2, CV_RGB(255,0,255));
			imshow("video", edges);
		} else
			webcam->render();
		counter();
	}
	cvDestroyAllWindows();
	return 0;
}

