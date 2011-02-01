#include <cv.h>


class Webcam;


typedef void* DummyType; // FIXME

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


