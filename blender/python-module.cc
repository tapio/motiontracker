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

/// Hello world test function
char const* greet()
{
	return "Hello world!";
}

BOOST_PYTHON_MODULE(PyMotionTracker)
{
	// Exposing a function to Python
	bpy::def("greet", greet);

	// The actual MotionTracker API
	bpy::class_<PyMotionTracker>("PyMotionTracker")//, bpy::init<void /*TODO: constructor params*/>())
		.def("getPosition", &PyMotionTracker::getPosition)
		.def("getRotation", &PyMotionTracker::getRotation);
}
