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

namespace bpy = boost::python;

struct DummyData
{
	void set(std::string data) { m_data = data; }
	std::string get() const { return m_data; }
	std::string m_data;
};

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
}
