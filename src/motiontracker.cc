#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>
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



ChessboardTracker::ChessboardTracker(Webcam &webcam, const CameraParameters &camParams)
	: MotionTracker(webcam, camParams), m_counter(5), m_boardScaleFactor(25), m_boardH(9), m_numCorners(6*9), m_boardSize(Size(6,9))
{
	for (int i = 0; i < m_numCorners; ++i)
		m_objectCorners.push_back(Point3f(m_boardScaleFactor*(i / m_boardH), m_boardScaleFactor*(i % m_boardH), 0.0f));
}

void ChessboardTracker::frameEvent(const cv::Mat& frame) {
	cv::Vec3f pos, rot;

	bool patternFound = findChessboardCorners(frame, m_boardSize, m_corners, CALIB_CB_FAST_CHECK);
	Mat img = frame;

	if (patternFound && (int)m_corners.size() == m_numCorners) {
		solvePnP(Mat(m_objectCorners), Mat(m_corners), m_camParams.intrinsic_parameters, m_camParams.distortion_coeffs, m_rvec, m_tvec, false);
	}

	m_counter();

	// Assign new variables
	// TODO
	boost::mutex::scoped_lock l(m_mutex);
	m_pos = pos;
	m_rot = rot;
}
