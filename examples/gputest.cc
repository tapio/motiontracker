/**
 * @file gputest.cc
 * @brief Simple OpenCV GPU CUDA module test.
 *
 * Falls back to CPU if CUDA is not available.
 */

#include <iostream>
#include <cv.h>
#include <opencv2/gpu/gpu.hpp>
#include <highgui.h>
#include <boost/lexical_cast.hpp>
#include "motiontracker.hh"
#include "utils.hh"

using namespace cv;

/**
 * @brief Listener class implementation.
 */
struct MyWebcamReceiver: public WebcamListener {
	std::string window; /// Window title / OpenCV id
	FPSCounter counter; /// FPS counter
	bool use_gpu; /// Switch for GPU enabling

	MyWebcamReceiver(Webcam& webcam, std::string win)
		: WebcamListener(webcam), window(win), counter(5), use_gpu(cv::gpu::getCudaEnabledDeviceCount())
	{
		if (use_gpu) std::cout << "GPU acceleration enabled." << std::endl;
		else std::cout << "GPU acceleration disabled." << std::endl;
	}

	/// Receives frames.
	/// @param frame the frame
	void frameEvent(const Mat &frame) {
		Mat img;
		Timer t;
		if (use_gpu) {
			// We need intermediate matrices with GPU
			gpu::GpuMat gpuframe(frame), gray, blur, binary;
			gpu::cvtColor(gpuframe, gray, CV_BGR2GRAY); // Switch to grayscale
			gpu::GaussianBlur(gray, blur, Size(15,15), 1.5, 1.5); // Blur to reduce noise
			gpu::threshold(blur, binary, 100, 255, THRESH_BINARY); // Threshold
			img = (Mat)binary;
		} else {
			cvtColor(frame, img, CV_BGR2GRAY); // Switch to grayscale
			GaussianBlur(img, img, Size(15,15), 1.5, 1.5); // Blur to reduce noise
			threshold(img, img, 100, 255, THRESH_BINARY); // Threshold
		}
		// Add FPS indicator
		putText(img, "FPS: " + boost::lexical_cast<std::string>(counter.getFPS())
			+ "   Algo FPS: " + boost::lexical_cast<std::string>(int(1.0/t.interval())),
			Point(0,30), FONT_HERSHEY_PLAIN, 2, CV_RGB(255,0,255));
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
	try {
		webcam.reset(new Webcam);
	} catch (std::exception& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}

	std::string window = "video";
	namedWindow(window, 1);

	{
		// Launch a receiver for doing the work whenever a frame is available
		MyWebcamReceiver video(*webcam, window);

		// Rest here
		while (waitKey(30) < 0);
	}

	cvDestroyWindow(window.c_str());
	return 0;
}

