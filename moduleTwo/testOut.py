#Adam's write to DE2 program
# Import the required module.
import time
import RPi.GPIO as GPIO
from bitstring import BitArray, BitStream 
import imgproc
from imgproc import *
# Set the mode to use physical numbering the pins.

img = Image("/home/pi/Desktop/381map.bmp")

# iterate over each pixel in the image
for x in range(0, img.width):
	for y in range(0, img.height):
		# Get the value at the xth column and yth row, place the intensities into variables
		red, green, blue = img[x, y]

		#use this to set up the info you want to transfer
 

GPIO.setmode(GPIO.BOARD) 

# pin 7 is the PWM clock attempt. 
#GPIO.setup(7, GPIO.OUT)
#p = GPIO.PWM(7, 100)    # create an object p for PWM on port 25 at 100 Hertz  
#p.start(100)             # start the PWM on 50 percent duty cycle

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
#rest are for the bus

GPIO.setup(19, GPIO.OUT) 
GPIO.setup(21, GPIO.OUT) 
GPIO.setup(23, GPIO.OUT) 
GPIO.setup(29, GPIO.OUT)
GPIO.setup(31, GPIO.OUT) 
GPIO.setup(33, GPIO.OUT) 
GPIO.setup(35, GPIO.OUT) 
GPIO.setup(37, GPIO.OUT)  


# clock and write enable to 0 
GPIO.output(3, False)
GPIO.output(5, False)
GPIO.output(7, False) 

GPIO.output(19, True) 
GPIO.output(21, False) 
GPIO.output(23, True) 
GPIO.output(29, False)
GPIO.output(31, True) 
GPIO.output(33, False) 
GPIO.output(35, True) 
GPIO.output(37, False)

GPIO.output(5, True)

data = BitArray('0b10101010')
adress = BitArray('0b10101010')
 

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
	#pin8 is most significant bit
	GPIO.output(37, ad[0]) 
	GPIO.output(35, ad[1]) 
	GPIO.output(33, ad[2]) 
	GPIO.output(31, ad[3])
	GPIO.output(29, ad[4]) 
	GPIO.output(23, ad[5]) 
	GPIO.output(21, ad[6]) 
	GPIO.output(19, ad[7])
#	print "set AdBus to:", adress.bin
	return

try:
    #in total want to send 4096 bytes of data
    
    #Fist byte iforms DE2 if another transfer will be needed
    #after the DE2 finshes reading the current data being input
    adress = BitArray('0b00000000')
    
    toDatBus( data )
    toAdBus( adress )
    GPIO.output(5,True) #write enable =1
    GPIO.output(3,True) #fake clock high
    time.sleep(.001)
    GPIO.output(3,False)
    GPIO.output(5,False)    
        
    #second byte is length of     
    adress = BitArray('0b00000001')    
    toDatBus( data )
    toAdBus( adress )
    GPIO.output(5,True) #write enable =1
    GPIO.output(3,True) #fake clock high
    time.sleep(.001)
    GPIO.output(3,False)
    GPIO.output(5,False) 
    adress += '0b01'
    
    adress = BitArray('0b01010101')
    counter = 0
      
    while(1):
    
        # count up to 3000000 - takes ~7s
        toDatBus( data )
        toAdBus( adress )
        GPIO.output(5,True) #write enable =1
        GPIO.output(3,True) #fake clock high
        time.sleep(.5)
        GPIO.output(3,False)
        GPIO.output(5,False) 
        time.sleep(.5)   
       
        
except KeyboardInterrupt:
    # here you put any code you want to run before the program 
    # exits when you press CTRL+C
    print "Exits when counter is at: %d" % counter # print value of counter
        
finally:
#    p.stop()                # stop the PWM output
    GPIO.cleanup() # this ensures a clean exit
