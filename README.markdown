MotionTracker
=============

The aim of this project is to create a webcam based system for tracking object position and orientation.

Dependencies
------------
Currently the project depends on Git (for fetching the source) and CMake (for building) plus the following libraries:

* OpenCV
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

