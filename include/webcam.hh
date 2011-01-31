#pragma once

#include <cv.h>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <vector>

namespace cv {
	// Forward declarations
	class VideoCapture;
	class VideoWriter;
}

struct CamFrame {
	int width;
	int height;
	std::vector<boost::uint8_t> data;
};

class Webcam {
  public:
	/// cam_id -1 means pick any device
	Webcam(int cam_id = -1);

	~Webcam();

	/// Thread runs here, don't call directly
	void operator()();

	Webcam& operator>>(cv::Mat& rhs);
	
	/// Get frame rate
	int getFPS() const;
	/// Capture failed to initialize?
	bool isNull() { return m_capture == 0; }
	/// When paused, does not get or render frames
	void pause(bool do_pause = true);
	/// Display frame
	void render();

  private:
	boost::scoped_ptr<boost::thread> m_thread;
	mutable boost::mutex m_mutex;
	mutable boost::mutex m_fpsmutex;
	boost::scoped_ptr<cv::VideoCapture> m_capture;
	boost::scoped_ptr<cv::VideoWriter> m_writer;
	cv::Mat m_latestFrame;
	cv::Mat m_displayFrame;
	CamFrame m_frame;
	bool m_frameAvailable;
	int m_fps;
	volatile bool m_running;
	volatile bool m_quit;
};
