import imgproc
from imgproc import *
import matplotlib.pyplot as plt
import networkx as nx

#open webcame to take picture
##camera = Camera(160, 120)

#open a viewer window to display images
#viewer = Viewer(819, 460, "The first step")

#take picture from camera
##img = camera.grabImage()

#get image (bmp) from pathname (if no camera)
img = Image("/home/pi/Desktop/381map.bmp")

#display image in viewer
#viewer.displayImage(img)


'''
#iterate over each pixel in image
for x in range (0, img.width):
	for y in range(0, img.height):
		#Get value at xth column and yth row, place intensities into variables		
		red, green, blue = img[x,y]
'''



G = nx.Graph()

for y in range(0, img.height):
	for x in range(0, img.width):
		G.add_node((x,y))

print(G.number_of_nodes())

for y in range(1, (img.height - 1)):
	for x in range(1, (img.width - 1)):
		G.add_edge((x,y),(x-1,y))	#left
		G.add_edge((x,y),(x-1,y-1))	#above left
		G.add_edge((x,y),(x, y-1))	#above
		#G.add_edge((x,y),(x+1,y-1))	#above right

print(G.number_of_edges())



'''
------------create graph of the parsed image------------

Create/set graph variable
Get color value of pixel

For each pixel on x axis
	For each pixel on y axis

		if x is still in the top row

			if x is the first pixel
				add x to graph
			else
				add x to graph

				check pixel(x) color value against pixel(x-1) color value	(LEFT)
				if able
					add edge to graph to connect pixel(x) and pixel(x-1)
					set edge value based on color of the pixels

		else
			add (x,y) to graph

			check (x,y) color value against (x-1,y) color value		(LEFT)
			if able 
				add edge to graph to connect (x,y) and (x-1,y)
				set edge value based on color of the pixels

			check (x,y) color value against (x, y-1) color value		(ABOVE)
			if able 
				add edge to graph to connect (x,y) and (x, y-1)
				set edge value based on color of the pixels

			check (x,y) color value against (x-1, y-1) color value		(ABOVE LEFT)
			if able 
				add edge to graph to connect (x,y) and (x-1, y-1)
				set edge value based on color of the pixels

			if x is NOT the last pixel on the image
				check (x,y) color value against (x+1, y-1) color value		(ABOVE RIGHT)
				if able 
					add edge to graph to connect (x,y) and (x+1, y-1)
					set edge value based on color of the pixels
------------------------------------------------------------
'''



#display the image again
#viewer.displayImage(img)

#delay before closing window (ms)
waitTime(5000)

#end of script
