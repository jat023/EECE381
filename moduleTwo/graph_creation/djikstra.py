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
    #import sys;sys.argv = ['', 'Test.testName']
    #unittest.main()
    graph = {'s': {'a': 2, 'b': 9},
             'a': {'s': 3, 'b': 4, 'c':8},
             'b': {'s': 6, 'a': 2, 'd': 2},
             'c': {'a': 2, 'd': 1, 't': 4},
             'd': {'b': 1, 'c': 2, 't': 5},
             't': {'c': 9, 'd': 5}}
    dijkstra(graph,'s','t')
