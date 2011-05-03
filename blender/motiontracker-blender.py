#!BPY

"""
Name: 'Motion Tracker'
Blender: 249b
Group: 'Object'
Tooltip: 'Rotate object by using a webcam motion tracker'
"""

__author__ = "Tapio Vierros, Antti Virta"
__bpydoc__=\
'''
This script utilizes a webcam motion tracker to rotate a blender object.
'''

from Blender import Draw, Scene, Mathutils
from Blender.Mathutils import *
import PyMotionTracker
from collections import deque

# Create a tracker object and initialize webcam
tracker = PyMotionTracker.PyMotionTracker()

# Select rotation method between absolute (euler) and rotational
eulerMethod = False

filterSize = 10
d = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
meanX = deque(d,filterSize)
meanY = deque(d,filterSize)
meanZ = deque(d,filterSize)

def gui():
    global tracker
    global eulerMethod
    
    global meanX, meanY, meanZ
    
    # Get scene
    scn = Scene.GetCurrent()
    if not scn.objects.context:
        return

    # Get selected objects
    sel = scn.objects.context

    # Get location and orientation from the tracker
    loc = tracker.getPosition()
    rot = tracker.getRotation()
        
    rotv = Vector(rot[0], rot[1], rot[2])
    rotMat = RotationMatrix(rotv.length,4,"r",rotv)
    euler = rotMat.toEuler()
    
   
        
    print "Location: " + str(loc)
    print "Euler angles: " + str(euler[0]) + " " + str(euler[1]) + " " + str(euler[2])

    # Loop through the objects
    if euler[0] != 0 and euler[1] != 0 and euler[2] != 0:
         # Add the angles to deques, which manage their size themselves
        if eulerMethod:
            meanX.append(euler[0])
            meanY.append(euler[1])
            meanZ.append(euler[2])
            euler = Euler(sum(meanX)/filterSize,sum(meanY)/filterSize,sum(meanZ)/filterSize)

        # Manipulate object location and orientation 
        for ob in sel:
            if eulerMethod:                 
                ob.setEuler(euler)
            else:
                ob.setMatrix(rotMat * ob.getMatrix())


def event(evt, val):
    if evt == Draw.ESCKEY:
        Draw.Exit()


def bevent( evt ):
    pass


def main():
    Draw.Register(gui, event, bevent)


if __name__ == '__main__':
    main()

