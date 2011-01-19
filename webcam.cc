#include <iostream>
#include <stdexcept>

#include "webcam.hh"

#include <cv.h>
#include <highgui.h>

Webcam::Webcam(int cam_id):
  m_thread(), m_capture(), m_writer(), m_frameAvailable(false), m_running(false), m_quit(false)
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
	while (!m_quit) {
		if (m_running) {
			try {
				// Get a new frame
				cv::Mat frame;
				*m_capture >> frame;
				if (m_writer) *m_writer << frame;
				boost::mutex::scoped_lock l(m_mutex);
				// Copy the frame to storage
				m_frame.width = frame.cols;
				m_frame.height = frame.rows;
				m_frame.data.assign(frame.data, frame.data + (m_frame.width * m_frame.height * 3));
				// Notify renderer
				m_frameAvailable = true;
			} catch (std::exception&) { std::cerr << "Error capturing webcam frame!" << std::endl; }
		}
	}
}

void Webcam::pause(bool do_pause) {
	boost::mutex::scoped_lock l(m_mutex);
	m_running = !do_pause;
	m_frameAvailable = false;
}

void Webcam::render() {
	if (!m_capture || !m_running) return;
	// Do we have a new frame available?
	if (m_frameAvailable && !m_frame.data.empty()) {
		boost::mutex::scoped_lock l(m_mutex);
		// Load the image
		// TODO: load the frame somewhere for display
		//XXX.load(m_frame.width, m_frame.height, pix::BGR, &m_frame.data[0]);
		m_frameAvailable = false;
	}
	// TODO: Draw
}
