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
