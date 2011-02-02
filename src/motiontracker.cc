#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <highgui.h>
#include "motiontracker.hh"

using namespace cv;

MotionTracker::MotionTracker(Webcam &webcam, const CameraParameters &camParams)
	: WebcamListener(webcam), m_camParams(camParams), m_pos(), m_rot()
{ }

cv::Vec3f MotionTracker::getRotation() const {
	boost::mutex::scoped_lock l(m_mutex);
	return m_rot;
}

cv::Vec3f MotionTracker::getPosition() const {
	boost::mutex::scoped_lock l(m_mutex);
	return m_pos;
}



ChessboardTracker::ChessboardTracker(Webcam &webcam, const CameraParameters &camParams, const std::string& win)
	: MotionTracker(webcam, camParams), m_window(win), m_counter(5), m_boardScaleFactor(25), m_boardH(9), m_numCorners(6*9), m_boardSize(Size(6,9))
{
	for (int i = 0; i < m_numCorners; ++i)
		m_objectCorners.push_back(Point3f(m_boardScaleFactor*(i / m_boardH), m_boardScaleFactor*(i % m_boardH), 0.0f));
}

void ChessboardTracker::frameEvent(const cv::Mat& frame) {
	cv::Vec3f pos, rot;

	bool patternFound = findChessboardCorners(frame, m_boardSize, m_corners, CALIB_CB_FAST_CHECK);
	Mat img = frame;
	drawChessboardCorners(img, m_boardSize, m_corners, patternFound);

	if (patternFound && (int)m_corners.size() == m_numCorners) {
		solvePnP(Mat(m_objectCorners), Mat(m_corners), m_camParams.intrinsic_parameters, m_camParams.distortion_coeffs, m_rvec, m_tvec, false);
		std::string loc =
			"x: " + boost::lexical_cast<std::string>((int)m_tvec.at<double>(0,0)) + ' '
			+ "y: " + boost::lexical_cast<std::string>((int)m_tvec.at<double>(0,1)) + ' '
			+ "z: " + boost::lexical_cast<std::string>((int)m_tvec.at<double>(0,2));
		putText(img, loc,
			Point(0,60), FONT_HERSHEY_PLAIN, 1, CV_RGB(255,0,255));
	}

	putText(img, boost::lexical_cast<std::string>(m_counter.getFPS()),
		Point(0,30), FONT_HERSHEY_PLAIN, 2, CV_RGB(255,0,255));

	imshow(m_window, img);
	m_counter();

	// Assign new variables
	// TODO
	boost::mutex::scoped_lock l(m_mutex);
	m_pos = pos;
	m_rot = rot;
}
