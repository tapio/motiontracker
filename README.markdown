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

Tools:

* C++ compiler
* CMake (for generating Makefiles / project files)
* _Optional:_ Git (for fetching the sources)
* _Optional:_ Doxygen (for building documentation)

Libraries:

* OpenCV (>= 2.1.0 to work with CMake, >= 2.2.0 with Qt GUI enabled for examples)
* Boost (headers and thread-component)
* _Optional:_ OpenGL and GLUT for some example programs

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
Currently there are five (unfinished) example programs:

* __edgefinder__ - simple app for testing OpenCV's algorithms and MotionTracker's webcam interface
* __calibrator__ - a utility for determining webcam's intrinsic camera parameters
* __chessboardfinder__ - finds a chessboard from webcam and displays the detection
* __glchessboardtracker__ - finds a chessboard from webcam and rotates a cube according to that
* __tracker__ - shows the motion tracker in action, requires calibration with _calibrator_
