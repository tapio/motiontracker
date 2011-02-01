#pragma once

#include <cv.h>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>


class Webcam;
namespace cv {
	// Forward declarations
	class VideoCapture;
	class VideoWriter;
}

typedef void* DummyType; // FIXME


/**
 * This class opens a webcam and gets frames from it.
 * TODO: This class needs clean-up and commenting
 */
class Webcam
{
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
	bool m_frameAvailable;
	int m_fps;
	volatile bool m_running;
	volatile bool m_quit;
};



/**
 * This class calculates object orientation and position from
 * a webcam video.
 */
class MotionTracker
{
public:
	/**
	 * Constructor.
	 * @param webcam reference to a valid webcam for getting video
	 */
	MotionTracker(Webcam &webcam /* Calibration parameters */);
	
	/** Destructor. */
	~MotionTracker();

	/**
	 * Returns the current orientation of the tracked object.
	 * @return the orientation
	 */
	DummyType getOrientation();

	/**
	 * Returns the current center position of the tracked object.
	 * @return the position
	 */
	DummyType getPosition();

private:
	Webcam &m_webcam;
};



/**
 * This abstract class recieves frames from a webcam.
 */
struct FrameReciever
{
	/**
	 * Constructor launches the listener thread.
	 * @param webcam reference to a valid webcam for getting video
	 */
	FrameReciever(Webcam &webcam);
	
	/** Destructor kills the listener thread. */
	~FrameReciever();

	/** Thread runs here. Don't call directly. */
	void operator()();

	/**
	 * This function is called every time a new frame is available.
	 * @param cv::Mat mat the frame in an OpenCV matrix
	 */
	virtual void frameEvent(cv::Mat mat);


	/** Reference to the Webcam object used for polling frames. */
	Webcam &webcam;
};



/**
 * This abstract class recieves motion events.
 */
struct MotionReciever
{
	/**
	 * Constructor launches the listener thread.
	 */
	MotionReciever(MotionTracker &motiontracker);
	
	/** Destructor kills the listener thread. */
	~MotionReciever();

	/** Thread runs here. Don't call directly. */
	void operator()();

	/**
	 * This function is called every time new motion happens.
	 */
	virtual void motionEvent(/* Proper struct/vector/matrix parameter */);


	/** Reference to MotionTracker object used for polling motion events. */
	MotionTracker &motionTracker;
};


