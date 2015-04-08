#Adam's write to DE2 program
# Import the required module.
import time
import RPi.GPIO as GPIO
from bitstring import BitArray, BitStream 
import imgproc
from imgproc import *
# Set the mode to use physical numbering the pins.

img = Image("/home/pi/Desktop/381_colours.bmp")
 

GPIO.setmode(GPIO.BOARD) 

#pins 11,13,15 are the state sent from the DE2
GPIO.setup(11, GPIO.IN)
GPIO.setup(13, GPIO.IN)
GPIO.setup(15, GPIO.IN) 

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

GPIO.output(3, False)
GPIO.output(5, False)
GPIO.output(7, False) #request

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

def setData( red, green, blue):
	if( green > 225 ):

		#sets datat if white	
		if (red > 225  and blue > 225):
			data = BitArray('0b00000001')
			if(t1==0):
				t1=1
				print "white"

		# sets data if (medium green)
		elif (red < 170 and  blue < 145):
			data = BitArray('0b00000011')
			if(t2==0):
				t2=1
				print "M green"

		# sets data to (light green)
		else:
			data = BitArray('0b00000100')
			if(t3==0):
				t3=1
				print "Light green"

	#set if purple, black or brown
	elif( green < 140 ):
		# sets data if purple
		if (red > 225 and blue > 225):
			data = BitArray('0b00000111')
			if(t4==0):
				t4=1
				print "purple"
		# sets data if black
		elif (red < 50 and blue < 50):
			data = BitArray('0b00001010')
			if(t5==0):
				t5=1
				print "black"

		#must be brown
		else :
			data = BitArray('0b00000110')		
			if(t6==0):
				t6=1
				print "brown"
	#sets if  blue, olive green or yellow
	else :
		# sets data if blue
		if ( blue > 200):
			data = BitArray('0b00000111')
			if(t7==0):
				t7=1
				print "blue"

		# sets data if yellow
		elif (red > 205 and blue > 40 ):
			data = BitArray('0b00000101')
			if(t8==0):
				t8=1
				print "yellow"

		
		# sets data if olive green
		elif (red > 80 and blue < 41):
			data = BitArray('0b00000111')
			if(t9==0):
				t9=1
				print "olive green"
		
		# sets data if (dark green) 
		elif( red < 81 and blue <100) :
			data = BitArray('0b00000010')
			if(t10==0):
				t10=1
				print "dark green"

		else:
			print red, green, blue

try:
    # clock and write enable to 0 
   
    #in total want to send 4096 bytes of data
    #send pi write request
    
   
    counter = 0    
    x = 0
    y = 0
    xMax = img.width
    yMax = img.height
    numPixel = xMax * yMax
    t1=0
    t2 =0
    t3 =0
    t4 =0
    t5 =0
    t6 =0
    t7 =0
    t8 =0
    t9=0
    t10 =0
   
    red, green, blue = img[x, y]
    x +=1
    redLast, greenLast, blueLast = img[x, y]
    print red, green, blue
    
   
    #write 4093 bytes of data
    while counter < numPixel: 

	red, green, blue = img[x, y]
	# sets data if white, dark green, medium green, or light green
	setData( red, green, blue)

	
	counter +=1

	if ( x < xMax ):
            x+=1
        else:
            x=0
            y+=1

	

        

    print "done writing- hope it worked"
        
except KeyboardInterrupt:
    # here you put any code you want to run before the program 
    # exits when you press CTRL+C
    print "Interupted. Exiting now"
        
finally:
    GPIO.cleanup() # this ensures a clean exit
