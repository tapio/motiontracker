#include <iostream>
#include <boost/thread/thread.hpp>
#include "motiontracker.hh"
#include "utils.hh"

MotionTracker::MotionTracker(Webcam &webcam, const CameraParameters &camparams)
	: FrameReceiver(webcam), m_pos(), m_rot()
{
	// TODO: Do stuff with camparams
	(void)camparams;
}

void MotionTracker::frameEvent(const cv::Mat& frame) {
	// TODO: Do stuff with frame
	(void)frame;
	cv::Vec3f pos, rot;

	// Assign new variables
	boost::mutex::scoped_lock l(m_mutex);
	m_pos = pos;
	m_rot = rot;
}

cv::Vec3f MotionTracker::getOrientation() const {
	boost::mutex::scoped_lock l(m_mutex);
	return m_rot;
}

cv::Vec3f MotionTracker::getPosition() const {
	boost::mutex::scoped_lock l(m_mutex);
	return m_pos;
}
