MotionTracker
=============

The aim of this project is to create a webcam based system for tracking object position and orientation.

It consists of the following parts:

* Library component with C++ API
* Example programs (also C++)
* Blender plugin (not yet started)
* Documentation

Dependencies
------------

Required tools:

* C++ compiler
* _CMake_ (for generating Makefiles / project files)
* Optional: _Git_ (for fetching the sources)
* Optional: _Doxygen_ (for building documentation)

Required libraries:

* OpenCV (>= 2.1.0 to work with CMake, >= 2.2.0 with Qt GUI enabled for examples)
* Boost (headers and thread-component)

Compiling
---------
    $ git clone git://github.com/tapio/motiontracker.git
    $ cd motiontracker
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./tracker

Example programs
----------------
Currently there are three example programs:

* __opencvtest__ - simple app for testing OpenCV's algorithms and MotionTracker's webcam interface
* __calibrator__ - a utility for determining webcam's intrinsic camera parameters
* __tracker__ - shows the motion tracker in action, requires calibration with _calibrator_
