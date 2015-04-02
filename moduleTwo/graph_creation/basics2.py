import imgproc
from imgproc import *
from time import sleep
import matplotlib.pyplot as plt
import networkx as nx
import os
import math
import RPi.GPIO as GPIO

def dijkstra(graph,source,dest,visited=[],distances={},predecessors={}):
    """ calculates a shortest path tree routed in source
    """    
    #make sure source and destination nodes are in the graph itself
    if source not in graph:
        raise TypeError('Source not on map')
    if dest not in graph:
        raise TypeError('Destination not on map')    

    # ending condition
    if source == dest:
        # build the shortest path and display it
        path=[]
        pred=dest
        while pred != None:
            path.append(pred)
            pred=predecessors.get(pred,None)
        print('Shortest path: '+str(path)+" cost="+str(distances[dest])) 
    else :     
        # if initial run, initialize the cost
        if not visited: 
            distances[source]=0
        # visit the neighbors
        for neighbor in graph[source] :
            if neighbor not in visited:
                new_distance = distances[source] + graph[source][neighbor]
                if new_distance < distances.get(neighbor,float('inf')):
                    distances[neighbor] = new_distance
                    predecessors[neighbor] = source
        # mark as visited
        visited.append(source)
        # now that all neighbors have been visited: recurse                         
        # select the non visited node with lowest distance 'x'
        # run Dijskstra with source='x'
        unvisited={}
        for k in graph:
            if k not in visited:
                unvisited[k] = distances.get(k,float('inf'))        
        x=min(unvisited, key=unvisited.get)
        dijkstra(graph,x,dest,visited,distances,predecessors)

if __name__ == "__main__":
    
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

	#speeds through terrain; simple five colors used for testing purposes
	#may need to be modified later
	dgSpeed = 0.15
	mgSpeed = 0.40
	lgSpeed = 0.70
	yellowSpeed = 0.90
	whiteSpeed = 0.95

		#creates graph by adding all the nodes, one for each pixel
	print("Creating graph. Give me a minute ...")
	G = nx.Graph()
	for x in range (0, 10):
		for y in range(0, 10):
			G.add_node((x,y))

	print "Number of nodes: ", G.number_of_nodes()

		#add edges and their respective speeds between nodes
	for y in range (1, 10):
		for x in range(1, 10):
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

			if (x == 10):
				G.add_edge((x,y),(x-1,y), weight = weight)
				G.add_edge( (x,y),(x-1,y-1),weight = weight)
				G.add_edge((x,y),(x,y-1), weight = weight)
			else:
				G.add_edge((x,y),(x-1,y), weight = weight)
				G.add_edge( (x,y),(x-1,y-1),weight = weight)
				G.add_edge((x,y),(x,y-1), weight = weight)
				G.add_edge((x,y),(x+1,y+1), weight = weight)

	print "Number of edges: ", G.number_of_edges()
	print("Done creating graph")

	print("")
	'''
	print("Drawing graph")
	pos = nx.spring_layout(G)
	elarge=[(u,v) for (u,v,d) in G.edges(data=True) if d['weight'] > 0.1]
	esmall=[(u,v) for (u,v,d) in G.edges(data=True) if d['weight'] < 0.1]

	nx.draw_networkx_nodes(G,pos, node_size=700)
	nx.draw_networkx_edges(G,pos, edgelist=elarge,width=6,style='dashed')
	nx.draw_networkx_edges(G,pos, edgelist=esmall,width=3)
	plt.show()
	print("")
	'''
	print("Drawing shortest path...")
	dijkstra(G,(0,0),(9,9))
	print("")
	print("Finished processing. Ready to transfer")
	print("")


	#display the image again
	viewer.displayImage(img)

	#delay before closing window (ms)
	sleep(3)
	#waitTime(5000)

	#end of script
