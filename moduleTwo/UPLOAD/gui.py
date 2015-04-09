from Tkinter import *
import Tkinter
import tkMessageBox

top = Tkinter.Tk()
frame = Frame(top, bd=5, height=500, width=500)
frame.pack()

def generateGraph():
	tkMessageBox.showinfo("Click", "Generating graph!")

def showNodes():
	tkMessageBox.showinfo("Click", "Many nodes!")

def showEdges():
	tkMessageBox.showinfo("Click", "Many more edges!")

def sendCoordinates():
	tkMessageBox.showinfo("Click", "That DE2 better be listening...")
	#make sure to get coordinates (start and destination) from entry fields
			#entryStart and entryEnd

def setCoordinates():
	tkMessageBox.showinfo("Click", "The coordinates have been set")
	s = entryStart.get()
	d = entryEnd.get()
	print(s)
	print(d)
	entryStart.delete(0, END)
	entryEnd.delete(0, END)

def reset():
	tkMessageBox.showinfo("Click", "Graph has been deleted. Click Create to regenerate.")

def quit():
	tkMessageBox.showinfo("Click", "You're leaving me? :( Okay...")
	top.destroy()

		#buttons
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

buttonCreateGraph.place(bordermode=OUTSIDE, x=20, y=100, height=30, width=150)
buttonNodes.place(bordermode=OUTSIDE, x=20, y=130, height=30, width=150)
buttonEdges.place(bordermode=OUTSIDE, x=20, y=160, height=30, width=150)
buttonSet.place(bordermode=OUTSIDE, x=10, y=270, height=30, width=200)
buttonSend.place(bordermode=OUTSIDE, x=220, y=270, height=30, width=200)
buttonQuit.place(bordermode=OUTSIDE, x=325, y=10, height=30, width=150)
buttonDelete.place(bordermode=OUTSIDE, x=200, y=100, height=30, width=150)

	#labels and text entry fields
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

labelStart.place(bordermode=OUTSIDE, x=20, y=200)
labelEnd.place(bordermode=OUTSIDE, x=20, y=230)
entryStart.place(bordermode=OUTSIDE, x=150, y=200)
entryEnd.place(bordermode=OUTSIDE, x=150, y=230)
labelHeader.place(bordermode=OUTSIDE, x=10, y=10)

top.mainloop()