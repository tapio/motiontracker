MotionTracker
=============

The aim of this project is to create a webcam based system for tracking object position and orientation.

It consists of the following parts:

* Library component with C++ API
* Example programs (also C++)
* Blender plugin with minimal Python API
* Documentation

Dependencies
------------

Tools:

* C++ compiler
* CMake (for generating Makefiles / project files)
* _Optional:_ Git (for fetching the sources)
* _Optional:_ Doxygen (for building documentation)

Libraries:

* OpenCV (>= 2.2.0 (examples also require the Qt GUI with OpenGL support enabled))
* Boost (headers and thread-component, optionally python-component for Blender-plugin)
* _Optional:_ Python libraries for the Blender-plugin's Python module
* _Optional:_ OpenGL for some example programs

Compiling
---------
    $ git clone git://github.com/tapio/motiontracker.git
    $ cd motiontracker
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./tracker

By giving cmake-command a parameter in the form of -DBUILD\_XXX=OFF you can disable some parts of the project. Possible values for BUILD\_XXX are:

* BUILD_APIDOC
* BUILD_EXAMPLES
* BUILD_PLUGIN
* BUILD\_SHARED\_LIBS (defaults to OFF, enable with ON)

<!-- (Note to people who read the plain text version instead of formatted: backslash-underscore means just underscore.) -->

Example programs
----------------
Currently there are seven (unfinished) example programs:

* __edgefinder__ - simple app for testing OpenCV's algorithms and MotionTracker's webcam interface
* __calibrator__ - a utility for determining webcam's intrinsic camera parameters
* __chessboardfinder__ - finds a chessboard from webcam and displays the detection
* __glchessboardtracker__ - finds a chessboard from webcam and tries to rotate a cube according to that
* __gputest__ - simple video thresholding that can use GPU for calculations
* __colortracker__ - tracks a colored blob
* __tracker__ - will show the motion tracker in action, requires calibration with _calibrator_
