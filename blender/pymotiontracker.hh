#pragma once
#include <boost/python.hpp>

class Webcam;
class ColorCrossTracker;

/**
 * @brief Class exposed to Python
 * 
 * This class serves as a simplified C++ to Python interface.
 */
struct PyMotionTracker
{
	PyMotionTracker(/*TODO: params*/);
	
	boost::python::tuple getPosition() const;
	
	boost::python::tuple getRotation() const;

	Webcam* webcam;
	ColorCrossTracker* tracker;
};
