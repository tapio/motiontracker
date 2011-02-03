#include <boost/thread/thread.hpp>
#include "motiontracker.hh"

WebcamListener::WebcamListener(Webcam &webcam)
	: m_webcam(webcam), m_thread(), m_quit(false)
{
	// Start thread
	m_thread.reset(new boost::thread(boost::ref(*this)));
}

WebcamListener::~WebcamListener() {
	m_quit = true;
	if (m_thread) m_thread->join();
}

void WebcamListener::operator()() {
	unsigned frameId = 0;
	cv::Mat frame;
	while (!m_quit) {
		// Check for new frame
		unsigned i = m_webcam.getFrameIndex();
		if (i > frameId) {
			m_webcam >> frame;
			// Push it
			if (!frame.empty())
				frameEvent(frame);
			frameId = i;
		}
	}
}




MotionListener::MotionListener(MotionTracker &motiontracker)
	: m_motionTracker(motiontracker), m_thread(), m_quit(false)
{
	// Start thread
	m_thread.reset(new boost::thread(boost::ref(*this)));
}

MotionListener::~MotionListener() {
	m_quit = true;
	if (m_thread) m_thread->join();
}

void MotionListener::operator()() {
	cv::Vec3f pos, rot, prevpos, prevrot;
	while (!m_quit) {
		prevpos = pos; prevrot = rot;
		pos = m_motionTracker.getPosition();
		rot = m_motionTracker.getRotation();
		// Check for change
		if (pos != prevpos || rot != prevrot) {
			motionEvent(pos, rot);
		}
	}
}
