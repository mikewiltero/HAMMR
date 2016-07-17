#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy
import sys
 
def callback(data):
	twist = Twist()
	
	# using the Twist type to get info from the joystick. the constant is a speed multiplier
	twist.linear.x = 6*data.axes[] # move forward and backward
	twist.linear.z = 6*data.axes[3] # yaw left/right
	twist.linear.y = 6*data.axes[]
	twist.angular.x = 6*data.axes[0]
	twist.angular.y = 6*data.axes[1]
	twist.angular.z = 6*data.axes[2]
	pub.publish(twist)
	
 
def joy_teleop():
	rospy.init_node('Joy_teleop')
	rospy.Subscriber("joy", Joy, callback)
	global pub
	pub = rospy.Publisher('turtle1/cmd_vel', Twist, queue_size=10)
 
	r = rospy.Rate(10) # 10hz
	while not rospy.is_shutdown():
		r.sleep()
 
	# spin() simply keeps python from exiting until this node is stopped
	rospy.spin()
 
if __name__ == '__main__':
	joy_teleop()