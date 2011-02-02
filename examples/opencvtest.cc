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
	CameraParameters cp;

	// Test variables
	float boardScaleFactor; // Chessboard square edge length in units you want to use

	int board_h;
	int numCorners;
	Size board_size;
	std::string loc;
	std::string rot;

	vector<Point3f> object_corners;
	vector<Point2f> corners;

	Mat rvec,tvec;
	// End of test variables

	MyWebcamReceiver(Webcam& webcam, std::string win)
		: WebcamListener(webcam), window(win), counter(5), boardScaleFactor(25), board_h(9), numCorners(6*9), board_size(Size(6,9))
	{
		for (int i = 0; i < numCorners; ++i) {
			object_corners.push_back(Point3f(boardScaleFactor*(i / board_h), boardScaleFactor*(i % board_h), 0.0f));
		}

		cp = cp.fromFile("calibration.xml");
	}

	void frameEvent(const cv::Mat &frame) {

		bool patternFound = findChessboardCorners(frame, board_size, corners, CALIB_CB_FAST_CHECK);
		Mat img = frame;
		drawChessboardCorners(img, board_size, corners, patternFound);

		/** Old stuff
		Mat edges;
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(15,15), 1.5, 1.5);
		Canny(edges, edges, 20, 60, 3);

		*/

		if (patternFound && (int)corners.size() == numCorners) {
			solvePnP(Mat(object_corners), Mat(corners), cp.intrinsic_parameters,cp.distortion_coeffs,rvec,tvec,false);
			loc = "x: " + boost::lexical_cast<std::string>((int)tvec.at<double>(0,0)) + ' '
				  + "y: " + boost::lexical_cast<std::string>((int)tvec.at<double>(0,1)) + ' '
				  + "z: " + boost::lexical_cast<std::string>((int)tvec.at<double>(0,2));
			putText(img, loc,
				Point(0,60), FONT_HERSHEY_PLAIN, 1, CV_RGB(255,0,255));
			Mat rmat;
			Rodrigues(rvec,rmat);
			double roll, pitch, yaw;
			roll = atan2(rmat.at<double>(1,0),rmat.at<double>(0,0));
			pitch = -asin(rmat.at<double>(2,0));
			yaw = atan2(rmat.at<double>(2,1),rmat.at<double>(2,2));
			rot = "roll (z): " + boost::lexical_cast<std::string>((int)(roll*180/3.1415)) + ' '
				  + "pitch (y): " + boost::lexical_cast<std::string>((int)(pitch*180/3.1415)) + ' '
				  + "yaw (x): " + boost::lexical_cast<std::string>((int)(yaw*180/3.1415));
			putText(img, rot,
				Point(0,75), FONT_HERSHEY_PLAIN, 1, CV_RGB(255,0,255));

		}

		putText(img, boost::lexical_cast<std::string>(counter.getFPS()),
			Point(0,30), FONT_HERSHEY_PLAIN, 2, CV_RGB(255,0,255));

		imshow(window, img);
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

	{
		// Launch a receiver for doing the work whenever a frame is available
		MyWebcamReceiver video(*webcam, "video");

		// Rest here
		while (waitKey(30) < 0);
	}

	cvDestroyWindow("video");
	return 0;
}

