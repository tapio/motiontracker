#include <iostream>
#include "motiontracker.hh"
#include "utils.hh"

MotionTracker::MotionTracker(Webcam &webcam, const CameraParameters &camparams)
	: m_webcam(webcam), m_thread(), m_quit(false), m_pos(), m_rot(), m_fps()
{
	// TODO
	(void)camparams;

	// Start thread
	m_thread.reset(new boost::thread(boost::ref(*this)));
}

MotionTracker::~MotionTracker() {
	m_quit = true;
	if (m_thread) m_thread->join();
}

void MotionTracker::operator()() {
	FPSCounter counter(5);
	cv::Mat frame;
	cv::Vec3f pos, rot;
	while (!m_quit) {
		try {
			// Get a new frame
			m_webcam >> frame;

			// TODO: Do stuff with frame

			// Assign new variables
			boost::mutex::scoped_lock l(m_mutex);
			m_pos = pos;
			m_rot = rot;
		} catch (std::exception&) { std::cerr << "Error capturing webcam frame!" << std::endl; }
		counter();
		boost::mutex::scoped_lock l(m_fpsmutex);
		m_fps = counter.getFPS();
	}
}

cv::Vec3f MotionTracker::getOrientation() const {
	boost::mutex::scoped_lock l(m_mutex);
	return m_rot;
}

cv::Vec3f MotionTracker::getPosition() const {
	boost::mutex::scoped_lock l(m_mutex);
	return m_pos;
}

int MotionTracker::getFPS() const {
	boost::mutex::scoped_lock l(m_fpsmutex);
	return m_fps;
}
