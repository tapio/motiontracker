#include <iostream>
#include <boost/shared_ptr.hpp>
#include <highgui.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "motiontracker.hh"
#include "utils.hh"

namespace {
	class MyWebcamListener;

	static const int ESCAPE = 27; ///< Keycode for Escape
	const char* camwindow = "webcam"; ///< OpenCV window id/title
	int window; ///< GLUT window id
	boost::shared_ptr<Webcam> webcam; ///< Webcam
	boost::shared_ptr<MyWebcamListener> camlistener; ///< OpenCV webcam visualizer
	boost::shared_ptr<ChessboardTracker> tracker; ///< The tracker

	/// Minimal class for displaying webcam video.
	// FIXME: Somehow window is not coming up
	struct MyWebcamListener: public WebcamListener {
		MyWebcamListener(Webcam& webcam): WebcamListener(webcam) { cv::namedWindow(camwindow); }
		~MyWebcamListener() { cv::destroyWindow(camwindow); }
		void frameEvent(const cv::Mat &frame) { /*cv::imshow(camwindow, frame);*/ }
	};
}

void resizeWindow(int w, int h)
{
	if (h == 0) h = 1; // Prevent div-by-zero
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void initGL(int w, int h)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	resizeWindow(w, h);
}

void drawScene()
{
	// Yeah, this is horrible OpenGL 1 stuff,
	// it suits the scope of this example.

	glClear(GL_COLOR_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();

	glLoadIdentity();

	// Position
	cv::Vec3f pos = tracker->getRotation();
	float x = pos[0] * 1;
	float y = pos[1] * 1;
	float z = pos[2] * 1 - 7.0f;
	//glTranslatef(x, y, z); // FIXME: Translate disabled
	glTranslatef(0.0f, 0.0f, -7.0f);
	std::cout << "Raw pos: " << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;

	// Rotation
	// FIXME: Bad :(
	cv::Vec3f rot = tracker->getRotation();
	float magnitude = cv::norm(rot);
	float sum = rot[0] + rot[1] + rot[2];
	float rx = rot[0] / sum;
	float ry = rot[1] / sum;
	float rz = rot[2] / sum;
	glRotatef(magnitude * 6, rx, ry, rz); // FIXME: Arbitrary multiplier
	std::cout << "Rot: " << magnitude << " " << rx << " " << ry << " " << rz << std::endl;

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

	glutSwapBuffers(); // Double buffering
}

void keyPressed(unsigned char key, int, int)
{
	// Quit on Escape
	if (key == ESCAPE) {
		glutDestroyWindow(window);
		camlistener.reset();
		tracker.reset();
		webcam.reset();
		exit(0); // Exit
	}
}

int main(int argc, char **argv)
{
	try {
		webcam.reset(new Webcam);
		tracker.reset(new ChessboardTracker(*webcam, CameraParameters::fromFile("calibration.xml")));
		camlistener.reset(new MyWebcamListener(*webcam));
	} catch (std::exception& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	window = glutCreateWindow("Chessboard Tracker");

	// Register functions
	glutDisplayFunc(&drawScene);
	glutIdleFunc(&drawScene);
	glutReshapeFunc(&resizeWindow);
	glutKeyboardFunc(&keyPressed);

	initGL(640, 480);

	glutMainLoop();

	return 1;
}
