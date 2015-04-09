from bitstring import BitArray, BitStream 

path = [(100,100),(99,99),(97,98),(97,96), (96,96), (95,95), (94,94), (93,93), (92,92), (91,91)]

newPathx = []
newPathy = []

def parsePath():
	coordinate1 = list(path.pop())
	x1 = coordinate1[0]
	y1 = coordinate1[1]
	coordinate2 = list(path.pop())
	x2 = coordinate2[0]
	y2 = coordinate2[1]
	test1 = [x1-x2, y1-y2]

	newPathx.append(x1)
	newPathy.append(y1)

	while path:
		coordinate = list(path.pop())
		x = coordinate[0]
		y = coordinate[1]
	
		test2 = [x2-x, y2-y]
		print test1
		print test2

		if (test1 == test2):
			print 1
			test1 = test2
			x2 = x
			y2 = y
		else:
			print 2
			newPathx.append(int(x))
			newPathy.append(int(y))
			x2 = x
			y2 = y

	newPathx.append(x)
	newPathy.append(y)
