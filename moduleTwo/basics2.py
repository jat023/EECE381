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

#speeds through terrain; simple five colors used for testing purposes
#may need to be modified later
dgSpeed = 0.15
mgSpeed = 0.40
lgSpeed = 0.70
yellowSpeed = 0.90
whiteSpeed = 0.95


print("Creating graph. Give me a minute ...")
G = nx.Graph()
for x in range (0, img.width):
	for y in range(0, img.height):
		G.add_node((x,y))

print('Number of nodes: ', G.number_of_nodes())

for x in range (1, img.width - 1):
	for y in range(1, img.height - 1):
		G.add_edge((x,y),(x-1,y-1))
		G.add_edge((x,y),(x-1,y))
		G.add_edge((x,y),(x,y-1))
		G.add_edge((x,y),(x+1,y+1))

print('Number of edges: ', G.number_of_edges())
print("")
print("Done creating graph")
print("")
print("Processing image...")
print("")

#iterate over each pixel in image
for x in range (1, img.width - 1):
	for y in range(1, img.height - 1):
		#Get value at xth column and yth row, place intensities into variables		
		red, green, blue = img[x,y]
		
		#if red, green, and blue all > 225, then we have white
		if (red > 225 and green > 225 and blue > 225):
			G[(x,y)][(x-1,y-1)]['weight'] = whiteSpeed
			G[(x,y)][(x-1,y)]['weight'] = whiteSpeed
			G[(x,y)][(x,y-1)]['weight'] = whiteSpeed
			G[(x,y)][(x+1,y-1)]['weight'] = whiteSpeed
		elif (red < 100 and green < 125 and blue < 100):
			G[(x,y)][(x-1,y-1)]['weight'] = dgSpeed
			G[(x,y)][(x-1,y)]['weight'] = dgSpeed
			G[(x,y)][(x,y-1)]['weight'] = dgSpeed
			G[(x,y)][(x+1,y-1)]['weight'] = dgSpeed
		elif (red < 100 and (green > 124 and green < 200) and blue < 100):
			G[(x,y)][(x-1,y-1)]['weight'] = mgSpeed
			G[(x,y)][(x-1,y)]['weight'] = mgSpeed
			G[(x,y)][(x,y-1)]['weight'] = mgSpeed
			G[(x,y)][(x+1,y-1)]['weight'] = mgSpeed
		elif (red < 100 and green > 199 and blue < 100):
			G[(x,y)][(x-1,y-1)]['weight'] = lgSpeed
			G[(x,y)][(x-1,y)]['weight'] = lgSpeed
			G[(x,y)][(x,y-1)]['weight'] = lgSpeed
			G[(x,y)][(x+1,y-1)]['weight'] = lgSpeed
		elif (red > 215 and green > 215 and blue < 10):
			G[(x,y)][(x-1,y-1)]['weight'] = yellowSpeed
			G[(x,y)][(x-1,y)]['weight'] = yellowSpeed
			G[(x,y)][(x,y-1)]['weight'] = yellowSpeed
			G[(x,y)][(x+1,y-1)]['weight'] = yellowSpeed
			

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
print("")
print("Finished processing. Ready to transfer")
print("")


#display the image again
viewer.displayImage(img)

#delay before closing window (ms)
sleep(5)
#waitTime(5000)

#end of script
