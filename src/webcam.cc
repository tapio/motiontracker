#include <iostream>
#include <stdexcept>
#include <highgui.h>
#include <boost/lexical_cast.hpp>

#include "webcam.hh"
#include "utils.hh"

Webcam::Webcam(int cam_id):
  m_thread(), m_capture(), m_writer(), m_latestFrame(), m_displayFrame(), m_frameAvailable(false), m_running(false), m_quit(false)
  {
	// Initialize the capture device
	m_capture.reset(new cv::VideoCapture(cam_id));
	if (!m_capture->isOpened()) {
		if (cam_id != -1) {
			std::clog << "Webcam/warning: Webcam id " << cam_id << " failed, trying autodetecting...";
			m_capture.reset(new cv::VideoCapture(-1));
		}
		if (!m_capture->isOpened())
			throw std::runtime_error("Could not initialize webcam capturing!");
	}
	// Initialize the video writer
	#ifdef SAVE_WEBCAM_VIDEO
	float fps = m_capture->get(CV_CAP_PROP_FPS);
	int framew = m_capture->get(CV_CAP_PROP_FRAME_WIDTH);
	int frameh = m_capture->get(CV_CAP_PROP_FRAME_HEIGHT);
	int codec = CV_FOURCC('P','I','M','1'); // MPEG-1
	std::string out_file = "webcam_out.mpg";
	m_writer.reset(new cv::VideoWriter(out_file.c_str(), codec, fps > 0 ? fps : 30.0f, cvSize(framew,frameh)));
	if (!m_writer->isOpened()) {
		std::cout << "Could not initialize webcam video saving!" << std::endl;
		m_writer.reset();
	}
	#endif
	// Start thread
	m_thread.reset(new boost::thread(boost::ref(*this)));
}

Webcam::~Webcam() {
	m_quit = true;
	if (m_thread) m_thread->join();
}

void Webcam::operator()() {
	m_running = true;
	FPSCounter counter(5);
	while (!m_quit) {
		if (m_running) {
			try {
				// Get a new frame
				cv::Mat newFrame;
				*m_capture >> newFrame;
				if (m_writer) *m_writer << newFrame;
				boost::mutex::scoped_lock l(m_mutex);
				m_latestFrame = newFrame;
				// Notify renderer
				m_frameAvailable = true;
			} catch (std::exception&) { std::cerr << "Error capturing webcam frame!" << std::endl; }
			counter();
			boost::mutex::scoped_lock l(m_fpsmutex);
			m_fps = counter.getFPS();
		}
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

void Webcam::pause(bool do_pause) {
	boost::mutex::scoped_lock l(m_mutex);
	m_running = !do_pause;
	m_frameAvailable = false;
}

void Webcam::render() {
	if (!m_capture || !m_running) return;
	// Do we have a new frame available?
	if (m_frameAvailable) {
		boost::mutex::scoped_lock l(m_mutex);
		// Load the image
		m_displayFrame = m_latestFrame;
		m_frameAvailable = false;
		//XXX.load(m_frame.width, m_frame.height, pix::BGR, &m_frame.data[0]);
	}
	try {
		if (!m_displayFrame.empty())
			imshow("video", m_displayFrame);
	} catch (...) {}
}
