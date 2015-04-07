import imgproc
from imgproc import *
from time import sleep
import matplotlib.pyplot as plt
import networkx as nx
import os
import math
import RPi.GPIO as GPIO



#open webcame to take picture
camera = Camera(819, 480)

#open a viewer window to display images
viewer = Viewer(819, 460, "The MAP")

#take picture from camera
img = camera.grabImage()

#get image (bmp) from pathname (if no camera)
#img = Image("/home/pi/Desktop/381map.bmp")

#display image in viewer
viewer.displayImage(img)



#delay before closing window (ms)
sleep(9)
#waitTime(5000)

#end of script
		
