#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "motiontracker.hh"


MotionTracker::MotionTracker(Webcam &webcam, const CameraParameters &camParams)
	: WebcamListener(webcam), m_camParams(camParams), m_pos(), m_rot(), m_counter(5)
{ }

cv::Vec3f MotionTracker::getRotation() const {
	boost::mutex::scoped_lock l(m_mutex);
	return m_rot;
}

cv::Vec3f MotionTracker::getPosition() const {
	boost::mutex::scoped_lock l(m_mutex);
	return m_pos;
}

int MotionTracker::getFPS() const {
	boost::mutex::scoped_lock l(m_mutex);
	return m_counter.getFPS();
}



ChessboardTracker::ChessboardTracker(Webcam &webcam, const CameraParameters &camParams)
	: MotionTracker(webcam, camParams), m_boardScaleFactor(25), m_boardH(9), m_numCorners(6*9), m_boardSize(cv::Size(6,9))
{
	for (int i = 0; i < m_numCorners; ++i)
		m_objectCorners.push_back(cv::Point3f(m_boardScaleFactor*(i / m_boardH), m_boardScaleFactor*(i % m_boardH), 0.0f));
}

void ChessboardTracker::frameEvent(const cv::Mat& frame) {
	// Find the chessboard
	bool patternFound = cv::findChessboardCorners(frame, m_boardSize, m_corners, cv::CALIB_CB_FAST_CHECK);

	// Solve the pose
	cv::Mat pos, rot;
	if (patternFound && (int)m_corners.size() == m_numCorners) {
		cv::solvePnP(cv::Mat(m_objectCorners), cv::Mat(m_corners), m_camParams.intrinsic_parameters, m_camParams.distortion_coeffs, rot, pos, false);

		// Assign new values
		boost::mutex::scoped_lock l(m_mutex);
		m_pos = cv::Vec3f(pos.at<double>(0,0), pos.at<double>(0,1), pos.at<double>(0,2));
		m_rot = cv::Vec3f(rot.at<double>(0,0), rot.at<double>(0,1), rot.at<double>(0,2));
		m_counter(); // Update FPS
	}
}




ColorTracker::ColorTracker(Webcam &webcam, int hue)
	: MotionTracker(webcam), m_hue(hue)
{ }

void ColorTracker::frameEvent(const cv::Mat& frame) {
	const int dH = 20;

	// Solve the position
	cv::Mat imgHSV, thresh;
	cv::cvtColor(frame, imgHSV, CV_BGR2HSV); // Switch to HSV color space
	cv::inRange(imgHSV, cv::Scalar(m_hue - dH, 120, 120), cv::Scalar(m_hue + dH, 255, 255), thresh);

	// Calculate the moments to estimate the position
	cv::Moments m = cv::moments(thresh, true);
	int x = m.m10 / m.m00;
	int y = m.m01 / m.m00;

	// Assign new values
	boost::mutex::scoped_lock l(m_mutex);
	m_pos = cv::Vec3f(x, y, 0);
	m_counter(); // Update FPS
}




ColorCrossTracker::ColorCrossTracker(Webcam &webcam, int solver)
	: MotionTracker(webcam), m_solver(solver)
{
	m_objectPoints.push_back(cv::Point3f(0,100,0)); // Blue
	m_objectPoints.push_back(cv::Point3f(0,-100,0)); // Red
	m_objectPoints.push_back(cv::Point3f(-100,0,0)); // Yellow
	m_objectPoints.push_back(cv::Point3f(100,0,0));	// Green
}

vector<cv::Point2f> ColorCrossTracker::getImagePoints() const {
	boost::mutex::scoped_lock l(m_mutex);
	return m_savedImagePoints;
}

void ColorCrossTracker::frameEvent(const cv::Mat& frame) {
	// Solve image points
	m_imagePoints.clear();
	cv::Mat imgHSV;
	cv::cvtColor(frame, imgHSV, CV_BGR2HSV); // Switch to HSV color space

	calculateImagePoint(imgHSV, 100); // Blue
	calculateImagePoint(imgHSV, 170); // Red
	calculateImagePoint(imgHSV, 25); // Yellow
	calculateImagePoint(imgHSV, 50); // Green

	// Solve pose
	if (m_solver == 1) solvePnP();
	else if (m_solver == 2) solvePOSIT();
	else throw std::runtime_error("Bad solver type in ColorCrossTracker");

	m_counter(); // Update FPS
}

void ColorCrossTracker::calculateImagePoint(const cv::Mat& frame, int hue) {
	const int dH = 10; // How much hue can vary to be accepted

	// Threshold
	cv::Mat thresh;
	cv::inRange(frame, cv::Scalar(hue - dH, 120, 120), cv::Scalar(hue + dH, 255, 255), thresh);

	// Calculate the moments to estimate the position
	cv::Moments m = cv::moments(thresh, true);
	int x = m.m10 / m.m00;
	int y = m.m01 / m.m00;

	// Save point
	m_imagePoints.push_back(cv::Point2f(x,y));
}

void ColorCrossTracker::solvePnP() {
	// TODO

	// Assign new values
	boost::mutex::scoped_lock l(m_mutex);
//	m_pos = cv::Vec3f(x, y, 0);
}

void ColorCrossTracker::solvePOSIT() {
	int x = 0, y = 0;
	for (int i = 0; i < 4; ++i) {
		x += m_imagePoints[i].x;
		y += m_imagePoints[i].y;
	}
	x /= 4;
	y /= 4;

	// TODO

	// Assign new values
	boost::mutex::scoped_lock l(m_mutex);
	m_pos = cv::Vec3f(x, y, 0);
	m_savedImagePoints = m_imagePoints;
}
