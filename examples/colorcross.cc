#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <boost/lexical_cast.hpp>
#include "motiontracker.hh"
#include "utils.hh"

using namespace cv;

int main(int argc, char** argv)
{
	(void)argc; (void)argv; // Suppress warnings
	Mat img = imread( "img/pos_init.png", 1 );

	const int dH = 10;

	int hues[] = {120, 30, 0, 65}; // Blue Yellow Red Green

	// Solve the position
	cv::Mat imgHSV, thresh;
	vector<Point2f> image_points;

	vector<Point3f> object_points;
	object_points.push_back(Point3f(0,135,0)); // Blue ball
	object_points.push_back(Point3f(135,0,0)); // Yellow ball
	object_points.push_back(Point3f(0,-135,0)); // Red ball
	object_points.push_back(Point3f(-135,0,0));	// Green ball

	Moments m;
	int x, y;

	cv::cvtColor(img, imgHSV, CV_BGR2HSV); // Switch to HSV color space
	for (int i = 0; i < 4; ++i) {
		cv::inRange(imgHSV, cv::Scalar(hues[i] - dH, 254, 254), cv::Scalar(hues[i] + dH, 256, 256), thresh);
		m = cv::moments(thresh, true);
		x = m.m10 / m.m00;
		y = m.m01 / m.m00;

		circle(img, Point(x, y), i, CV_RGB(0,0,0),8);
		image_points.push_back(Point2f((float)x,(float)y));
	}
	Mat rvec, tvec;

	CameraParameters cp;
	cp = cp.fromFile("calibration.xml");

	Mat intrinsic_matrix(3,3, CV_64FC1);
	Mat distortion_coeffs(1,5,CV_64FC1);

	intrinsic_matrix.ptr<double>(0)[0] = 1.;
	intrinsic_matrix.ptr<double>(1)[1] = 1.;
	intrinsic_matrix.ptr<double>(0)[2] = 320.;
	intrinsic_matrix.ptr<double>(1)[2] = 240.;

	distortion_coeffs.ptr(0)[0] = 0.;
	distortion_coeffs.ptr(0)[1] = 0.;
	distortion_coeffs.ptr(0)[2] = 0.;
	distortion_coeffs.ptr(0)[3] = 0.;
	distortion_coeffs.ptr(0)[4] = 0.;

	std::cout << "Intrinsic matrix after calibration:" << std::endl;
	std::cout << intrinsic_matrix.at<double>(0,0) << " " << intrinsic_matrix.at<double>(0,1) << " " << intrinsic_matrix.at<double>(0,2) << std::endl;
	std::cout << intrinsic_matrix.at<double>(1,0) << " " <<  intrinsic_matrix.at<double>(1,1)<< " " << intrinsic_matrix.at<double>(1,2) << std::endl;
	std::cout << intrinsic_matrix.at<double>(2,0) << " " << intrinsic_matrix.at<double>(2,1) << " " << intrinsic_matrix.at<double>(2,2) << std::endl;

	std::cout << "Distortion coefficients:" << std::endl;
	std::cout << distortion_coeffs.at<double>(0,0) << " " << distortion_coeffs.at<double>(0,1) << " " << distortion_coeffs.at<double>(0,2) << " " << distortion_coeffs.at<double>(0,3) << std::endl;

	solvePnP(Mat(object_points), Mat(image_points), intrinsic_matrix,distortion_coeffs,rvec,tvec,false);

	std::string loc, rot;
	loc = "x: " + boost::lexical_cast<std::string>((int)tvec.at<double>(0,0)) + ' '
		  + "y: " + boost::lexical_cast<std::string>((int)tvec.at<double>(0,1)) + ' '
		  + "z: " + boost::lexical_cast<std::string>((int)tvec.at<double>(0,2));
	putText(img, loc,
		Point(0,455), FONT_HERSHEY_PLAIN, 1, CV_RGB(255,0,255));
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
		Point(0,475), FONT_HERSHEY_PLAIN, 1, CV_RGB(255,0,255));


	namedWindow( "Image" );
	//namedWindow( "HSV" );
	//namedWindow( "Threshold" );

	// Show the original image
	imshow("Image", img);

	// Create an image for the output

	// Show the processed image
	//imshow("HSV", imgHSV);
	//imshow("Threshold", thresh);

	waitKey(0);

	return 0;
}
