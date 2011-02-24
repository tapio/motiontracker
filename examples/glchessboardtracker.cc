#include <iostream>
#include <boost/shared_ptr.hpp>
#include <highgui.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "motiontracker.hh"
#include "utils.hh"

namespace {
	class MyWebcamListener;

	const char* window = "webcam"; ///< OpenCV window id/title
	boost::shared_ptr<Webcam> webcam; ///< Webcam
	boost::shared_ptr<MyWebcamListener> camlistener; ///< OpenCV webcam visualizer
	boost::shared_ptr<ColorCrossTracker> tracker; ///< The tracker

	/// Minimal class for displaying webcam video.
	struct MyWebcamListener: public WebcamListener {
		MyWebcamListener(Webcam& webcam): WebcamListener(webcam) { }
		void frameEvent(const cv::Mat &frame) { cv::imshow(window, frame); }
	};
}

void drawScene(void*)
{
	// Yeah, this is horrible OpenGL 1 stuff,
	// but it suits the scope of this example.

	std::cout << "Tracker FPS: " << tracker->getFPS() << std::endl;

	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT); // Clear The Screen And The Depth Buffer
	glLoadIdentity();

	// Position
	cv::Vec3f pos = tracker->getPosition();
	// FIXME: Arbitrary factors
	float x = pos[0] * 0.005;
	float y = pos[1] * 0.005;
	float z = pos[2] * 0.0005 - 7.0f;
	glTranslatef(x, y, z);
	//glTranslatef(0.0f, 0.0f, -7.0f);
	std::cout << "Raw pos: " << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;

	// Rotation
	// FIXME: Bad :(
	cv::Vec3f rot = tracker->getRotation();
	float magnitude = cv::norm(rot);
	float sum = rot[0] + rot[1] + rot[2];
	float rx = 0; //rot[0] / sum;
	float ry = 0; //rot[1] / sum;
	float rz = 0; //rot[2] / sum;
	glRotatef(magnitude * 6, rx, ry, rz); // FIXME: Arbitrary multiplier
	//std::cout << "Rot: " << magnitude << " " << rx << " " << ry << " " << rz << std::endl;

	// Draw a cube
	glBegin(GL_QUADS);
		// top of cube
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f( 1.0f, 1.0f,-1.0f);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		// bottom of cube
		glColor3f(1.0f,0.5f,0.0f);
		glVertex3f( 1.0f,-1.0f, 1.0f);
		glVertex3f(-1.0f,-1.0f, 1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glVertex3f( 1.0f,-1.0f,-1.0f);
		// front of cube
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f,-1.0f, 1.0f);
		glVertex3f( 1.0f,-1.0f, 1.0f);
		// back of cube.
		glColor3f(1.0f,1.0f,0.0f);
		glVertex3f( 1.0f,-1.0f,-1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glVertex3f( 1.0f, 1.0f,-1.0f);
		// left of cube
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f,-1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glVertex3f(-1.0f,-1.0f, 1.0f);
		// Right of cube
		glColor3f(1.0f,0.0f,1.0f);
		glVertex3f( 1.0f, 1.0f,-1.0f);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		glVertex3f( 1.0f,-1.0f, 1.0f);
		glVertex3f( 1.0f,-1.0f,-1.0f);
	glEnd();

}

int main(int argc, char **argv)
{
	(void)argc; (void)argv; // Suppress warnings
	try {
		webcam.reset(new Webcam);
		tracker.reset(new ColorCrossTracker(*webcam, 2));
		camlistener.reset(new MyWebcamListener(*webcam));
	} catch (std::exception& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}

	cv::namedWindow(window);

	// Register callback
	cv::createOpenGLCallback(window, &drawScene);

	// Rest here
	while (cv::waitKey(30) < 0);

	// Clean-up
	camlistener.reset();
	tracker.reset();
	webcam.reset();
	cv::destroyWindow(window);

	return 0;
}
