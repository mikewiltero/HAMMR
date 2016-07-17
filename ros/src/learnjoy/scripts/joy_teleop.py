#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy
import sys
from std_msgs.msg import UInt16
import time
 
def callback(data):
	twist = Twist()
	
	# using the Twist type to get info from the joystick. the constant is a speed multiplier
	twist.angular.x = 160*data.axes[3] # move forward and backward
	#sys.stdout.write("%f" %twist.angular.x)
	# twist.angular.x is the thing that has the data for the x axis
	
	# translate the raw joy value to a value friendly to the arduino servo function (value between 0-180
	farts = translate(twist.angular.x, -160, 160, 0, 160)
	pub.publish(farts)

	
 
def joy_teleop():
	rospy.init_node('Joy_teleop')
	rospy.Subscriber("joy", Joy, callback)
	global pub
	pub = rospy.Publisher('servo', UInt16, queue_size=10)
 
	r = rospy.Rate(10) # 10hz
	while not rospy.is_shutdown():
		r.sleep()
 
	# spin() simply keeps python from exiting until this node is stopped
	rospy.spin()
	
# this function is similar to arduino's Map function.
# "value" is the value needing to be translated
# leftMin is the lower end of the incoming value
# leftMax is the upper end of the incoming value
# rightMin is the lower end of the translated value
# rightMax is the upper end of the translated value
# 
# Source: http://stackoverflow.com/questions/1969240/mapping-a-range-of-values-to-another
def translate(value, leftMin, leftMax, rightMin, rightMax):
	#figure out how 'wide' each range is
	leftSpan = leftMax - leftMin
	rightSpan = rightMax - rightMin
	
	#convert the left range into 0-1 range (float)
	valueScaled = float(value - leftMin) / float(leftSpan)
	
	#convert the 0-1 range into a value in the right range.
	return rightMin + (valueScaled * rightSpan)
 
if __name__ == '__main__':
	joy_teleop()