#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "motiontracker.hh"

using namespace cv;

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
	int board_w = 6;	// Board width in squares
	int board_h = 9;	// Board height in squares

	float boardScaleFactor = 25; // Chessboard square edge length in units you want to use

	int numCorners = board_h * board_w;
	Size board_size = Size(board_w, board_h);

	vector<Point2f> corners;
	vector<Point3f> object_corners;

	vector<vector<Point2f> > image_points;
	vector<vector<Point3f> > object_points;

	int successes = 0;

	for (int i = 0; i < numCorners; ++i) {
		object_corners.push_back(Point3f(boardScaleFactor*(i / board_h), boardScaleFactor*(i % board_h), 0.0f));
	}

	namedWindow("Calibration", 1);
	Mat img;
	Mat gray_img;
	int c;
	while (successes < n_boards) {
		while (waitKey(30) < 0) {
			*cam >> img;

			if (!img.empty()) {
				imshow("Calibration", img);
			}
			displayOverlay("Calibration", "Press any key to take a picture", 1);
		}
		bool patternFound = findChessboardCorners(img, board_size, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		if (patternFound) {
			cvtColor(img, gray_img, CV_BGR2GRAY); // Convert to gray image for cornerSubPix

			cornerSubPix(gray_img, corners, Size(11,11), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(img, board_size, corners, patternFound);

			imshow("Calibration", img);
			displayOverlay("Calibration", "Press S to save calibration image or any other key to discard it", 1);
			c = waitKey(0);

			if (c == 's' || c == 'S') {
				image_points.push_back(corners);
				object_points.push_back(object_corners);
				imshow("Calibration", gray_img);
				displayOverlay("Calibration", "Calibration image saved. Press any key to continue", 1);
				successes++;
				waitKey(0);
			} else {
				imshow("Calibration", img);
				displayOverlay("Calibration", "Calibration image discarded. Press any key to continue", 1);
				waitKey(0);
			}
		} else {
			imshow("Calibration", img);
			displayOverlay("Calibration", "Could not detect calibration pattern in image. Press any key to continue", 1);
			waitKey(0);
		}
	} // End of collection loop

	Mat intrinsic_matrix(3,3,CV_32FC1);
	Mat distortion_coeffs;
	vector<Mat> rvecs;
	vector<Mat> tvecs;

	intrinsic_matrix.ptr<float>(0)[0] = 1;
	intrinsic_matrix.ptr<float>(1)[1] = 1;

	// Calibrate camera
	// NB: Converts matrices to 64bit versions
	calibrateCamera(object_points, image_points, img.size(), intrinsic_matrix, distortion_coeffs, rvecs, tvecs);

	std::cout << "Intrinsic matrix after calibration:" << std::endl;
	std::cout << intrinsic_matrix.at<double>(0,0) << " " << intrinsic_matrix.at<double>(0,1) << " " << intrinsic_matrix.at<double>(0,2) << std::endl;
	std::cout << intrinsic_matrix.at<double>(1,0) << " " <<  intrinsic_matrix.at<double>(1,1)<< " " << intrinsic_matrix.at<double>(1,2) << std::endl;
	std::cout << intrinsic_matrix.at<double>(2,0) << " " << intrinsic_matrix.at<double>(2,1) << " " << intrinsic_matrix.at<double>(2,2) << std::endl;

	std::cout << "Distortion coefficients:" << std::endl;
	std::cout << distortion_coeffs.at<double>(0,0) << " " << distortion_coeffs.at<double>(0,1) << " " << distortion_coeffs.at<double>(0,2) << " " << distortion_coeffs.at<double>(0,3) << std::endl;

	CalibrationParameters(intrinsic_matrix, distortion_coeffs).saveToFile("calibration.xml");

	cvDestroyAllWindows();
	return 0;
}

