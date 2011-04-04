#pragma once
#include <boost/python.hpp>

class Webcam;
class ColorCrossTracker;

/**
 * @brief Class exposed to Python
 * 
 * This class serves as a simplified C++ to Python interface.
 *
 * How to test this:
 *
 * - Compile and locate libPyMotionTracker.so / .dll
 * - Remove the lib prefix (e.g. mv libPyMotionTracker.so PyMotionTracker.so)
 * - Start python interpreter in the same directory: python
 * - Type into the python prompt:
 *   - >>> import PyMotionTracker
 *   - >>> print PyMotionTracker.greet()
 *   - >>> tracker = PyMotionTracker.PyMotionTracker()
 *   - >>> print tracker.getPosition()
 */
struct PyMotionTracker
{
	/// Constructor
	PyMotionTracker(/*TODO: params*/);

	/// Destructor
	~PyMotionTracker();

	/// Returns position
	boost::python::tuple getPosition() const;
	
	/// Returns rotation
	boost::python::tuple getRotation() const;

	Webcam* webcam; ///< Webcam object
	ColorCrossTracker* tracker; ///< Tracker object
};
