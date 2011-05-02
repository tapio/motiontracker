#include <boost/python.hpp>
#include <iostream>
#include "pymotiontracker.hh"
#include "motiontracker.hh"
#include "utils.hh"

namespace bpy = boost::python;

PyMotionTracker::PyMotionTracker(/*TODO: params*/)
{
	webcam = new Webcam;
	tracker = new ColorCrossTracker(*webcam, 2);
	std::cout << "PyMotionTracker initialized." << std::endl;
}

PyMotionTracker::~PyMotionTracker()
{
	if (tracker) delete tracker;
	if (webcam) delete webcam;
	std::cout << "PyMotionTracker deinitialized." << std::endl;
}

bpy::tuple PyMotionTracker::getPosition() const
{
	if (!tracker) return bpy::make_tuple(0, 0, 0);
	cv::Vec3f pos = tracker->getPosition();
	return bpy::make_tuple(pos[0], pos[1], pos[2]);
}

bpy::tuple PyMotionTracker::getRotation() const
{
	if (!tracker) return bpy::make_tuple(0, 0, 0);
	cv::Vec3f rot = tracker->getRotation();
	return bpy::make_tuple(rot[0], rot[1], rot[2]);
}

bpy::tuple PyMotionTracker::getRotationMatrix() const
{
	if (!tracker) return bpy::make_tuple(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	cv::Mat rotm = tracker->getRotationMatrix();
	return bpy::make_tuple(rotm.at<float>(0,0), rotm.at<float>(0,1), rotm.at<float>(0,2),
						   rotm.at<float>(1,0), rotm.at<float>(1,1), rotm.at<float>(1,2),
						   rotm.at<float>(2,0), rotm.at<float>(2,1), rotm.at<float>(2,2));
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
	bpy::class_<PyMotionTracker>("PyMotionTracker")
		.def("getPosition", &PyMotionTracker::getPosition)
		.def("getRotation", &PyMotionTracker::getRotation)
		.def("getRotationMatrix", &PyMotionTracker::getRotationMatrix);
}
