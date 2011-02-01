#include "motiontracker.hh"


MotionTracker::MotionTracker(Webcam &webcam, const CameraParameters &camparams)
	: m_webcam(webcam), m_thread(), m_quit(false)
{
	// TODO
	(void)camparams;
}

MotionTracker::~MotionTracker() {
	m_quit = true;
	if (m_thread) m_thread->join();
}

cv::Vec3f MotionTracker::getOrientation() {
	// TODO
	return cv::Vec3f();
}

cv::Vec3f MotionTracker::getPosition() {
	// TODO
	return cv::Vec3f();
}
