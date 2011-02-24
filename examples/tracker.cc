#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <boost/lexical_cast.hpp>
#include "motiontracker.hh"
#include "utils.hh"

using namespace cv;

struct MyTracker: public WebcamListener, public MotionListener {
	ColorCrossTracker* ctr;
	std::string window;
	FPSCounter counter;


	volatile int x;
	volatile int y;

	MyTracker(Webcam& cam, MotionTracker& tr, ColorCrossTracker* ctr, std::string win)
		: WebcamListener(cam), MotionListener(tr), ctr(ctr), window(win), counter(5), x(), y()
	{ }

	void motionEvent(cv::Vec3f pos, cv::Vec3f) {
		x = pos[0];
		y = pos[1];
	}

	void frameEvent(const cv::Mat& frame) {
		Mat img(frame);
		std::vector<Point2f> imagePoints = ctr->getImagePoints();

		// Draw circles to detected tips
		for (size_t i = 0; i < imagePoints.size(); ++i) {
			circle(img,imagePoints[i],3,Scalar(255,0,255),3);
		}

		// Draw reprojected points
		std::vector<Point2f> projectedPoints = ctr->getProjectedPoints();

		for (size_t i = 0; i < projectedPoints.size(); ++i) {
			if (i > 0)
				line(img, projectedPoints[0], projectedPoints[i], CV_RGB(255, 255, 255),2);
			circle(img,projectedPoints[i],3,Scalar(0,255,255),3);
		}

		// Print FPS indicator and position + rotation vector components
		putText(img, boost::lexical_cast<std::string>(counter.getFPS()),
			Point(0, 475), FONT_HERSHEY_PLAIN, 1, CV_RGB(255,0,255));

		std::string loc, rotv;
		loc = "Position vector: x: " + boost::lexical_cast<std::string>((int)ctr->getPosition()[0]) + ' '
			  + "y: " + boost::lexical_cast<std::string>((int)ctr->getPosition()[1]) + ' '
			  + "z: " + boost::lexical_cast<std::string>((int)ctr->getPosition()[2]);
		putText(img, loc,
			Point(0,455), FONT_HERSHEY_PLAIN, 1, CV_RGB(255,0,255));

		rotv = "Rotation vector: x: " + boost::lexical_cast<std::string>((int)ctr->getRotation()[0]) + ' '
			  + "y: " + boost::lexical_cast<std::string>((int)ctr->getRotation()[1]) + ' '
			  + "z: " + boost::lexical_cast<std::string>((int)ctr->getRotation()[2]) + ' '
			  + "mag: " + boost::lexical_cast<std::string>((int)(norm(ctr->getRotation())*180/3.1415));
		putText(img, rotv,
			Point(0,435), FONT_HERSHEY_PLAIN, 1, CV_RGB(255,0,255));

		// Show on screen
		//cvtColor(img, img, CV_BGR2HSV); // Switch to HSV color space
		imshow(window, img);
		counter(); // Update FPS counter
	}
};

int main(int argc, char** argv)
{
	(void)argc; (void)argv; // Suppress warnings
	boost::scoped_ptr<Webcam> webcam;
	boost::scoped_ptr<ColorCrossTracker> tracker;

	try {
		webcam.reset(new Webcam);
		tracker.reset(new ColorCrossTracker(*webcam,2));
	} catch (std::exception const &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}

	std::string window = "video";
	namedWindow(window, 1);

	{
		// Launch a receiver for doing the work whenever a frame is available
		MyTracker mytracker(*webcam, *tracker, tracker.get(), window);

		// Rest here
		while (waitKey(30) < 0);
	}

	cvDestroyWindow(window.c_str());
	return 0;
}
