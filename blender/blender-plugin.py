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

from Blender import Draw, Scene
import PyMotionTracker

def main():
	# Get scene
	scn = Scene.GetCurrent()
	if not scn.objects.context:
		return

	# Create a tracker object and initialize webcam
	tracker = PyMotionTracker.PyMotionTracker()

	# Get selected objects
	sel = scn.objects.context

	# Main loop
	while True:
		# Get location and orientation from the tracker
		loc = tracker.getPosition()
		rot = tracker.getRotation()

		# Loop through the objects
		for ob in sel:
			# Manipulate object location and orientation
			ob.loc = (ob.LocX + loc[0],  ob.LocY + loc[1],  ob.LocZ + loc[2])
			ob.rot = (ob.RotX + rot[0],  ob.RotY + rot[1],  ob.RotZ + rot[2])

	
if __name__ == '__main__':
	main()

