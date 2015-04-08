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



# clock and write enable to 0 
GPIO.output(3, False)
GPIO.output(5, False)
GPIO.output(7, False) 
 

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
def delay():
	pass
	pass
	pass
	pass
	pass
	pass
	pass

try:
    #in total want to send 4096 bytes of data
    
    #Fist byte iforms DE2 if another transfer will be needed
    #after the DE2 finshes reading the current data being input
    
    data = BitArray('0b00000000')
    adress = BitArray('0b000000000000')
    #send pi write request
    GPIO.output(7, True)
    
    
    counter = 0    
    x = 0
    y = 0
    xMax = img.width
    yMax = img.height
    colour = 5
    temp = 3
    
    print "starting writing"
	
    GPIO.output(3,True) #fake clock high
    delay()
    GPIO.output(3,False)
   
    #Fist byte iforms DE2 if another transfer will be needed
    #after the DE2 finshes reading the current data being input
    adress = BitArray(bin='{0:012b}'.format(counter))
    data = BitArray('0b00000000') # has more data to send after this
    toDatBus( data )
    toAdBus( adress )
    GPIO.output(5,True) #write enable =1
    GPIO.output(3,True) #fake clock high
    delay()
    GPIO.output(3,False)
    GPIO.output(5,False) 
    print "adress was", adress.bin , "wrote", data.bin
    counter += 1

        
    #second + third byte is # of bytes sending (normally 4093)    
    adress = BitArray(bin='{0:012b}'.format(counter)) 
    data = BitArray('0b00001111')
    toDatBus( data )
    toAdBus( adress )
    GPIO.output(5,True) #write enable =1
    GPIO.output(3,True) #fake clock high
    delay()
    GPIO.output(3,False)
    GPIO.output(5,False)
    print "adress was", adress.bin , "wrote", data.bin 
    counter += 1

    adress = BitArray(bin='{0:012b}'.format(counter)) 
    data = BitArray('0b11111101')
    toDatBus( data )
    toAdBus( adress )
    GPIO.output(5,True) #write enable =1
    GPIO.output(3,True) #fake clock high
    delay()
    GPIO.output(3,False)
    GPIO.output(5,False) 
    print "adress was", adress.bin , "wrote", data.bin
    counter += 1

    #use this to set up the info you want to transfer
    #write 4092 bytes of data
    while counter < 4096: 

        red, green, blue = img[x, y]
	
    
        data = BitArray(bin='{0:08b}'.format(colour))
	adress = BitArray(bin='{0:012b}'.format(counter))
        toDatBus( data )
        toAdBus( adress )
        GPIO.output(5,True) #write enable =1
        GPIO.output(3,True) #fake clock high
        delay()
        GPIO.output(3,False)
        GPIO.output(5,False)   
        counter += 1
	temp +=1

	if(temp > 319):
		temp =0
		print "drew line of %d" % colour 
		colour +=1
	if(colour >10):
		colour = 1
	
       

    print "done writing- hope it worked"
        
except KeyboardInterrupt:
    # here you put any code you want to run before the program 
    # exits when you press CTRL+C
    print "Interupted when counter is at: %d" % counter # print value of counter
        
finally:
    GPIO.output(7, False)
    GPIO.cleanup() # this ensures a clean exit
