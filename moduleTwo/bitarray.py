from bitstring import BitArray, BitStream, Bits

#from time import sleep
import time

#import imgproc
#from imgproc import *
from PIL import Image

test = Image.open("/home/pi/Desktop/381map.bmp")
test.show()

data = BitArray('0b10010011')

def toBus( dat ):
	a = dat[0]
	b=  dat[1]
	c=  dat[7]
	print(dat.bin)
	return


toBus(data)

a = data[0]
b=  data[1]
c=  data[7]


d= BitArray('0b1')
#d.append('0b1')
print d.bin
d +=Bits(a)
print d.bin
d += Bits(b)
print d.bin
d.append(Bits(c))
print d.bin

print "a= ", a
time.sleep(1)
print "b=", b
time.sleep(.009)
print "c= ", c 
print(d)
print(d.bin)
