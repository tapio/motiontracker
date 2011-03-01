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
	/// Constructor
	PyMotionTracker(/*TODO: params*/);
	
	/// Returns position
	boost::python::tuple getPosition() const;
	
	/// Returns rotation
	boost::python::tuple getRotation() const;

	Webcam* webcam; ///< Webcam object
	ColorCrossTracker* tracker; ///< Tracker object
};
