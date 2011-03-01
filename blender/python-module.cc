/**
 * How to test this:
 *
 * - Compile and locate libPyMotionTracker.so / .dll
 * - Remove the lib prefix (e.g. mv libPyMotionTracker.so PyMotionTracker.so)
 * - Start python interpreter in the same directory: python
 * - Type into the python prompt:
 *   >>> import PyMotionTracker
 *   >>> print PyMotionTracker.greet()
 *   >>> dd = PyMotionTracker.DummyData()
 *   >>> dd.set("trololo")
 *   >>> print dd.get()
 */

#include <boost/python.hpp>
#include "pymotiontracker.hh"
#include "motiontracker.hh"
#include "utils.hh"

namespace bpy = boost::python;

PyMotionTracker::PyMotionTracker(/*TODO: params*/)
{
	webcam = new Webcam;
	tracker = new ColorCrossTracker(*webcam, 2);
	// TODO: Handle resource freeing somehow
}

bpy::tuple PyMotionTracker::getPosition() const
{
	if (!tracker) return bpy::make_tuple(0, 0, 0);;
	cv::Vec3f pos = tracker->getPosition();
	return bpy::make_tuple(pos[0], pos[1], pos[2]);
}

bpy::tuple PyMotionTracker::getRotation() const
{
	if (!tracker) return bpy::make_tuple(0, 0, 0);;
	cv::Vec3f rot = tracker->getRotation();
	return bpy::make_tuple(rot[0], rot[1], rot[2]);
}

/// Dummy test class
struct DummyData
{
	void set(std::string data) { m_data = data; }
	std::string get() const { return m_data; }
	std::string m_data;
};

/// Hello world test function
char const* greet()
{
	return "Hello world!";
}

BOOST_PYTHON_MODULE(PyMotionTracker)
{
	// Exposing a function to Python
	bpy::def("greet", greet);

	// Exposing a class to Python
	bpy::class_<DummyData>("DummyData")
		.def("get", &DummyData::get)
		.def("set", &DummyData::set);

	// The actual MotionTracker API
	bpy::class_<PyMotionTracker>("PyMotionTracker")//, bpy::init<void /*TODO: constructor params*/>())
		.def("getPosition", &PyMotionTracker::getPosition)
		.def("getRotation", &PyMotionTracker::getRotation);
}
