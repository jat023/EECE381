#Adam's write to DE2 program
# Import the required module. 
import RPi.GPIO as GPIO 
# Set the mode to use physical numbering the pins. 

GPIO.setmode(GPIO.BCM) 
# pin 3 is the clock. 
GPIO.setup(3, GPIO.OUT)
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

try:
    # here you put your main loop or block of code

    while counter < 3000000:
        # count up to 3000000 - takes ~7s
        counter += 1
        GPIO.output(3,True)
        time.sleep(10)
        GPIO.output(3,False)
        time.sleep(10)    
        print "Target reached: %d" % counter
        
except KeyboardInterrupt:
    # here you put any code you want to run before the program 
    # exits when you press CTRL+C
    print "Exits when counter is at:", counter # print value of counter
        
finally:
    GPIO.cleanup() # this ensures a clean exit