#include <boost/thread/thread.hpp>
#include "motiontracker.hh"

FrameReceiver::FrameReceiver(Webcam &webcam)
	: m_webcam(webcam), m_thread(), m_quit(false)
{
	// Start thread
	m_thread.reset(new boost::thread(boost::ref(*this)));
}

FrameReceiver::~FrameReceiver() {
	m_quit = true;
	if (m_thread) m_thread->join();
}

void FrameReceiver::operator()() {
	unsigned frameId = 0;
	cv::Mat frame;
	while (!m_quit) {
		// Check for new frame
		if (m_webcam.getFrameIndex() > frameId) {
			m_webcam >> frame;
			// Push it
			if (!frame.empty())
				frameEvent(frame);
		}
	}
}




MotionReceiver::MotionReceiver(MotionTracker &motiontracker)
	: m_motionTracker(motiontracker), m_thread(), m_quit(false)
{
	// Start thread
	m_thread.reset(new boost::thread(boost::ref(*this)));
}

MotionReceiver::~MotionReceiver() {
	m_quit = true;
	if (m_thread) m_thread->join();
}

void MotionReceiver::operator()() {
	cv::Vec3f pos, rot, prevpos, prevrot;
	while (!m_quit) {
		prevpos = pos; prevrot = rot;
		pos = m_motionTracker.getPosition();
		rot = m_motionTracker.getOrientation();
		// Check for change
		if (pos != prevpos || rot != prevrot) {
			motionEvent(pos, rot);
		}
	}
}
