#include <iostream>
#include <stdexcept>
#include <boost/thread/thread.hpp>
#include <highgui.h>

#include "motiontracker.hh"
#include "utils.hh"

Webcam::Webcam(int device):
  m_thread(), m_capture(), m_quit(false), m_latestFrame(), m_frameIndex()
  {
	// Initialize the capture device
	m_capture.reset(new cv::VideoCapture(device));
	if (!m_capture->isOpened()) {
		if (device != -1) {
			std::cout << "Webcam id " << device << " failed, trying autodetecting...";
			m_capture.reset(new cv::VideoCapture(-1));
		}
		if (!m_capture->isOpened())
			throw std::runtime_error("Could not initialize webcam capturing!");
	}

	// Start thread
	m_thread.reset(new boost::thread(boost::ref(*this)));
}

Webcam::~Webcam() {
	m_quit = true;
	if (m_thread) m_thread->join();
}

void Webcam::operator()() {
	FPSCounter counter(5);
	while (!m_quit) {
		try {
			// Get a new frame
			cv::Mat newFrame;
			*m_capture >> newFrame;
			boost::mutex::scoped_lock l(m_mutex);
			m_latestFrame = newFrame;
			++m_frameIndex;
		} catch (std::exception&) { std::cerr << "Error capturing webcam frame!" << std::endl; }
		counter();
		boost::mutex::scoped_lock l(m_fpsmutex);
		m_fps = counter.getFPS();
	}
}

Webcam& Webcam::operator>>(cv::Mat& rhs) {
	boost::mutex::scoped_lock l(m_mutex);
	rhs = m_latestFrame;
	return *this;
}

int Webcam::getFPS() const {
	boost::mutex::scoped_lock l(m_fpsmutex);
	return m_fps;
}

unsigned Webcam::getFrameIndex() const {
	boost::mutex::scoped_lock l(m_mutex);
	return m_frameIndex;
}
