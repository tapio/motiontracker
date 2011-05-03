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

# Create a tracker object and initialize webcam
tracker = PyMotionTracker.PyMotionTracker()

def gui():
    global tracker
    degToRad = 3.1415/180

    # Get scene
    scn = Scene.GetCurrent()
    if not scn.objects.context:
        return

    # Get selected objects
    sel = scn.objects.context

    # Get location and orientation from the tracker
    loc = tracker.getPosition()
    rot = tracker.getRotation()
    rotm = tracker.getRotationMatrix()
    
    rot_matrix =  Matrix([rotm[0],rotm[1],rotm[2]],
    [rotm[3],rotm[4],rotm[5]],
    [rotm[6],rotm[7],rotm[8]])
    
    euler = rot_matrix.toEuler()
    
    rotv = Vector(rot[0], rot[1], rot[2])
    rotMat = RotationMatrix(rotv.length,4,"r",rotv)
    
    print "Location: " + str(loc)
    print "Euler angles: " + str(euler[0]) + " " + str(euler[1]) + " " + str(euler[2])

    # Loop through the objects
    if euler[0] != 0 and euler[1] != 0 and euler[2] != 0:
        for ob in sel:
            # Manipulate object location and orientation
            # ob.RotX = degToRad*euler[0]
            # ob.RotY = degToRad*euler[1]
            # ob.RotZ = degToRad*euler[2]
            ob.setMatrix(rotMat * ob.getMatrix())
            # = (degToRad*euler[0], degToRad*euler[1], degToRad*euler[2])


def event(evt, val):
    if evt == Draw.ESCKEY:
        Draw.Exit()


def bevent( evt ):
    pass


def main():
    Draw.Register(gui, event, bevent)


if __name__ == '__main__':
    main()

