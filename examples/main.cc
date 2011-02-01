#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <boost/lexical_cast.hpp>
#include "motiontracker.hh"
#include "utils.hh"

using namespace cv;

void cb_calibrateButton(int n, void* webcamPtr) {
	Webcam* cam = (Webcam*)webcamPtr;
	int n_boards = 3; // Number of pictures taken
	const int board_dt = 20;
	int board_w = 6; // Board width in squares
	int board_h = 9; // Board height in squares
	int board_n = board_h * board_w;
	Size board_size = Size(board_w, board_h);

	Mat distortion_coeffs(5, 1, CV_32FC1);

	vector<Point2f> corners;
	vector<vector<Point2f> > image_points2;

	// Here we assume that the calibration pattern is fully visible in every image,
	// which means that object_points has n_boards same vectors

	vector<Point3f> object_corners;
	vector<vector<Point3f> > object_points2;

	for (int i = 0; i < board_n; ++i) {
		object_corners.push_back(Point3f(i / board_w, i % board_w, 0));
	}

	for (int i = 0; i < n_boards; ++i)
		object_points2.push_back(object_corners);

	int step, successes = 0;

	namedWindow("Calibration", 1);
	Mat frame;
	Mat img;
	int c;
	while (successes < n_boards) {
		while (waitKey(30) < 0) {
			*cam >> frame;

			if (!frame.empty()) {
				imshow("Calibration", frame);
			}
			displayOverlay("Calibration", "Press any key to take a picture", 1);
		}
		bool patternFound = findChessboardCorners(frame, board_size, corners);

		if (patternFound) {
			cvtColor(frame, img, CV_BGR2GRAY);
			drawChessboardCorners(frame, board_size, Mat(corners), patternFound);
			cornerSubPix(img, corners, Size(11,11), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			imshow("Calibration", frame);
			displayOverlay("Calibration", "Press S to save calibration image or any other key to discard it", 1);
			c = waitKey(0);

			if (c == 's' || c == 'S') {
				image_points2.push_back(corners);
				imshow("Calibration", img);
				displayOverlay("Calibration", "Calibration image saved. Press any key to continue", 1);
				successes++;
				waitKey(0);
			} else {
				imshow("Calibration", img);
				displayOverlay("Calibration", "Calibration image discarded. Press any key to continue", 1);
				waitKey(0);
			}
		} else {
			imshow("Calibration", frame);
			displayOverlay("Calibration", "Could not detect calibration pattern in image. Press any key to continue", 1);
			waitKey(0);
		}
	} // End of collection loop

	Mat intrinsic_matrix = initCameraMatrix2D(object_points2, image_points2, frame.size());

	std::cout << "Camera parameters:" << std::endl << "fx: " << intrinsic_matrix.at<float>(0,0) << std::endl;
	std::cout << "fy: " << intrinsic_matrix.at<float>(1,1) << std::endl;
	std::cout << "cx: " << intrinsic_matrix.at<float>(0,2) << std::endl;
	std::cout << "cy: " << intrinsic_matrix.at<float>(2,2) << std::endl;

	vector<Mat> rvecs;
	vector<Mat> tvecs;
	// Calibrate camera
	double error = calibrateCamera(object_points2, image_points2, Size(640,480), intrinsic_matrix, distortion_coeffs, rvecs, tvecs,0);

	std::cout << "Camera parameters:" << std::endl << "fx: " << intrinsic_matrix.at<float>(0,0) << std::endl;
	std::cout << "fy: " << intrinsic_matrix.at<float>(1,1) << std::endl;
	std::cout << "cx: " << intrinsic_matrix.at<float>(0,2) << std::endl;
	std::cout << "cy: " << intrinsic_matrix.at<float>(2,2) << std::endl;
	std::cout << "estimation error: " << error << std::endl;
	destroyWindow("Calibration");
	return;

}


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
	createButton("Calibrate", cb_calibrateButton,webcam.get(), CV_CHECKBOX);

	// Launch a receiver for doing the work whenever a frame is available
	MyWebcamReceiver video(*webcam, "video");

	// Rest here
	while (waitKey(30) < 0);

	cvDestroyAllWindows();
	return 0;
}

