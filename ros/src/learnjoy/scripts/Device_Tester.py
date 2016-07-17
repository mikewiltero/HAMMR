#!/usr/bin/python3
from Device import Device
import time

#create servo object, set unlimited speed, move servo to 90 degrees angle
servo = Device("/dev/ttyACM0","/dev/ttyACM1")
servo.set_speed(0,5)
servo.set_target(0, 7000)
time.sleep(1)

d=0
loop=0

