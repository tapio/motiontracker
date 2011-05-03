/**
 * @file tracker.cc
 * @brief Main demo program.
 *
 * Detects the reference object from the webcam video and calculates its pose.
 * Validation of the calculation is done by overlay.
 */

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <boost/lexical_cast.hpp>
#include "motiontracker.hh"
#include "utils.hh"

using namespace cv;

/**
 * @brief Listener class implementation.
 */
struct MyTracker: public WebcamListener {

	ColorCrossTracker* ctr; ///< Pointer to the tracker instance
	std::string window; ///< Window title / OpenCV id
	FPSCounter counter; ///< FPS counter

	/// Constructor
	/// @param cam reference to a webcam object
	/// @param ctr pointer to a tracker object
	/// @param win OpenCV window name
	MyTracker(Webcam& cam, ColorCrossTracker* ctr, std::string win)
		: WebcamListener(cam), ctr(ctr), window(win), counter(5)
	{ }

	/// Receives frames.
	/// @param frame the frame
	void frameEvent(const cv::Mat& frame) {
		Mat img(frame);
		std::vector<Point2f> imagePoints = ctr->getImagePoints();

		// Draw circles to detected tips with magenta
		for (size_t i = 0; i < imagePoints.size(); ++i) {
			circle(img,imagePoints[i],3,Scalar(255,0,255),3);
		}

		// Draw reprojected points with yellow color
		std::vector<Point2f> projectedPoints = ctr->getProjectedPoints();

		for (size_t i = 0; i < projectedPoints.size(); ++i) {
			if (i > 0)
				line(img, projectedPoints[0], projectedPoints[i], CV_RGB(255, 255, 255),2);
			circle(img, projectedPoints[i], 3, Scalar(0,255,255), 3);
		}

		// Print FPS indicator and position + rotation vector components
		putText(img, boost::lexical_cast<std::string>(counter.getFPS()),
			Point(0, 475), FONT_HERSHEY_PLAIN, 1, CV_RGB(255,0,255));

		#ifdef PRINT_ROTLOC
		std::string loc, rotv;
		loc = "Position vector: x: " + boost::lexical_cast<std::string>((int)ctr->getPosition()[0]) + ' '
			  + "y: " + boost::lexical_cast<std::string>((int)ctr->getPosition()[1]) + ' '
			  + "z: " + boost::lexical_cast<std::string>((int)ctr->getPosition()[2]);
		putText(img, loc,
			Point(0,455), FONT_HERSHEY_PLAIN, 1, CV_RGB(255,0,255));

		rotv = "Rotation vector: x: " + boost::lexical_cast<std::string>((float)ctr->getRotation()[0]) + ' '
			  + "y: " + boost::lexical_cast<std::string>((float)ctr->getRotation()[1]) + ' '
			  + "z: " + boost::lexical_cast<std::string>((float)ctr->getRotation()[2]) + ' '
			  + "mag: " + boost::lexical_cast<std::string>((float)(norm(ctr->getRotation())*180/3.1415));
		putText(img, rotv,
			Point(0,435), FONT_HERSHEY_PLAIN, 1, CV_RGB(255,0,255));
		#endif // PRINT_LOCROT

		// Show on screen
		imshow(window, img);
		counter(); // Update FPS counter
	}
};

/// Main
int main(int argc, char** argv)
{
	(void)argc; (void)argv; // Suppress warnings
	boost::scoped_ptr<Webcam> webcam;
	boost::scoped_ptr<ColorCrossTracker> tracker;

	try {
		CalibrationParameters calibParams = CalibrationParameters::fromFile("calibration.xml");
		webcam.reset(new Webcam);
		tracker.reset(new ColorCrossTracker(*webcam, calibParams, 2));
	} catch (std::exception const &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::string window = "video";
	namedWindow(window, 1);

	{
		// Launch a receiver for doing the work whenever a frame is available
		MyTracker mytracker(*webcam, tracker.get(), window);

		// Rest here
		while (waitKey(30) < 0);
	}

	cvDestroyWindow(window.c_str());
	return EXIT_SUCCESS;
}
