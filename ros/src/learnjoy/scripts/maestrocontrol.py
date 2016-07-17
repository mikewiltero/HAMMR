import serial
import time
import math
#from Tkinter import *

#master = Tk()

#w = Scale(master, from_=0, to=43, orient=HORIZONTAL, resolution=0.5, length=300, label='Position')
#x = Scale(master, from_=1, to=100, orient=HORIZONTAL, length=300, label='Speed')
#w.pack()
#x.pack()

angle_dest = 0.0
current_pos = 100000000 #some large number that will never = destination
scale_factor = .0075385366 #converts integer values from getPosition to degrees; value calculated as average value of angle/integer measurements
timeToExit = False

#Variables to set
destination = 40 #range: 0.00-43.33 mm 
speed = 7 #range: integer 1-300 

class ServoController():
    def __init__(self):
        self.sc = serial.Serial('/dev/ttyACM0', 9600)

    def closeServo(self):
        self.sc.close()

    def setAngle(self, n, angle):
        if angle > 180 or angle <0:
           angle=90
        byteone=int(254*angle/180)
        bud=chr(0xFF)+chr(n)+chr(byteone)
        self.sc.write(bud)

    def setPosition(self, servo, position):
        position = position * 4
        poslo = (position & 0x7f)
        poshi = (position >> 7) & 0x7f
        chan  = servo &0x7f
        data =  chr(0xaa) + chr(0x0c) + chr(0x04) + chr(chan) + chr(poslo) + chr(poshi)
        self.sc.write(data)
        
    def setSpeed(self, channel, speed):
        """Set the speed of the given channel.  Speed is given in units of 0.25us / 10ms.  This means there is a range from 1 to 40000.  Getting a handle of what this _actually_ means in practice, in terms of the visual speed of the motors, will take a bit of work."""
        commandByte = chr(0x87)
        channelByte = chr(channel)
        lowTargetByte = chr(speed & 0x7F)
        highTargetByte = chr((speed >> 7) & 0x7F)
        command = commandByte + channelByte + lowTargetByte + highTargetByte
        self.sc.write(command)
        
    def getPosition(self, servo):
        chan  = servo &0x7f
        data =  chr(0xaa) + chr(0x0c) + chr(0x10) + chr(chan)
        self.sc.write(data)
        w1 = ord(self.sc.read())
        w2 = ord(self.sc.read())
        position = w1 + 256*w2 #reconstruct integer from two little endian bytes
        return position

    def getErrors(self):
        data =  chr(0xaa) + chr(0x0c) + chr(0x21)
        self.sc.write(data)
        w1 = ord(self.sc.read())
        w2 = ord(self.sc.read())
        return w1, w2

    def triggerScript(self, subNumber):
        data =  chr(0xaa) + chr(0x0c) + chr(0x21) + chr(0x27) + chr(subNumber)
        self.sc.write(data)
        w1 = ord(self.sc.read())
        w2 = ord(self.sc.read())
        return w1, w2
    
    def angle_to_mm(self, angle):
        angle = math.radians(angle) #convert to radians to make math.cos() happy
        mm = 43.33 - math.cos(angle) * 44  #mm position = mm fully extended - calculated mm position (since 180 degrees = fully extended, and angle range is 30-180)
        return mm
    
    def mm_to_angle(self,mm):
        if (mm >= 0.0) and (mm <= 43.33): 
            angle = math.acos((43.33-mm)/44.0) #decimals to force float conversion, since calculations only work with floats; math doesn't like fractions 
            angle = math.degrees(angle)
            return angle
            
Maestro = ServoController()

#setAngle Range: 10 (retracted) to 90 (extended)
def controller():
    global current_pos,destination,speed
    destination = w.get()
    speed = x.get()
    Maestro.setSpeed(0, speed)
    if destination >= 0 and destination <= 43.33:
        if math.fabs(current_pos-destination) > .5:
            angle_dest = Maestro.mm_to_angle(destination)
            try:
                current_pos = math.fabs(Maestro.angle_to_mm((Maestro.getPosition(0) - 4095)*.02381401523))
                print "%.1f" % current_pos
                Maestro.setAngle(0, angle_dest * 2) #servo input angle range is 0-180, but actual mechanical range is 0-90. Therefore, multiply desired angle by two to achieve intended result.
                time.sleep(.05)  
            except:
                Maestro.setAngle(0, angle_dest * 2) #servo input angle range is 0-180, but actual mechanical range is 0-90. Therefore, multiply desired angle by two to achieve intended result.
                time.sleep(.05)

    else: 
        print "\nPlease set destination between 0.00 and 43.33 mm\n"
    #x.after(1,controller)
    
#x.after(1,controller)
#x.mainloop()
            
