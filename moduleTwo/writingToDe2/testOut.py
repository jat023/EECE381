#Adam's write to DE2 program
# Import the required module.
import time
import RPi.GPIO as GPIO
from bitstring import BitArray, BitStream 
import imgproc
from imgproc import *
# Set the mode to use physical numbering the pins.

img = Image("/home/pi/Desktop/381_colours.bmp")
 
print "Started code"

GPIO.setmode(GPIO.BOARD) 

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

def pass:
	pass
	pass
	pass
	pass
	pass
	pass
	pass

print "starting try block"

def fakeDraw():
    # clock and write enable to 0 
    GPIO.output(3, False)
    GPIO.output(5, False)
    GPIO.output(7, False) #request
       
    x = 0
    y = 0
    xMax = img.width
    yMax = img.height
    print img.width, img.height
    numPixel = xMax * yMax
    pixCount = 0
    done = 0
    GPIO.output(7, True)
    print "completed initialization"
   
    
	counter = 0

	GPIO.output(3,True) #fake clock high
	pass
	GPIO.output(3,False)
	
	
    #last chunck of data
    done =1

    #Fist byte iforms DE2 if another transfer will be needed
    #after the DE2 finshes reading the current data being input
    data = BitArray('0b00001100') # has more data to send after this
    adress = BitArray(bin='{0:012b}'.format(counter))
    toDatBus( data )
    toAdBus( adress )
    GPIO.output(5,True) #write enable =1
    GPIO.output(3,True) #fake clock high
    pass
   
    GPIO.output(3,False)
    GPIO.output(5,False) 
    print "last section data: sent", data.bin , "to", adress.bin

    
    numLeft = BitArray(bin='{0:016b}'.format(3900))
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
	    
	   
	#write 4093 bytes of data
	while counter < 4096: 

		data = BitArray('0b00000010')

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

		

	
    GPIO.output(7, False)
    print "done drawing picture"
    return
		
 def sendPath()   
    pathX = [10,50,300]
    pathY = [10,50,200]
    ##################
    #need to update to use len()
    lenght = 3*len(pathX)
    pixCount = 0
    done = 0
    
    print "completed initialization"
   
    while( done == 0): 
	
        counter = 0
        GPIO.output(7, True)
        
        GPIO.output(3,True) #fake clock high
        pass
        GPIO.output(3,False)
        
        print (pixCount + 4093), "vs", lenght
            if((pixCount + 4093) < lenght):


            #Fist byte iforms DE2 if another transfer will be needed
            #after the DE2 finshes reading the current data being input
            adress = BitArray(bin='{0:012b}'.format(counter))
            data = BitArray('0b00000001') # has more data to send after this
            print "more data: sent", data.bin , "to", adress.bin
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

            #Fist byte iforms DE2 if another transfer will be needed
            #after the DE2 finshes reading the current data being input
            data = BitArray('0b00001100') # has more data to send after this
            adress = BitArray(bin='{0:012b}'.format(counter))
            toDatBus( data )
            toAdBus( adress )
            GPIO.output(5,True) #write enable =1
            GPIO.output(3,True) #fake clock high
            pass
           
            GPIO.output(3,False)
            GPIO.output(5,False) 
            print "last section data: sent", data.bin , "to", adress.bin

            
            numLeft = BitArray(bin='{0:016b}'.format(lenght - pixCount))
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
            
           
        #write 4093 bytes of data
        while counter < 4096: 
            numLeft = BitArray(bin='{0:016b}'.format(pathX.pop()))
            print "num left is", numLeft.bin
            mostSig = numLeft[0:8]
            print "first 8 bits are", mostSig.bin
            leastSig = numLeft[8:16]
            print "last 8 bits are", leastSig.bin
            numRight = BitArray(bin='{0:08b}'.format(pathY.pop()))
            
            data = itArray(bin='{0:016b}'.format(mostSig))
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

            data = itArray(bin='{0:016b}'.format(leastSig))
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
            
            data = itArray(bin='{0:016b}'.format(numRight))
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


        if(done == 1):
            GPIO.output(7, False)
            print "done drawing picture"
            break
        else:
            GPIO.output(7, False)
            print "another loop: drawn %d pixels" % pixCount
        
        
        #input dropping to 0 will signal DE2 ready for new data
        while (GPIO.input(11)):
            pass
	
   
        
except KeyboardInterrupt:
    # here you put any code you want to run before the program 
    # exits when you press CTRL+C
    GPIO.output(7, False)
    print "Interupted -> Exiting..." % counter # print value of counter
        
finally:
    GPIO.output(7, False)
    GPIO.cleanup() # this ensures a clean exit
