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
	pub.publish(twist.angular.x)

	
 
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
 
if __name__ == '__main__':
	joy_teleop()