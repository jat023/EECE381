import imgproc
from imgproc import *
from time import sleep
import matplotlib.pyplot as plt
import networkx as nx
import os
import math
import RPi.GPIO as GPIO
import Tkinter

#speeds through terrain
dgSpeed = 0.15
mgSpeed = 0.40
lgSpeed = 0.70
yellowSpeed = 0.90
whiteSpeed = 0.95


#Dijkstra's algorithm as implemented by NetworkX
def shortest_path(G, start, end):
	
		#shortest path using dijkstra's algorithm
		#the result of the function is stored in the variable 'list'
		#	this list can act as a queue or stack, using functions such as
		#	pop() to get coordinates to send to DE2	
	list = nx.dijkstra_path(G,start,end,weight='weight')
	
	print list
	print("")
	print("Path length is:")
	print(nx.dijkstra_path_length(G,start,end,weight='weight'))


#creates graph by adding all the nodes, one for each pixel
def create_nodes_of_Graph(G):
	for x in range (0, 320):
		for y in range(0, 240):
			G.add_node((x,y))

	return G


#add edges and their respective speeds between nodes
def create_edges_of_Graph(G, img):
	for y in range (1, 320):
		for x in range(1, 240):
			#get RGB color value of the current node
			red, green, blue = img[x,y]
		
			# sets weight to whiteSpeed for edge
			if (red > 225 and green > 225 and blue > 225):
				weight = whiteSpeed
			# sets weight to dgSpeed (dark green) for edge
			elif (red < 100 and green < 125 and blue < 100):
				weight = dgSpeed
			# sets weight to mgSpeed (medium green) for edge
			elif (red < 100 and (green > 124 and green < 200) and blue < 100):
				weight = mgSpeed
			# sets weight to lgSpeed (light green) for edge
			elif (red < 100 and green > 199 and blue < 100):
				weight = lgSpeed
			# sets weight to yellowSpeed for edge
			elif (red > 215 and green > 215 and blue < 10):
				weight = yellowSpeed

			if (x == 0):
				G.add_edge((x,y),(x-1,y), weight = weight)
				G.add_edge( (x,y),(x-1,y-1),weight = weight)
				G.add_edge((x,y),(x,y-1), weight = weight)
			else:
				G.add_edge((x,y),(x-1,y), weight = weight)
				G.add_edge( (x,y),(x-1,y-1),weight = weight)
				G.add_edge((x,y),(x,y-1), weight = weight)
				G.add_edge((x,y),(x+1,y+1), weight = weight)

	return G

#main function controlling logic flow
if __name__ == "__main__":
    
	#open webcame to take picture
	#camera = Camera(160, 120)

	#open a viewer window to display images
	viewer = Viewer(819, 460, "The MAP")

	#take picture from camera
	#img = camera.grabImage()

	#get image (bmp) from pathname (if no camera)
	img = Image("/home/pi/Desktop/381map.bmp")

	#display image in viewer
	viewer.displayImage(img)

	print("Creating graph. Give me a minute ...")
	print("")
	G = nx.Graph()
	G = create_nodes_of_Graph(G)
	print "Number of nodes: ", G.number_of_nodes()
	G = create_edges_of_Graph(G, img)
	print "Number of edges: ", G.number_of_edges()
	print("Done creating graph")
	print("")
	print("Drawing shortest path...")

		#these points currently arbitrariliy assigned; should be able to be
		#set by user when GUI is in place
	start = (0,0)
	end = (75,189)

	#call to the shortest_path function, uses Dijkstra's Algorithm
	shortest_path(G, start, end)

	print("")
	print("Finished processing. Ready to transfer")
	print("")

	#display the image again
	viewer.displayImage(img)

	#delay before closing window (ms)
	sleep(3)
	#waitTime(5000)

	#end of script
