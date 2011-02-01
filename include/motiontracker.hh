#pragma once

#include <cv.h>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>

// Forward declarations
namespace cv {
	class VideoCapture;
}
namespace boost {
	class thread;
}

struct CameraParameters
{
	cv::Mat intrinsic_parameters; // Intrinsic parameters
	cv::Mat distortion_coeffs; // Distortion coefficients

	CameraParameters();
	CameraParameters(cv::Mat ip, cv::Mat dc);

	/**
	  * Saves the parameters to a configuration file.
	  * @param filename path to the file used for saving
	  */
	void saveToFile(std::string filename);

	/**
	  * Creates a new object by reading the parameters from a file.
	  * @param filename path to the file used for reading
	  * @return new object containing the read parameters
	  */
	static CameraParameters fromFile(std::string filename);
};



/**
 * This class opens a webcam and gets frames from it.
 */
class Webcam: public boost::noncopyable
{
public:
	/**
	 * Opens up a video capture device and starts recieving frames.
	 * @param device the capture deivce id, -1 means auto pick
	 */
	Webcam(int device = -1);

	/** Destructor closes device. */
	~Webcam();

	/** Thread runs here, don't call directly. */
	void operator()();

	/**
	 * Retrieve the latest frame.
	 * @param rhs the OpenCV matrix where the frame is stored
	 * @return a reference to the class it self for chaining
	 */
	Webcam& operator>>(cv::Mat& rhs);

	/**
	 * Get frame rate.
	 * @return frames per second
	 */
	int getFPS() const;

	/**
	  * Get the index number of the current frame.
	  * @return the index
	  */
	unsigned getFrameIndex() const;

	/**
	 * Check if the device is in zombie state.
	 * @return true if this class has no valid device
	 */
	bool isNull() { return m_capture == 0; }

private:
	boost::scoped_ptr<boost::thread> m_thread;
	boost::scoped_ptr<cv::VideoCapture> m_capture;
	mutable boost::mutex m_mutex;
	mutable boost::mutex m_fpsmutex;
	volatile bool m_quit;
	cv::Mat m_latestFrame;
	int m_fps;
	unsigned m_frameIndex;
};



/**
 * This abstract class receives frames from a webcam.
 */
struct WebcamListener: public boost::noncopyable
{
	/**
	 * Constructor launches the listener thread.
	 * @param webcam reference to a valid webcam for getting video
	 */
	WebcamListener(Webcam &webcam);

	/** Destructor kills the listener thread. */
	virtual ~WebcamListener();

	/** Thread runs here, don't call directly. */
	void operator()();

	/**
	 * This function is called every time a new frame is available.
	 * @param frame the frame in an OpenCV matrix
	 */
	virtual void frameEvent(const cv::Mat& frame) { (void)frame; }

private:

	Webcam &m_webcam; /// Reference to the Webcam object used for polling frames.
	boost::scoped_ptr<boost::thread> m_thread;; /// Receiver thread
	bool m_quit; /// Flag telling the receiver to quit
};



/**
 * This class calculates object orientation and position from
 * a webcam video.
 */
class MotionTracker: public WebcamListener
{
public:
	/**
	 * Constructor.
	 * @param webcam reference to a valid webcam for getting video
	 * @param camparams camera parameters from calibration
	 */
	MotionTracker(Webcam &webcam, const CameraParameters &camparams);

	/** Thread calls this, don't call directly. */
	void frameEvent(const cv::Mat& frame);

	/**
	 * Returns the current orientation of the tracked object.
	 * @return the orientation in a vector
	 */
	cv::Vec3f getRotation() const;

	/**
	 * Returns the current center position of the tracked object.
	 * @return the position in a vector
	 */
	cv::Vec3f getPosition() const;

private:
	mutable boost::mutex m_mutex;
	cv::Vec3f m_pos;
	cv::Vec3f m_rot;
};



/**
 * This abstract class receives motion events.
 */
struct MotionListener: public boost::noncopyable
{
	/**
	 * Constructor launches the listener thread.
	 */
	MotionListener(MotionTracker &motiontracker);
	
	/** Destructor kills the listener thread. */
	virtual ~MotionListener();

	/** Thread runs here, don't call directly. */
	void operator()();

	/**
	 * This function is called every time new motion happens.
	 * @param pos the position vector of the tracked object
	 * @param rot the orientation vector of the tracked object
	 */
	virtual void motionEvent(cv::Vec3f pos, cv::Vec3f rot) { (void)pos; (void)rot; }

private:

	MotionTracker &m_motionTracker; /// Reference to MotionTracker object used for polling motion events.
	boost::scoped_ptr<boost::thread> m_thread;; /// Receiver thread
	bool m_quit; /// Flag telling the receiver to quit
};


