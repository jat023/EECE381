#!/usr/bin/env pythonrat

import imgproc
from imgproc import *
import RPi.GPIO as GPIO
from Tkinter import Frame
from Tkinter import Tk
from Tkinter import Place
from Tkinter import Grid
from Tkinter import Pack
from Tkinter import Label
from Tkinter import Entry
import Tkinter
import tkMessageBox
from time import sleep
#import matplotlib.pyplot as plt
import networkx as nx
import os
import math

top = Tk()
top.title("Orienteering APP")
frame = Frame(top, bd=5, height=500, width=500)
frame.pack()

#speeds through terrain
dgSpeed = 0.15
mgSpeed = 0.40
lgSpeed = 0.70
yellowSpeed = 0.90
whiteSpeed = 0.95

G = nx.Graph()
start = (0,0)
end = (0,0)
path = []
img = Image("/home/pi/Desktop/381_colours.bmp")
weight = 0

#Dijkstra's algorithm as implemented by NetworkX
def shortest_path(G, start, end):
	
		#shortest path using dijkstra's algorithm
		#the result of the function is stored in the variable 'list'
		#	this list can act as a queue or stack, using functions such as
		#	pop() to get coordinates to send to DE2	
	list = nx.dijkstra_path(G,start,end,weight='weight')

	print("Path length is:")
	print(nx.dijkstra_path_length(G,start,end,weight='weight'))
	return list

#creates graph by adding all the nodes, one for each pixel
def create_nodes_of_Graph():
	global G

	for x in range (0, 320):
		for y in range(0, 240):
			G.add_node((x,y))

#add edges and their respective speeds between nodes
def create_edges_of_Graph():
	global G
	global img
	global weight

	for y in range (1, 240):
		for x in range(1, 320):
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

			if (x == 320):
				G.add_edge((x,y),(x-1,y), weight = weight)
				G.add_edge( (x,y),(x-1,y-1),weight = weight)
				G.add_edge((x,y),(x,y-1), weight = weight)
			else:
				G.add_edge((x,y),(x-1,y), weight = weight)
				G.add_edge( (x,y),(x-1,y-1),weight = weight)
				G.add_edge((x,y),(x,y-1), weight = weight)
				G.add_edge((x,y),(x+1,y-1), weight = weight)

def generateGraph():
	global path
	print("Creating graph. Give me a minute.")
	print("")
	print("Generating nodes ...")
	create_nodes_of_Graph()
	print("Generating edges...")
	create_edges_of_Graph()
	print("")
	print("Done creating graph.")
	print("")
	print("Looking for shortest path...")
	#path = shortest_path(G, start, end)
	tkMessageBox.showinfo("Click", "Graph finished!")

def showNodes():
	print(G.number_of_nodes())
	tkMessageBox.showinfo("Click", "Many nodes!")

def showEdges():
	print(G.number_of_edges())
	tkMessageBox.showinfo("Click", "Many more edges!")

def sendCoordinates():
	tkMessageBox.showinfo("Click", "That DE2 better be listening...")
	# TODO - human generated stub
	#

def setCoordinates():
# fix this later... but soon later
	global start
	global end
	start = entryStart.get()
	end = entryEnd.get()
	print start
	print end
	entryStart.delete(0, END)
	entryEnd.delete(0, END)
	tkMessageBox.showinfo("Click", "The coordinates have been set")

def reset():
	global G
	G.clear()
	tkMessageBox.showinfo("Click", "Graph has been deleted. Click Create to regenerate.")

def quit():
	tkMessageBox.showinfo("Click", "You're leaving me? :( Okay...")
	top.destroy()

#main function controlling logic flow
if __name__ == "__main__":
    
    	#buttons on the GUI
	buttonCreateGraph = Tkinter.Button(top, text="Create", command = generateGraph)
	buttonNodes = Tkinter.Button(top, text="Nodes", command=showNodes)
	buttonEdges = Tkinter.Button(top, text="Edges", command=showEdges)
	buttonSend = Tkinter.Button(top, text="Send Coordinates", command=sendCoordinates)
	buttonQuit = Tkinter.Button(top, text="Quit", command=quit)
	buttonSet = Tkinter.Button(top, text="Set Coordinates", command=setCoordinates)
	buttonDelete = Tkinter.Button(top, text="Reset", command=reset)

	buttonCreateGraph.pack()
	buttonNodes.pack()
	buttonEdges.pack()
	buttonQuit.pack()
	buttonSend.pack()
	buttonSet.pack()
	buttonDelete.pack()

	buttonCreateGraph.place(x=20, y=100, height=30, width=150)
	buttonNodes.place(x=20, y=130, height=30, width=150)
	buttonEdges.place(x=20, y=160, height=30, width=150)
	buttonSet.place(x=10, y=270, height=30, width=200)
	buttonSend.place(x=220, y=270, height=30, width=200)
	buttonQuit.place(x=325, y=10, height=30, width=150)
	buttonDelete.place(x=200, y=100, height=30, width=150)

		#labels and entry fields for setting coordinates
	labelHeader = Label(top, text="Welcome to Orienteering!")
	labelStart = Label(top, text="Starting point")
	labelEnd = Label(top, text="Destination point")
	entryStart = Entry(top, bd=5)
	entryEnd = Entry(top, bd=5)
		
	labelHeader.pack()
	labelStart.pack()
	labelEnd.pack()
	entryStart.pack()
	entryEnd.pack()

	labelStart.place(x=20, y=200)
	labelEnd.place(x=20, y=230)
	entryStart.place(x=150, y=200)
	entryEnd.place(x=150, y=230)
	labelHeader.place(x=10, y=10)

	#open webcame to take picture
	#camera = Camera(160, 120)

	#open a viewer window to display images
	#viewer = Viewer(819, 460, "The MAP")

	#take picture from camera
	#img = camera.grabImage()

	#get image (bmp) from pathname (if no camera)
	#img = Image("/home/pi/Desktop/381_colours.bmp")

	#display image in viewer
	#viewer.displayImage(img)

	top.mainloop()

	#display the image again
	#viewer.displayImage(img)

	#delay before closing window (ms)
	#sleep(3)
	#waitTime(5000)

	#end of script
