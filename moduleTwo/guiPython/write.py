#Adam's write to DE2 program
# Import the required module.
import time
import RPi.GPIO as GPIO
from bitstring import BitArray, BitStream 
import imgproc
from imgproc import *
# Set the mode to use physical numbering the pins.

img = Image("/home/pi/Desktop/381_colours.bmp")
 
GPIO.setmode(GPIO.BOARD)	#set pin mode of GPIO on Raspberry Pi
GPIO.setwarnings(False)

GPIO.setup(11, GPIO.IN)		#pins 11 is the state sent from the DE2
GPIO.setup(7, GPIO.OUT)		#pin 7 is the Pi write Request              
GPIO.setup(3,GPIO.OUT)		#pin 3 is the fake clock.
GPIO.setup(5, GPIO.OUT)		#pin 5 is the write enable

GPIO.setup(19, GPIO.OUT)	#data bus setup
GPIO.setup(21, GPIO.OUT) 
GPIO.setup(23, GPIO.OUT) 
GPIO.setup(29, GPIO.OUT)
GPIO.setup(31, GPIO.OUT) 
GPIO.setup(33, GPIO.OUT) 
GPIO.setup(35, GPIO.OUT) 
GPIO.setup(37, GPIO.OUT)  

GPIO.setup(40, GPIO.OUT)	#address bus setup
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

#write 4096 bytes to the DE2 each cycle until full map is sent
def writeToDe2():
	GPIO.output(3, False) #clock and write enable to 0
	GPIO.output(5, False)
	GPIO.output(7, False) #pi request bit from Pi GPIO
	        
	x = 0
	y = 0
	xMax = img.width
	yMax = img.height
	print img.width, img.height
	numPixel = xMax * yMax
	pixCount = 0
	done = 0
	GPIO.output(7, True)
	print "Initialization complete."
	
	while( done == 0): 
		counter = 0

		GPIO.output(3,True) #fake clock high
		pass
		GPIO.output(3,False)

		print (pixCount + 4093), "vs", numPixel
		if ((pixCount + 4093) < numPixel):

			#First byte informs DE2 if another transfer is needed
			#after DE2 finshes reading the current data input
			adress = BitArray(bin='{0:012b}'.format(counter))
			data = BitArray('0b00000001') # has more data to send after this
			toDatBus( data )
			toAdBus( adress )
			GPIO.output(5,True) #write enable =1
			GPIO.output(3,True) #fake clock high
			pass
			GPIO.output(3,False)
			GPIO.output(5,False) 
			counter += 1

		
			#second + third byte is # of bytes sending (normally 4093)    
			adress = BitArray(bin='{0:012b}'.format(counter)) 
			data = BitArray('0b00001111')
			toDatBus( data )
			toAdBus( adress )
			GPIO.output(5,True) #write enable =1
			GPIO.output(3,True) #fake clock high
			pass
			GPIO.output(3,False)
			GPIO.output(5,False)
			counter += 1

			adress = BitArray(bin='{0:012b}'.format(counter)) 
			data = BitArray('0b11111101')
			toDatBus( data )
			toAdBus( adress )
			GPIO.output(5,True) #write enable =1
			GPIO.output(3,True) #fake clock high
			pass
			GPIO.output(3,False)
			GPIO.output(5,False) 
			counter += 1
		else:
			#last chunck of data
			done =1

			#First byte informs DE2 if another transfer will be needed
				#after DE2 finshes reading the current data being sent
			adress = BitArray(bin='{0:012b}'.format(counter))
			data = BitArray('0b00000000') 	#has more data to send after this
			toDatBus( data )
			toAdBus( adress )
			GPIO.output(5,True) #write enable =1
			GPIO.output(3,True) #fake clock high
			pass
		
			GPIO.output(3,False)
			GPIO.output(5,False) 
			print "last section data: sent", data.bin , "to", adress.bin
			
			numLeft = BitArray(bin='{0:016b}'.format(numPixel - pixCount))
			print "num left is", numLeft.bin
			mostSig = numLeft[0:8]
			print "first 8 bits are", mostSig.bin
			leastSig = numLeft[8:16]
			print "last 8 bits are", leastSig.bin

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
	    
		#write 4093 bytes of data to DE2 for te map
		while counter < 4096: 
			red, green, blue = img[x, y]
			
			# sets data if white, dark green, medium green, or light green
			if ( green > 225 ):
				#sets datat if white	
				if (red > 225  and blue > 225):
					data = BitArray('0b00000001')
				# sets data if (medium green)
				elif (red < 170 and  blue < 145):
					data = BitArray('0b00000011')
				# sets data to (light green)
				else:
					data = BitArray('0b00000100')
			#set if purple, black or brown
			elif ( green < 140 ):
				# sets data if purple
				if (red > 225 and blue > 225):
					data = BitArray('0b00000111')
				# sets data if black
				elif (red < 50 and blue < 50):
					data = BitArray('0b00000110')
				#must be brown
				else :
					data = BitArray('0b00000110')		
			#sets if  blue, olive green or yellow
			else :
				# sets data if blue
				if ( blue > 200):
					data = BitArray('0b00001000')
				# sets data if yellow
				elif (red > 205 and blue > 40 ):
					data = BitArray('0b00000101')
				# sets data if olive green
				elif (red > 80 and blue < 41):
					data = BitArray('0b00000111')
				# sets data if (dark green) 
				elif( red < 81 and blue <100) :
					data = BitArray('0b00000010')
				else:
					data = BitArray('0b00000000')

			adress = BitArray(bin='{0:012b}'.format(counter))
			toDatBus( data )
			toAdBus( adress )
			GPIO.output(5,True) #write enable =1
			GPIO.output(3,True) #fake clock high
			pass
			
			GPIO.output(3,False)
			GPIO.output(5,False)
			counter += 1
			pixCount +=1    	

			if ( x < (xMax-1) ):
				x+=1
			else:
				x=0
				y+=1
			#print "adress was", adress.bin , "wrote", data.bin

		if (done == 1):
			GPIO.output(7, False)
			print "done drawing picture"
			break
		else:
			GPIO.output(7, False)
			time.sleep(.001)
			GPIO.output(7, True)
			print "another loop: drawn %d pixels" % pixCount
		
	    #input dropping to 0 will signal DE2 ready for new data
		while (GPIO.input(11)):
			pass
	
	print "done writing- hope it worked"

def cleanUp():
	GPIO.cleanup() # this ensures a clean exit


