#Adam's write to DE2 program
# Import the required module. 
import RPi.GPIO as GPIO
from bitstring import BitArray, BitStream 
# Set the mode to use physical numbering the pins. 

GPIO.setmode(GPIO.BOARD) 
# pin 3 is the PWM clock attempt. 
GPIO.setup(3, GPIO.OUT)

p = GPIO.PWM(3, 50)    # create an object p for PWM on port 25 at 50 Hertz  
p.start(50)             # start the PWM on 50 percent duty cycle              
                        
# pin 7 is the fake clock. 
GPIO.setup(7,GPIO.OUT)

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
	print "set DatBus to:", dat.bin
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
	print "set AdBus to:", dat.bin
	return

try:
    # here you put your main loop or block of code
    p.start(50) 
    #p.ChangeFrequency(100)  # change the frequency to # Hz  
    
    while counter < 3000000:
    
        # count up to 3000000 - takes ~7s
        counter += 1
        GPIO.output(7,True)
        time.sleep(10)
        GPIO.output(7,False)
        time.sleep(10)    
        print "Target reached: %d" % counter
        
except KeyboardInterrupt:
    # here you put any code you want to run before the program 
    # exits when you press CTRL+C
    print "Exits when counter is at: %d" % counter # print value of counter
        
finally:
    p.stop()                # stop the PWM output
    GPIO.cleanup() # this ensures a clean exit
