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


def main():
	scn = Scene.GetCurrent()
	if not scn.objects.context:
		return

	sel = scn.objects.context

	for ob in sel:
		ob.loc = (ob.LocX+1,  ob.LocY+1,  ob.LocZ+1) # TODO: Manipulate
		ob.rot = (ob.RotX+1,  ob.RotY+1,  ob.RotZ+1) # TODO: Manipulate

	
if __name__ == '__main__':
	main()

