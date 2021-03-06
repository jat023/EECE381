#Adam's write to DE2 program
# Import the required module.
import time
import RPi.GPIO as GPIO
from bitstring import BitArray, BitStream 
import imgproc
from imgproc import *
import write
# Set the mode to use physical numbering the pins.

img = Image("/home/pi/Desktop/381_colours.bmp")

GPIO.setmode(GPIO.BOARD) 
GPIO.setwarnings(False)

#pins 11 is the state sent from the DE2
GPIO.setup(11, GPIO.IN)

# pin 7 is the Pi write Request 
GPIO.setup(7, GPIO.OUT)              
                        
# pin 3 is the fake clock. 
GPIO.setup(3,GPIO.OUT)

#pin 5 is the write enable
GPIO.setup(5, GPIO.OUT) 

#data bus
GPIO.setup(19, GPIO.OUT) 
GPIO.setup(21, GPIO.OUT) 
GPIO.setup(23, GPIO.OUT) 
GPIO.setup(29, GPIO.OUT)
GPIO.setup(31, GPIO.OUT) 
GPIO.setup(33, GPIO.OUT) 
GPIO.setup(35, GPIO.OUT) 
GPIO.setup(37, GPIO.OUT)  

#adress bus
GPIO.setup(40, GPIO.OUT) 
GPIO.setup(38, GPIO.OUT) 
GPIO.setup(36, GPIO.OUT) 
GPIO.setup(32, GPIO.OUT)
GPIO.setup(26, GPIO.OUT) 
GPIO.setup(24, GPIO.OUT) 
GPIO.setup(22, GPIO.OUT) 
GPIO.setup(18, GPIO.OUT)
GPIO.setup(16, GPIO.OUT) 
GPIO.setup(12, GPIO.OUT) 
GPIO.setup(10, GPIO.OUT) 
GPIO.setup(8, GPIO.OUT)  



def toDatBus( dat ):
	#pin37 is most significant bit
	GPIO.output(37, dat[0]) 
	GPIO.output(35, dat[1]) 
	GPIO.output(33, dat[2]) 
	GPIO.output(31, dat[3])
	GPIO.output(29, dat[4]) 
	GPIO.output(23, dat[5]) 
	GPIO.output(21, dat[6]) 
	GPIO.output(19, dat[7])
#	print "set DatBus to:", dat.bin
	return

def toAdBus( ad ):
	#pin26 is most significant bit
	GPIO.output(40, ad[0]) 
	GPIO.output(38, ad[1]) 
	GPIO.output(36, ad[2]) 
	GPIO.output(32, ad[3])
	GPIO.output(26, ad[4]) 
	GPIO.output(24, ad[5]) 
	GPIO.output(22, ad[6]) 
	GPIO.output(18, ad[7])
	GPIO.output(16, ad[8]) 
	GPIO.output(12, ad[9]) 
	GPIO.output(10, ad[10]) 
	GPIO.output(8, ad[11])
#	print "set AdBus to:", adress.bin
	return

def sendPath(Xpath, Ypath):
	pathX = Xpath
	pathY = Ypath
   	GPIO.output(3, False) #clock and write enable to 0
   	GPIO.output(5, False)
	GPIO.output(7, True)
	print "SHOULD BE in state 4"
	counter =0
	length = len(pathX)
	print "length", length
	numBytes = 3* length

	######################
	#First byte informs DE2 if another transfer will be needed
	#after DE2 finshes reading the current data being sent
	adress = BitArray(bin='{0:012b}'.format(counter))
	data = BitArray('0b00001100') 	#no more data
	toDatBus( data )
	toAdBus( adress )
	GPIO.output(5,True) #write enable =1
	GPIO.output(3,True) #fake clock high
	pass
	GPIO.output(3,False)
	GPIO.output(5,False) 
	print "FIRST BIT=", data.bin , "to adress", adress.bin
	counter += 1

	numLeft = BitArray(bin='{0:016b}'.format(numBytes))

	mostSig = numLeft[0:8]

	leastSig = numLeft[8:16]
	print "LAST number of bits sending=", numLeft.bin,"split into", mostSig.bin , leastSig.bin

	#second + third byte is # of bytes sending (normally 4093)    
	adress = BitArray(bin='{0:012b}'.format(counter)) 
	data = mostSig
	toDatBus( data )
	toAdBus( adress )
	GPIO.output(5,True) #write enable =1
	GPIO.output(3,True) #fake clock high
	pass
	GPIO.output(3,False)
	GPIO.output(5,False) 
	counter += 1
	print "2nd BIT=", data.bin , "to adress", adress.bin

	adress = BitArray(bin='{0:012b}'.format(counter)) 
	data = leastSig
	toDatBus( data )
	toAdBus( adress )
	GPIO.output(5,True) #write enable =1
	GPIO.output(3,True) #fake clock high
	pass
	GPIO.output(3,False)
	GPIO.output(5,False) 
	counter += 1
	print "3rd BIT=", data.bin , "to adress", adress.bin
	
	

	
	dotCount = 0
	#######
	while (dotCount < length): 
	    tempX = pathX.pop()
	    numLeft = BitArray(bin='{0:016b}'.format(tempX))
	    mostSig = numLeft[0:8]
	    leastSig = numLeft[8:16]
	    tempY = pathY.pop()
	    numRight = BitArray(bin='{0:08b}'.format(tempY))
	    print "sent coord:", tempX, tempY
#	    print "X was", tempX, "and became", mostSig.bin, leastSig.bin
#	    print "Y was", tempY, "and became", numRight.bin
	    
	    data = mostSig
	    adress = BitArray(bin='{0:012b}'.format(counter))
	    toDatBus( data )
	    toAdBus( adress )
	    GPIO.output(5,True) #write enable =1
	    GPIO.output(3,True) #fake clock high
	    pass
	    GPIO.output(3,False)
	    GPIO.output(5,False)
#	    print "sent XmostSig=", data.bin, "to", adress.bin
	    counter += 1


	    data = leastSig
	    adress = BitArray(bin='{0:012b}'.format(counter))
	    toDatBus( data )
	    toAdBus( adress )
	    GPIO.output(5,True) #write enable =1
	    GPIO.output(3,True) #fake clock high
	    pass
	    GPIO.output(3,False)
	    GPIO.output(5,False)
#	    print "sent XleastSig=", data.bin, "to", adress.bin
	    counter += 1
	   
	    
	    data = numRight
	    adress = BitArray(bin='{0:012b}'.format(counter))
	    toDatBus( data )
	    toAdBus( adress )
	    GPIO.output(5,True) #write enable =1
	    GPIO.output(3,True) #fake clock high
	    pass
	    GPIO.output(3,False)
	    GPIO.output(5,False)
#	    print "sent Y", data.bin, "to", adress.bin
	    counter += 1

	    dotCount +=1 

	GPIO.output(7, False)
	print "state 5"
	time.sleep(1)
	GPIO.output(7, True)
	print "state6"
	return
