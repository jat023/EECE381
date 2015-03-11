import imgproc
from imgproc import *
import matplotlib.pyplot as plt

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



#iterate over each pixel in image
for x in range (0, img.width):
	for y in range(0, img.height):
		#Get value at xth column and yth row, place intensities into variables		
		red, green, blue = img[x,y]

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
				add edge to graph to connect (x,y) and (x, y-1)
				set edge value based on color of the pixels

			if x is NOT the last pixel on the iamge
				check (x,y) color value against (x+1, y-1) color value		(ABOVE RIGHT)
				if able 
					add edge to graph to connect (x,y) and (x+1, y-1)
					set edge value based on color of the pixels
------------------------------------------------------------
'''



#display the image again
viewer.displayImage(img)

#delay before closing window (ms)
waitTime(5000)

#end of script
