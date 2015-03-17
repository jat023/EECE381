import imgproc
from imgproc import *
from time import sleep
import matplotlib.pyplot as plt
import networkx as nx
import os
import math
import RPi.GPIO as GPIO

#open webcame to take picture
##camera = Camera(160, 120)

#open a viewer window to display images
viewer = Viewer(819, 460, "The first step")

#take picture from camera
##img = camera.grabImage()

#get image (bmp) from pathname (if no camera)
img = Image("/home/pi/Desktop/381map.bmp")

#display image in viewer
viewer.displayImage(img)

#constants used in the algorithm
ref_red = 192
ref_blue = 80
ref_green = 80
threshold = 96 
m = img.width	#x axis
n = img.height	#y axis

''' This creats our graph; remove comments when needed
G = nx.grid_2d_graph(m, n, periodic=False, create_using=None)
print(G.number_of_nodes())
print(G.number_of_edges())
print("Done creating graph")
print("")
print("Processing image...")
'''

#iterate over each pixel in image
for x in range (0, img.width):
	for y in range(0, img.height):
		#Get value at xth column and yth row, place intensities into variables		
		red, green, blue = img[x,y]

		if (red == blue and blue == green):
			#RGB values are the same; they will be a shade of grey, wht, or blk
			#remove edges from this node
		
		

		'''
		# subtract the pixel colour from the reference
		d_red = ref_red - red
		d_green = ref_green - green
		d_blue = ref_blue - blue

		# length of the difference vector
		length = math.sqrt( (d_red * d_red) + (d_green * d_green) + (d_blue * d_blue) )

		if length > threshold:
			print(length)
			print(threshold)
			img[x, y] = 0, 0, 0
		else:
			img[x, y] = 255, 255, 255
		'''



#display the image again
viewer.displayImage(img)

#delay before closing window (ms)
sleep(5)
#waitTime(5000)

#end of script
