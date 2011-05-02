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
* BUILD_DEPRECATED
* BUILD_PLUGIN
* BUILD\_SHARED\_LIBS (defaults to OFF, enable with ON)

<!-- (Note to people who read the plain text version instead of formatted: backslash-underscore means just underscore.) -->


Test programs and tools
-----------------------
Below is a list with short descriptions of the examples and tools.

* __edgefinder__ - simple app for testing OpenCV's algorithms and MotionTracker's webcam interface
* __gputest__ - simple video thresholding that can use GPU for calculations
* __colorcalibrator__ - a utility for searching correct hue thresholding values for object tracking
* __colortracker__ - tracks a colored blob
* __tracker__ - shows the motion tracker in action, requires calibration with _colorcalibrator_

The next ones have been deprecated and are not being maintained anymore. If you want to test them, you need to set the BUILD\_DEPRECATED CMake flag to ON.

* __calibrator__ - a utility for determining webcam's intrinsic camera parameters (for solvePnP algorithm)
* __chessboardfinder__ - finds a chessboard from webcam and displays the detection
* __glchessboardtracker__ - finds a chessboard from webcam and tries to rotate a cube according to that
* __colorcross__ - early tracker test for still image


Colorcalibrator How To
----------------------
1. Take a picture by pressing any key
2. Click on the four points to be tracked in the picture
3. Press any key to enter the next stage of the calibration
4. Adjust the hue and saturation/value sliders to get desired result
5. Repeat for three other points
6. Finished (saved to calibration.xml)


Blender-plugin How To
---------------------
1. Build the python module (libPyMotionTracker.so / PyMotionTracker.dll)
2. If the name has lib-prefix, rename it to be without it (e.g. PyMotionTracker.so)
3. Copy (or symlink) the python module and the plugin (motiontracker-blender.py) to Blender's script directory (~/.blender/scripts)
4. Open Blender (calibration.xml in working directory) and select an object to manipulate
5. Change to the Scripts Window (green snake icon)
6. Start the plugin by accessing it from the menu: Scripts | Object | Motion Tracker

