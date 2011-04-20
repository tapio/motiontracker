#!BPY

"""
Name: 'Motion Tracker'
Blender: 249b
Group: 'Object'
Tooltip: 'Rotate object by using a webcam motion tracker'
"""

__author__ = "Tapio Vierros"
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

    # Get scene
    scn = Scene.GetCurrent()
    if not scn.objects.context:
        return

    # Get selected objects
    sel = scn.objects.context

    # Get location and orientation from the tracker
    loc = tracker.getPosition()
    rot = tracker.getRotation()
    
    ## Rotation matrix stuff
    rotm = tracker.getRotationMatrix()
    
    rot_matrix =  Matrix([rotm[0],rotm[1],rotm[2]],
    [rotm[3],rotm[4],rotm[5]],
    [rotm[6],rotm[7],rotm[8]])
    
    euler = rot_matrix.toEuler()
    

    print "Location: " + str(loc)
    print "Rotation: " + str(rot)
    print "Rotation matrix: " + str(rotm)
    print "Euler: " + str(euler)

    # Loop through the objects
    for ob in sel:
        # Manipulate object location and orientation
        #ob.loc = (ob.LocX + loc[0],  ob.LocY + loc[1],  ob.LocZ + loc[2])
        #ob.rot = (ob.RotX + rot[0],  ob.RotY + rot[1],  ob.RotZ + rot[2])
        #ob.rot = (rot[0], rot[1], rot[2])
        ob.setEuler(euler);


def event(evt, val):
    if evt == Draw.ESCKEY:
        Draw.Exit()


def bevent( evt ):
    pass


def main():
    Draw.Register(gui, event, bevent)


if __name__ == '__main__':
    main()

