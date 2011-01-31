#include <iostream>
#include "webcam.hh"

#include "cv.h"
#include "highgui.h"
#include "utils.hh"
#include <boost/lexical_cast.hpp>

using namespace cv;

void cb_calibrateButton(int n, void* webcamPtr) {
	Webcam* cam = (Webcam*)webcamPtr;
	int n_boards = 8; // Number of pictures taken
	const int board_dt = 20;
	int board_w = 5; // Board with in squares
	int board_h = 8; // Board height in squares
	int board_n = board_w * board_h;
	Size board_size = Size(board_w, board_h);

	namedWindow("Calibration",1);
	Mat frame;
	while (waitKey(30) < 0) {
		*cam >> frame;
		if (!frame.empty()) {
			imshow("Calibration", frame);
		} else
			cam->render();
	}
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
	createButton("Calibrate",cb_calibrateButton,&webcam);
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

