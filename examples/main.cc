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
/*
	Mat image_points(n_boards*board_n, 2, CV_32FC1);
	Mat object_points(n_boards*board_n, 2, CV_32FC1);
	Mat point_counts(n_boards, 1, CV_32SC1);
	Mat intrinsic_matrix(3, 3, CV_32FC1);
	Mat distortion_coeffs(5, 1, CV_32FC1);

	vector<Point2f> corners;
	vector<vector<Point2f> > image_points2;
	int corner_count;
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
			} else
				cam->render();
			displayOverlay("Calibration", "Press any key to take a picture", 1);
		}
		bool patternFound = findChessboardCorners(frame, board_size, corners);

		if (patternFound) {
			cvtColor(frame, img, CV_BGR2GRAY);
			drawChessboardCorners(img, board_size, Mat(corners), patternFound);
			cornerSubPix(img, corners, Size(11,11), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			imshow("Calibration", img);
			displayOverlay("Calibration", "Press S to save calibration image or any other key to discard it", 1);
			c = waitKey(0);
			// THIS PART IS B0RKED
			// TODO: object_points and image_points should be vectors !
			if (c == 's' || c == 'S') {
				step = successes * board_n;
				for (int i = step, j = 0; j < board_n; ++i, ++j) {
					image_points.at<float>(i,0) = corners[j].x;
					image_points.at<float>(i,1) = corners[j].y;
					image_points2.push_back(corners);
					object_points.at<float>(i,0) = j/board_w;
					object_points.at<float>(i,1) = j%board_w;
					object_points.at<float>(i,2) = 0.0f;
				}
				point_counts.at<int>(successes,0) = board_n;
			// END OF B0RK
				imshow("Calibration", img);
				displayOverlay("Calibration", "Calibration image saved. Press any key to continue", 1);
				successes++;
				waitKey(0);
			}
			else {
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

	// Initialize focal lenghts
	intrinsic_matrix.at<float>(0,0) = 1.0f;
	intrinsic_matrix.at<float>(1,1) = 1.0f;

	vector<Mat> rvecs;
	vector<Mat> tvecs;
	// Calibrate camera
	calibrateCamera(object_points, image_points2, frame.size(), intrinsic_matrix, distortion_coeffs, rvecs, tvecs,0);
	std::cout << intrinsic_matrix.at<float>(0,0) << std::endl;
	std::cout << intrinsic_matrix.at<float>(1,1) << std::endl;

	destroyWindow("Calibration");
	return;
	*/
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
	namedWindow("video", 1);
	createButton("Calibrate", cb_calibrateButton,webcam.get(), CV_CHECKBOX);
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

