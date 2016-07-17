#!/usr/bin/env python
import serial
#import ServoController
from time import sleep, time
import roslib
import rospy
from geometry_msgs.msg import Twist
import sys
import maestro


#SURGE_LEFT = 1
#SURGE_RIGHT = 0

#YAW_LEFT = 2
#YAW_RIGHT = 3
#servoTest = 0

#lets first see if we can put it in teh callback function
#def servoGo():
	#ServoController.setTarget(servoTest, 1700)
	
	
servo = maestro.Controller()

	
def callback(msg):
	#callback things
	sys.stdout.write("Linear: [%f, %f, %f] | Angular: [%f, %f, %f] \n" %(msg.linear.x, msg.linear.y, msg.linear.z, msg.angular.x, msg.angular.y, msg.angular.z))
	servo.setAccel(0,4)		# set servo 0 acceleration to 4
	servo.setTarget(0,6000)	# set servo to move to center position
	servo.close

def joy_servo():
    rospy.init_node('joy_servo')
    rospy.Subscriber("turtle1/cmd_vel", Twist, callback)
    rospy.spin()

if __name__ == '__main__':
    joy_servo()